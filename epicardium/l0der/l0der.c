#include "l0der/l0der.h"

#include <alloca.h>
#include <stdio.h>
#include <string.h>

#include "epicardium.h"
#include "l0der/elf.h"
#include "modules/log.h"

/*
 * l0der is, in reality, a boneless operating-system style ELF loader.
 *
 * To implement it, we parse an ELF file somewhat defensively, trying to
 * not DoS ourselves by overallocating RAM (no heap allocations, no recursion).
 *
 * Currently we support only relocatable, PIE binaries. Adding support for
 * static ELFs would be trivial, however we want to keep the possibility to
 * shuffle around memory areas in future versions of card10 (possibly giving
 * l0dables more RAM than 256k) without having to recompile all l0dables. We
 * are also keeping the opportunity to have separate loading schemes in the
 * future, for instance:
 *  - l0dables running next to pycardium, without unloading it
 *  - multiple l0dables running next to each other (TSR-style)
 *
 * Thus, we use PIE l0dables to keep these possibilities open and not write down
 * a memory map in stone.
 */

#define WEAK_SYMBOL_MAX 128

struct _pie_load_info {
	/// Populated by _load_pie
	// Addresses within ELF file.
	uint32_t image_start;
	uint32_t image_limit;
	// Highest alignment request for a segment.
	uint32_t strictest_alignment;

	/// Populated by _parse_dynamic_symbols
	// List of weak symbols for which relocations can be ignored.
	uint32_t weak_symbols[WEAK_SYMBOL_MAX];
	uint32_t weak_symbol_count;

	/// Populated by _get_load_addr
	// Load address of ELF file.
	uint32_t load_address;
	// Addresses within memory of ELF file.
	uint32_t image_load_start;
	uint32_t image_load_limit;
	// Stack top.
	uint32_t stack_top;
};

/*
 * Read an ELF header, check E_IDENT.
 */
static int _read_elf_header(int fd, Elf32_Ehdr *hdr)
{
	int res;

	epic_file_seek(fd, 0, SEEK_SET);

	if ((res = epic_file_read(fd, hdr, sizeof(Elf32_Ehdr))) !=
	    sizeof(Elf32_Ehdr)) {
		LOG_ERR("l0der", "_read_elf_header: read failed: %d", res);
		return res;
	}

	if (hdr->e_ident[0] != ELFMAG0 || hdr->e_ident[1] != ELFMAG1 ||
	    hdr->e_ident[2] != ELFMAG2 || hdr->e_ident[3] != ELFMAG3) {
		LOG_ERR("l0der", "_read_elf_header: not an ELF file");
		return -ENOEXEC;
	}

	if (hdr->e_ident[4] != ELFCLASS32) {
		LOG_ERR("l0der", "_read_elf_header: not a 32-bit ELF");
		return -ENOEXEC;
	}

	if (hdr->e_ident[5] != ELFDATA2LSB) {
		LOG_ERR("l0der", "_read_elf_header: not a little-endian ELF");
		return -ENOEXEC;
	}

	if (hdr->e_ident[6] != EV_CURRENT) {
		LOG_ERR("l0der", "_read_elf_header: not a v1 ELF");
		return -ENOEXEC;
	}

	if (hdr->e_ehsize < sizeof(Elf32_Ehdr)) {
		LOG_ERR("l0der", "_raed_elf_header: header too small");
		return -ENOEXEC;
	}

	return 0;
}

/*
 * Read bytes from file at a given offset.
 *
 * :param int fd: file pointer to read from
 * :param uint32_t address: address from which to read
 * :param void *data: buffer into which to read
 * :param size_t count: amount of bytes to read
 * :returns: ``0`` on success or a negative value on error.  Possible errors:
 *
 *	- ``-EIO``: Could not read from FAT - address out of bounds of not enough bytes available.
 */
static int _seek_and_read(int fd, uint32_t address, void *data, size_t count)
{
	int res;

	if ((res = epic_file_seek(fd, address, SEEK_SET)) != 0) {
		LOG_ERR("l0der",
			"_seek_and_read: could not seek to 0x%lx: %d",
			address,
			res);
		return res;
	}

	if ((res = epic_file_read(fd, data, count)) != count) {
		LOG_ERR("l0der", "_seek_and_read: could not read: %d", res);
		return res;
	}

	return 0;
}

/*
 * Read an ELF program header header.
 */
static int _read_program_header(int fd, uint32_t phdr_addr, Elf32_Phdr *phdr)
{
	return _seek_and_read(fd, phdr_addr, phdr, sizeof(Elf32_Phdr));
}

