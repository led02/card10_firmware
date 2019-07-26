#include "l0der/l0der.h"

#include <stdio.h>
#include <string.h>
#include <ff.h>

#include "epicardium.h"
#include "l0der/elf.h"
#include "modules/log.h"

/*
 * Read an ELF header, check E_IDENT.
 */
static int _read_elf_header(FIL *fp, Elf32_Ehdr *hdr)
{
	f_lseek(fp, 0);

	unsigned int read;
	FRESULT fres = f_read(fp, hdr, sizeof(Elf32_Ehdr), &read);
	if (fres != FR_OK) {
		LOG_ERR("l0der", "_read_elf_header: f_read failed: %d", fres);
		return -1;
	}

	if (read != sizeof(Elf32_Ehdr)) {
		LOG_ERR("l0der", "_read_elf_header: file truncated");
		return -1;
	}

	if (hdr->e_ident[0] != ELFMAG0 ||
		hdr->e_ident[1] != ELFMAG1 ||
		hdr->e_ident[2] != ELFMAG2 ||
		hdr->e_ident[3] != ELFMAG3) {
		LOG_ERR("l0der", "_read_elf_header: not an ELF file");
		return -1;
	}

	if (hdr->e_ident[4] != ELFCLASS32) {
		LOG_ERR("l0der", "_read_elf_header: not a 32-bit ELF");
		return -1;
	}

	if (hdr->e_ident[5] != ELFDATA2LSB) {
		LOG_ERR("l0der", "_read_elf_header: not a little-endian ELF");
		return -1;
	}

	if (hdr->e_ident[6] != EV_CURRENT) {
		LOG_ERR("l0der", "_read_elf_header: not a v1 ELF");
		return -1;
	}

	if (hdr->e_ehsize < sizeof(Elf32_Ehdr)) {
		LOG_ERR("l0der", "_raed_elf_header: header too small");
		return -1;
	}

	return 0;
}

/*
 * Read an ELF program header header.
 */
static int _read_program_header(FIL *fp, uint32_t phdr_addr, Elf32_Phdr *phdr)
{
	FRESULT fres;

	if ((fres = f_lseek(fp, phdr_addr)) != FR_OK) {
		LOG_ERR("l0der", "_read_program_header: could not seek to 0x%lx: %d", phdr_addr, fres);
		return -EIO;
	}

	unsigned int read;
	if ((fres = f_read(fp, phdr, sizeof(Elf32_Phdr), &read)) != FR_OK || read < sizeof(Elf32_Phdr)) {
		LOG_ERR("l0der", "_read_program_header: could not read phdr: %d", fres);
		return -EIO;
	}

	return 0;
}

/*
 * Read an ELF program header header.
 */
static int _read_section_header(FIL *fp, uint32_t shdr_addr, Elf32_Shdr *shdr)
{
	FRESULT fres;

	if ((fres = f_lseek(fp, shdr_addr)) != FR_OK) {
		LOG_ERR("l0der", "_read_section_header: could not seek to 0x%lx: %d", shdr_addr, fres);
		return -EIO;
	}

	unsigned int read;
	if ((fres = f_read(fp, shdr, sizeof(Elf32_Shdr), &read)) != FR_OK || read < sizeof(Elf32_Shdr)) {
		LOG_ERR("l0der", "_read_section_header: could not read shdr (0x%x bytes) at %08lx: %d, got 0x%x bytes",
				sizeof(Elf32_Shdr), shdr_addr, fres, read);
		return -EIO;
	}

	return 0;
}

/*
 * Check an ELF program header.
 *
 * This function ensures basic memory sanity of a program header / segment.
 * It ensures that it points to a file region that is contained within the file fully.
 */