/*
 * Read an ELF section header header.
 */
static int _read_section_header(int fd, uint32_t shdr_addr, Elf32_Shdr *shdr)
{
	return _seek_and_read(fd, shdr_addr, shdr, sizeof(Elf32_Shdr));
}

/*
 * Check an ELF program header.
 *
 * This function ensures basic memory sanity of a program header / segment.
 * It ensures that it points to a file region that is contained within the file fully.
 */
static int _check_program_header(int fd, int size, Elf32_Phdr *phdr)
{
	// Check file size/offset.
	uint32_t file_start = phdr->p_offset;
	uint32_t file_limit = phdr->p_offset + phdr->p_filesz;
	if (file_limit < file_start) {
		LOG_ERR("l0der", "_check_program_header: file size overflow");
		return -ENOEXEC;
	}
	if (file_limit > size) {
		LOG_ERR("l0der",
			"_check_program_header: extends past end of file");
		return -ENOEXEC;
	}

	if (phdr->p_type == PT_LOAD) {
		// Check mem/file size.
		if (phdr->p_filesz > phdr->p_memsz) {
			LOG_ERR("l0der",
				"_check_program_header: file size larger than memory size");
			return -ENOEXEC;
		}

		uint32_t mem_start = phdr->p_vaddr;
		uint32_t mem_limit = phdr->p_vaddr + phdr->p_memsz;

		if (mem_limit < mem_start) {
			LOG_ERR("l0der",
				"_check_program_header: mem size overflow");
			return -ENOEXEC;
		}
	}

	return 0;
}

/*
 * Check an ELF section header.
 *
 * This function ensures basic memory sanity of a section header.
 * It ensures that it points to a file region that is contained within the file fully.
 */
static int _check_section_header(int fd, int size, Elf32_Shdr *shdr)
{
	// Check file size/offset.
	uint32_t file_start = shdr->sh_offset;
	uint32_t file_limit = shdr->sh_offset + shdr->sh_size;
	if (file_limit < file_start) {
		LOG_ERR("l0der", "_check_section_header: file size overflow");
		return -ENOEXEC;
	}
	if (file_limit > size) {
		LOG_ERR("l0der",
			"_check_section_header: extends past end of file");
		return -ENOEXEC;
	}

	return 0;
}

/*
 * Interpreter expected in l0dable PIE binaries.
 */
static const char *_interpreter = "card10-l0dable";

/*
 * Check that the given INTERP program header contains the correct interpreter string.
 */
static int _check_interp(int fd, Elf32_Phdr *phdr)
{
	int res;
	uint32_t buffer_size = strlen(_interpreter) + 1;
	char *interp         = alloca(buffer_size);
	memset(interp, 0, buffer_size);

	if ((res = _seek_and_read(fd, phdr->p_offset, interp, buffer_size)) !=
	    0) {
		return res;
	}

	if (strncmp(interp, _interpreter, strlen(_interpreter)) != 0) {
		LOG_ERR("l0der",
			"_check_interp: invalid interpreter, want card10-l0dable");
		return -1;
	}

	return 0;
}

/*
 * Calculate address at which binary should be loaded.
 *
 * Currently this means trying to fit it into core1 RAM.
 */
static int _get_load_addr(struct _pie_load_info *li)
{
	uint32_t image_size = li->image_limit - li->image_start;

	// ref: Documentation/memorymap.rst
	uint32_t core1_mem_start = 0x20040000;
	uint32_t core1_mem_limit = 0x20080000;
	uint32_t core1_mem_size  = core1_mem_limit - core1_mem_start;

	if (image_size > core1_mem_size) {
		LOG_ERR("l0der",
			"_get_load_addr: image too large (need 0x%08lx bytes, have %08lx",
			image_size,
			core1_mem_size);
		return -ENOMEM;
	}

	// Place image at bottom of core1 memory range.
	li->load_address     = core1_mem_start;
	li->image_load_start = li->load_address + li->image_start;
	li->image_load_limit = li->load_address + li->image_limit;

	// Ensure within alignment requests.
	if ((li->load_address % li->strictest_alignment) != 0) {
		LOG_ERR("l0der",
			"_get_load_addr: too strict alignment request for %ld bytes",
			li->strictest_alignment);
		return -ENOEXEC;
	}

	// Place stack at top of core1 memory range.
	li->stack_top = core1_mem_limit;

	// Check that there is enough stack space.
	uint32_t stack_space = li->stack_top - li->image_load_limit;
	if (stack_space < 8192) {
		LOG_WARN(
			"l0der",
			"_get_load_addr: low stack space (%ld bytes)",
			stack_space
		);
	} else if (stack_space < 256) {
		LOG_ERR("l0der",
			"_get_load_addr: low stack space (%ld bytes), cannot continue",
			stack_space);
		return -ENOMEM;
	}

	LOG_INFO(
		"l0der",
		"Stack at %08lx, %ld bytes available",
		li->stack_top,
		stack_space
	);

	return 0;
}

/*
 * Load a program segment into memory.
 *
 * Segment must be a LOAD segment.
 */
static int _load_segment(int fd, struct _pie_load_info *li, Elf32_Phdr *phdr)
{
	uint32_t segment_start = li->load_address + phdr->p_vaddr;
	uint32_t segment_limit = segment_start + phdr->p_memsz;

	LOG_INFO(
		"l0der",
		"Segment %08lx-%08lx: 0x%lx bytes from file",
		segment_start,
		segment_limit,
		phdr->p_filesz
	);
	memset((void *)segment_start, 0, phdr->p_memsz);

	return _seek_and_read(
		fd, phdr->p_offset, (void *)segment_start, phdr->p_filesz
	);
}

/*
 * Parse dynamic symbol sections.
 */
static int _parse_dynamic_symbols(
	int fd, int size, struct _pie_load_info *li, Elf32_Ehdr *hdr
) {
	int res;
	Elf32_Shdr shdr;
	Elf32_Sym sym;

	// Go through all dynamic symbol sections.
	for (int i = 0; i < hdr->e_shnum; i++) {
		uint32_t shdr_addr = hdr->e_shoff + (i * hdr->e_shentsize);
		if ((res = _read_section_header(fd, shdr_addr, &shdr)) != 0) {
			return res;
		}

		if (shdr.sh_type != SHT_DYNSYM) {
			continue;
		}

		if ((res = _check_section_header(fd, size, &shdr)) != 0) {
			return res;
		}

		if ((shdr.sh_size % sizeof(Elf32_Sym)) != 0) {
			LOG_ERR("l0der",
				"_parse_dynamic_symbols: SHT_DYN section with invalid size: %ld",
				shdr.sh_size);
			return -EIO;
		}
		uint32_t sym_count = shdr.sh_size / sizeof(Elf32_Sym);

		// Read symbols one by one.
		if ((res = epic_file_seek(fd, shdr.sh_offset, SEEK_SET)) != 0) {
			LOG_ERR("l0der",
				"_parse_dynamic_symbols: seek to first relocation (at 0x%lx) failed",
				shdr.sh_offset);
			return res;
		}

		for (int j = 0; j < sym_count; j++) {
			if ((res = epic_file_read(
				     fd, &sym, sizeof(Elf32_Sym))) !=
			    sizeof(Elf32_Sym)) {
				LOG_ERR("l0der",
					"__parse_dynamic_symbols: symbol read failed: %d",
					res);
				return res;
			}

			uint32_t bind = ELF32_ST_BIND(sym.st_info);
			if (bind != STB_WEAK) {
				continue;
			}

			if (li->weak_symbol_count >= WEAK_SYMBOL_MAX) {
				LOG_ERR("l0der",
					"__parse_dynamic_symbols: too many weak symbols (limit: %d)",
					WEAK_SYMBOL_MAX);
				return -ENOMEM;
			}

			li->weak_symbols[li->weak_symbol_count++] = j;
		}
	}

	return 0;
}

/*
 * Apply dynamic relocations from ELF.
 *
 * Currently, we only support R_ARM_RELATIVE relocations. These seem to be
 * the only one used when making 'standard' PIE binaries on RAM. However, other
 * kinds might have to be implemented in the future.
 */