static int _check_program_header(FIL *fp, Elf32_Phdr *phdr) {
	size_t size = f_size(fp);

	// Check file size/offset.
	uint32_t file_start = phdr->p_offset;
	uint32_t file_limit = phdr->p_offset + phdr->p_filesz;
	if (file_limit < file_start) {
		LOG_ERR("l0der", "_check_program_header: file size overflow");
		return -ENOEXEC;
	}
	if (file_limit > size) {
		LOG_ERR("l0der", "_check_program_header: extends past end of file");
		return -ENOEXEC;
	}

	if (phdr->p_type == PT_LOAD) {	
		// Check mem/file size.
		if (phdr->p_filesz > phdr->p_memsz) {
			LOG_ERR("l0der", "_check_program_header: file size larger than memory size");
			return -ENOEXEC;
		}

		uint32_t mem_start = phdr->p_vaddr;
		uint32_t mem_limit = phdr->p_vaddr + phdr->p_memsz;

		if (mem_limit < mem_start) {
			LOG_ERR("l0der", "_check_program_header: mem size overflow");
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
static int _check_section_header(FIL *fp, Elf32_Shdr *shdr) {
	size_t size = f_size(fp);

	// Check file size/offset.
	uint32_t file_start = shdr->sh_offset;
	uint32_t file_limit = shdr->sh_offset + shdr->sh_size;
	if (file_limit < file_start) {
		LOG_ERR("l0der", "_check_section_header: file size overflow");
		return -ENOEXEC;
	}
	if (file_limit > size) {
		LOG_ERR("l0der", "_check_section_header: extends past end of file");
		return -ENOEXEC;
	}

	return 0;
}

static const char *_interpreter = "card10-l0dable";

static int _check_interp(FIL *fp, Elf32_Phdr *phdr)
{
	uint32_t buffer_size = 64;
	char interp[buffer_size];
	memset(interp, 0, buffer_size);

	if (phdr->p_filesz > buffer_size) {
		LOG_ERR("l0der", "_check_interp: interpreter size too large");
		return -1;
	}

	FRESULT fres;
	if ((fres = f_lseek(fp, phdr->p_offset)) != FR_OK) {
		LOG_ERR("l0der", "_check_interp: could not seek to 0x%lx: %d", phdr->p_offset, fres);
		return -1;
	}

	unsigned int read; // unused (we don't care if the read gets truncated)
	if ((fres = f_read(fp, interp, buffer_size, &read)) != FR_OK) {
		LOG_ERR("l0der", "_check_interp: could not read segment %d", fres);
		return -1;
	}

	if (strncmp(interp, _interpreter, strlen(_interpreter)) != 0) {
		LOG_ERR("l0der", "_check_interp: invalid interpreter, want card10-l0dable");
		return -1;
	}

	return 0;
}

static int _get_load_addr(uint32_t image_start, uint32_t image_limit, void **load)
{
	uint32_t image_size = image_limit - image_start;

	// ref: Documentation/memorymap.rst
	uint32_t core1_mem_start = 0x20040000;
	uint32_t core1_mem_limit = 0x20080000;
	uint32_t core1_mem_size = core1_mem_limit - core1_mem_start;

	if (image_size > core1_mem_size) {
		LOG_ERR("l0der", "_get_load_addr: image too large (need 0x%08lx bytes, have %08lx",
				image_size, core1_mem_size);
		return -ENOMEM;
	}

	*load = (void *)core1_mem_start;

	return 0;
}

static int _load_segment(FIL *fp, void *image_load_addr, Elf32_Phdr *phdr)
{
	uint32_t segment_start = (uint32_t)image_load_addr + phdr->p_vaddr;
	uint32_t segment_limit = segment_start + phdr->p_memsz;

	LOG_INFO("l0der", "Segment %08lx-%08lx: 0x%lx bytes from file",
			segment_start, segment_limit, phdr->p_filesz);
	memset((void *)segment_start, 0, phdr->p_memsz);

	FRESULT fres;
	unsigned int read;

	if ((fres = f_lseek(fp, phdr->p_offset)) != FR_OK) {
		LOG_ERR("l0der", "_load_segment: seek failed: %d", fres);
		return -EIO;
	}

	if ((fres = f_read(fp, (void *)segment_start, phdr->p_filesz, &read)) != FR_OK || read != phdr->p_filesz) {
		LOG_ERR("l0der", "_load_segment: read failed");
		return -EIO;
	}

	return 0;
}

static int _run_relocations(FIL *fp, void *load_addr, Elf32_Ehdr *hdr) {
	int res;
	FRESULT fres;
	Elf32_Shdr shdr;
	Elf32_Rel rel;

	// Go through all relocation sections.
	for (int i = 0; i < hdr->e_shnum; i++) {
		uint32_t shdr_addr = hdr->e_shoff + (i * hdr->e_shentsize);
		if ((res = _read_section_header(fp, shdr_addr, &shdr)) != 0) {
			return res;
		}

		// We don't support RELA (relocation with addend) sections (yet?).
		if (shdr.sh_type == SHT_RELA) {
			LOG_ERR("l0der", "_run_relocations: found unsupported SHT_RELA section, bailing");
			return -ENOEXEC;
		}

		if (shdr.sh_type != SHT_REL) {
			continue;
		}

		if ((res = _check_section_header(fp, &shdr)) != 0) {
			return res;
		}

		if ((shdr.sh_size % sizeof(Elf32_Rel)) != 0) {
			LOG_ERR("l0der", "_run_relocations: SHT_REL section with invalid size: %ld", shdr.sh_size);
			return -EIO;
		}
		uint32_t reloc_count = shdr.sh_size / sizeof(Elf32_Rel);

		// Read relocations one by one.
		if ((fres = f_lseek(fp, shdr.sh_offset)) != FR_OK) {
			LOG_ERR("l0der", "_run_relocations: seek to first relocation (at 0x%lx) failed", shdr.sh_offset);
			return -EIO;
		}

		for (int j = 0; j < reloc_count; j++) {
			unsigned int read;
			if ((fres = f_read(fp, &rel, sizeof(Elf32_Rel), &read)) != FR_OK || read != sizeof(Elf32_Rel)) {
				LOG_ERR("l0der", "_run_relocations: relocation read failed: %d", fres);
				return -EIO;
			}

			uint8_t type = ELF32_R_TYPE(rel.r_info);
			switch (type) {
				case R_ARM_RELATIVE:
					if ((rel.r_offset % 4) != 0) {
						LOG_ERR("l0der", "_run_relocations: R_ARM_RELATIVE address must be 4-byte aligned");
						return -ENOEXEC;
					}
					// TODO(q3k): check whether offset is contained in binary.
					volatile uint32_t *addr = (uint32_t *)(rel.r_offset + load_addr);
					*addr += (uint32_t)load_addr;
					break;
				default:
					LOG_ERR("l0der", "_run_relocations: unsupported relocation type %d", type);
					return -ENOEXEC;
			}
		}
	}

	return 0;
}

static int _load_pie(FIL *fp, Elf32_Ehdr *hdr, struct l0dable_info *info)
{
	int res;

	// First pass over program headers: sanity check sizes and calculate
	// memory image bounds. l0der currently only supports loading the image into
	// the core1 address space, that is from 0x1008_0000 to 0x1010_0000. Thus,
	// we need to ensure that all the LOADable segments can fit within this
	// range.
	
	uint32_t image_start = 0xFFFFFFFF;
	uint32_t image_limit = 0x0;

	Elf32_Phdr phdr;

	int status_interp = -1;

	for (int i = 0; i < hdr->e_phnum; i++) {
		uint32_t phdr_addr = hdr->e_phoff + (i * hdr->e_phentsize);
		if ((res = _read_program_header(fp, phdr_addr, &phdr)) != 0) {
			return res;
		}

		if ((res = _check_program_header(fp, &phdr)) != 0) {
			return res;
		}

		if (phdr.p_type == PT_INTERP) {
			status_interp = _check_interp(fp, &phdr);
			continue;
		}

		if (phdr.p_type == PT_LOAD) {
			// Check alignment request.
			if (phdr.p_align > 4) {
				LOG_ERR("l0der", "_load_pie: phdr %d alignment too strict", i);
				return -ENOEXEC;
			}

			uint32_t mem_start = phdr.p_vaddr;
			uint32_t mem_limit = phdr.p_vaddr + phdr.p_memsz;

			// Record memory usage.
			if (mem_start < image_start) {
				image_start = mem_start;
			}
			if (mem_limit > image_limit) {
				image_limit = mem_limit;
			}
		}
	}

	if (status_interp != 0) {
		LOG_ERR("l0der", "_load_pie: not a card10 l0dable");
		return -ENOEXEC;
	}


	if (image_limit < image_start) {
		LOG_ERR("l0der", "_load_pie: no loadable segments");
		return -ENOEXEC;
	}

	LOG_INFO("l0der", "Image bounds %08lx - %08lx", image_start, image_limit);

	void *load_addr;
	if ((res = _get_load_addr(image_start, image_limit, &load_addr)) != 0) {
		return res;
	}

	LOG_INFO("l0der", "Loading at %08lx", (uint32_t)load_addr);

	// Second pass through program headers: load all LOAD segments.
	
	for (int i = 0; i < hdr->e_phnum; i++) {
		uint32_t phdr_addr = hdr->e_phoff + (i * hdr->e_phentsize);
		if ((res = _read_program_header(fp, phdr_addr, &phdr)) != 0) {
			return res;
		}

		if (phdr.p_type != PT_LOAD) {
			continue;
		}

		if ((res = _load_segment(fp, load_addr, &phdr)) != 0) {
			return res;
		}
	}

	// Run relocations.
	
	if ((res = _run_relocations(fp, load_addr, hdr)) != 0) {
		return res;
	}

	uint32_t image_entrypoint = (uint32_t)load_addr + hdr->e_entry;
	LOG_INFO("l0der", "Entrypoint (ISR Vector) at %08lx", image_entrypoint);

	info->isr_vector = (void *)image_entrypoint;

	return 0;
}

int l0der_load_path(const char *path, struct l0dable_info *info)
{
	FIL fh;

	FRESULT fres = f_open(&fh, path, FA_OPEN_EXISTING|FA_READ);
	if (fres != FR_OK) {
		LOG_ERR("l0der", "l0der_load_path: could not open ELF file %s: %d", path, fres);
		return -ENOENT;
	}

	int size = f_size(&fh);

	int res = 0;

	// Load ELF header and ensure it's somewhat sane.

	Elf32_Ehdr hdr;
	if (_read_elf_header(&fh, &hdr) != 0) {
		res = -EINVAL;
		goto done;
	}

	// Sanitize segments.

	uint32_t ph_start = hdr.e_phoff;
	uint32_t ph_limit = hdr.e_phoff + (hdr.e_phnum * hdr.e_phentsize);
	if (ph_limit < ph_start) {
		LOG_ERR("l0der", "l0der_load_path: invalid program header count/size: overflow");
		return -ENOEXEC;
	}
	if (ph_limit - ph_start == 0) {
		LOG_ERR("l0der", "l0der_load_path: no segments");
		return -ENOEXEC;
	}
	if (ph_limit > size) {
		LOG_ERR("l0der", "l0der_load_path: program header table extends past end of file");
		return -ENOEXEC;
	}
	if (hdr.e_phentsize < sizeof(Elf32_Phdr)) {
		LOG_ERR("l0der", "l0der_load_path: invalid program header table entry size");
		return -ENOEXEC;
	}

	// Sanitize sections.
	
	uint32_t sh_start = hdr.e_shoff;
	uint32_t sh_limit = hdr.e_shoff + (hdr.e_shnum + hdr.e_shentsize);
	if (sh_limit < sh_start) {
		LOG_ERR("l0der", "l0der_load_path: invalid section header count/size: overflow");
		return -ENOEXEC;
	}
	if (sh_limit > size) {
		LOG_ERR("l0der", "l0der_load_path: section header table extends past end of file");
		return -ENOEXEC;
	}
	if (hdr.e_shentsize < sizeof(Elf32_Shdr)) {
		LOG_ERR("l0der", "l0der_load_path: invalid section header table entry size");
		return -ENOEXEC;
	}

	// Check whether it's something that we can load.

	if (hdr.e_type == ET_DYN && hdr.e_machine == EM_ARM && hdr.e_version == EV_CURRENT) {
		LOG_INFO("l0der", "Loading PIE l0dable %s ...", path);
		res = _load_pie(&fh, &hdr, info);
		goto done;
	} else {
		LOG_ERR("l0der", "l0der_load_path: %s: not an ARM PIE, cannot load.", path);
		res = -ENOEXEC;
		goto done;
	}

done:
	f_close(&fh);
	return res;
}