static int
_run_relocations(int fd, int size, struct _pie_load_info *li, Elf32_Ehdr *hdr)
{
	int res;
	Elf32_Shdr shdr;
	Elf32_Rel rel;

	// Go through all relocation sections.
	for (int i = 0; i < hdr->e_shnum; i++) {
		uint32_t shdr_addr = hdr->e_shoff + (i * hdr->e_shentsize);
		if ((res = _read_section_header(fd, shdr_addr, &shdr)) != 0) {
			return res;
		}

		// We don't support RELA (relocation with addend) sections (yet?).
		if (shdr.sh_type == SHT_RELA) {
			LOG_ERR("l0der",
				"_run_relocations: found unsupported SHT_RELA section, bailing");
			return -ENOEXEC;
		}

		if (shdr.sh_type != SHT_REL) {
			continue;
		}

		if ((res = _check_section_header(fd, size, &shdr)) != 0) {
			return res;
		}

		if ((shdr.sh_size % sizeof(Elf32_Rel)) != 0) {
			LOG_ERR("l0der",
				"_run_relocations: SHT_REL section with invalid size: %ld",
				shdr.sh_size);
			return -ENOEXEC;
		}
		uint32_t reloc_count = shdr.sh_size / sizeof(Elf32_Rel);

		// Read relocations one by one.
		if ((res = epic_file_seek(fd, shdr.sh_offset, SEEK_SET)) != 0) {
			LOG_ERR("l0der",
				"_run_relocations: seek to first relocation (at 0x%lx) failed",
				shdr.sh_offset);
			return res;
		}

		for (int j = 0; j < reloc_count; j++) {
			if ((res = epic_file_read(
				     fd, &rel, sizeof(Elf32_Rel))) !=
			    sizeof(Elf32_Rel)) {
				LOG_ERR("l0der",
					"_run_relocations: relocation read failed: %d",
					res);
				return res;
			}

			uint32_t sym = ELF32_R_SYM(rel.r_info);
			uint8_t type = ELF32_R_TYPE(rel.r_info);

			// Skip relocations that are for weak symbols.
			// (ie., do not resolve relocation - they default to a safe NULL)
			uint8_t skip = 0;
			if (sym != 0) {
				for (int k = 0; k < li->weak_symbol_count;
				     k++) {
					if (li->weak_symbols[k] == sym) {
						skip = 1;
						break;
					}
				}
			}
			if (skip) {
				continue;
			}

			switch (type) {
			case R_ARM_RELATIVE:
				// Relocate.
				if ((rel.r_offset % 4) != 0) {
					LOG_ERR("l0der",
						"_run_relocations: R_ARM_RELATIVE address must be 4-byte aligned");
					return -ENOEXEC;
				}
				volatile uint32_t *addr =
					(uint32_t
						 *)(rel.r_offset + li->load_address);
				if ((uint32_t)addr < li->image_load_start ||
				    (uint32_t)addr >= li->image_load_limit) {
					LOG_ERR("l0der",
						"_run_relocations: R_ARM_RELATIVE address (%08lx) is outside image boundaries",
						(uint32_t)addr);
					return -ENOEXEC;
				}

				*addr += li->load_address;
				break;
			default:
				LOG_ERR("l0der",
					"_run_relocations: unsupported relocation type %d",
					type);
				return -ENOEXEC;
			}
		}
	}

	return 0;
}

/*
 * Load a l0dable PIE binary.
 */
static int
_load_pie(int fd, int size, Elf32_Ehdr *hdr, struct l0dable_info *info)
{
	int res;
	struct _pie_load_info li = { 0 };

	// First pass over program headers: sanity check sizes, calculate image
	// size bounds, check alignment.

	li.image_start = 0xffffffff;
	li.image_limit = 0x0;

	Elf32_Phdr phdr;

	int status_interp = -1;

	for (int i = 0; i < hdr->e_phnum; i++) {
		uint32_t phdr_addr = hdr->e_phoff + (i * hdr->e_phentsize);
		if ((res = _read_program_header(fd, phdr_addr, &phdr)) != 0) {
			return res;
		}

		if ((res = _check_program_header(fd, size, &phdr)) != 0) {
			return res;
		}

		if (phdr.p_type == PT_INTERP) {
			status_interp = _check_interp(fd, &phdr);
			continue;
		}

		if (phdr.p_type == PT_LOAD) {
			// Check alignment request.
			if ((phdr.p_offset % phdr.p_align) !=
			    (phdr.p_vaddr % phdr.p_align)) {
				LOG_ERR("l0der",
					"_load_pie: phdr %d alignment too strict",
					i);
				return -ENOEXEC;
			}
			if (phdr.p_align > li.strictest_alignment) {
				li.strictest_alignment = phdr.p_align;
			}

			uint32_t mem_start = phdr.p_vaddr;
			uint32_t mem_limit = phdr.p_vaddr + phdr.p_memsz;

			// Record memory usage.
			if (mem_start < li.image_start) {
				li.image_start = mem_start;
			}
			if (mem_limit > li.image_limit) {
				li.image_limit = mem_limit;
			}
		}
	}

	if (status_interp != 0) {
		// Expected interpreter string was not found.
		LOG_ERR("l0der", "_load_pie: not a card10 l0dable");
		return -ENOEXEC;
	}

	if (li.image_limit < li.image_start) {
		// We didn't find any LOAD segment.
		LOG_ERR("l0der", "_load_pie: no loadable segments");
		return -ENOEXEC;
	}

	LOG_INFO(
		"l0der",
		"Image bounds %08lx - %08lx",
		li.image_start,
		li.image_limit
	);

	if ((res = _get_load_addr(&li)) != 0) {
		return res;
	}

	LOG_INFO("l0der", "Loading at %08lx", li.load_address);

	// Second pass through program headers: load all LOAD segments.

	for (int i = 0; i < hdr->e_phnum; i++) {
		uint32_t phdr_addr = hdr->e_phoff + (i * hdr->e_phentsize);
		if ((res = _read_program_header(fd, phdr_addr, &phdr)) != 0) {
			return res;
		}

		if (phdr.p_type != PT_LOAD) {
			continue;
		}

		if ((res = _load_segment(fd, &li, &phdr)) != 0) {
			return res;
		}
	}

	// Load dynamic symbols.
	if ((res = _parse_dynamic_symbols(fd, size, &li, hdr)) != 0) {
		return res;
	}

	// Run relocations.
	if ((res = _run_relocations(fd, size, &li, hdr)) != 0) {
		return res;
	}

	uint32_t image_entrypoint = li.load_address + hdr->e_entry;
	LOG_INFO("l0der", "Entrypoint (ISR Vector) at %08lx", image_entrypoint);

	// Setup stack
	uint32_t *isr = (uint32_t *)image_entrypoint;
	isr[0]        = li.stack_top;

	info->isr_vector = (void *)image_entrypoint;
	return 0;
}

int l0der_load_path(const char *path, struct l0dable_info *info)
{
	int fd, res;
	if ((fd = epic_file_open(path, "rb")) < 0) {
		LOG_ERR("l0der",
			"l0der_load_path: could not open ELF file %s: %d",
			path,
			fd);
		return fd;
	}

	if ((res = epic_file_seek(fd, 0, SEEK_END)) != 0) {
		return res;
	}

	int size = epic_file_tell(fd);

	if ((res = epic_file_seek(fd, 0, SEEK_SET)) != 0) {
		return res;
	}

	// Load ELF header and ensure it's somewhat sane.

	Elf32_Ehdr hdr;
	if ((res = _read_elf_header(fd, &hdr)) != 0) {
		goto done;
	}

	// Sanitize segments.

	uint32_t ph_start = hdr.e_phoff;
	uint32_t ph_limit = hdr.e_phoff + (hdr.e_phnum * hdr.e_phentsize);
	if (ph_limit < ph_start) {
		LOG_ERR("l0der",
			"l0der_load_path: invalid program header count/size: overflow");
		return -ENOEXEC;
	}
	if (ph_limit - ph_start == 0) {
		LOG_ERR("l0der", "l0der_load_path: no segments");
		return -ENOEXEC;
	}
	if (ph_limit > size) {
		LOG_ERR("l0der",
			"l0der_load_path: program header table extends past end of file");
		return -ENOEXEC;
	}
	if (hdr.e_phentsize < sizeof(Elf32_Phdr)) {
		LOG_ERR("l0der",
			"l0der_load_path: invalid program header table entry size");
		return -ENOEXEC;
	}

	// Sanitize sections.

	uint32_t sh_start = hdr.e_shoff;
	uint32_t sh_limit = hdr.e_shoff + (hdr.e_shnum + hdr.e_shentsize);
	if (sh_limit < sh_start) {
		LOG_ERR("l0der",
			"l0der_load_path: invalid section header count/size: overflow");
		return -ENOEXEC;
	}
	if (sh_limit > size) {
		LOG_ERR("l0der",
			"l0der_load_path: section header table extends past end of file");
		return -ENOEXEC;
	}
	if (hdr.e_shentsize < sizeof(Elf32_Shdr)) {
		LOG_ERR("l0der",
			"l0der_load_path: invalid section header table entry size");
		return -ENOEXEC;
	}

	// Check whether it's something that we can load.

	if (hdr.e_type == ET_DYN && hdr.e_machine == EM_ARM &&
	    hdr.e_version == EV_CURRENT) {
		LOG_INFO("l0der", "Loading PIE l0dable %s ...", path);
		res = _load_pie(fd, size, &hdr, info);
		goto done;
	} else {
		LOG_ERR("l0der",
			"l0der_load_path: %s: not an ARM PIE, cannot load.",
			path);
		res = -ENOEXEC;
		goto done;
	}

done:
	epic_file_close(fd);
	return res;
}
