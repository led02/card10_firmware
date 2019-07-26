#pragma once

/*
 * l0der, the l0dable loader.
 *
 * l0der is the ELF loader responsible for retrieving a l0dable from FAT and
 * into memory for core1 to execute.
 *
 * l0dables are PIE ELF binaries. They can be loaded anywhere into memory,
 * although for now we load them at a static address (but that might change
 * with address space evolution and/or multi-app / resident app support.
 *
 */

struct l0dable_info {
	/** The address of the entry ISR vector. */
	void *isr_vector;
};

/**
 * Load a l0dable into memory.
 *
 * :param const char *path: Path of l0dable on FAT filesystem.
 * :param l0dable_info l0dable: Information about loaded l0dable.
 * :returns: ``0`` on success or a negative value on error.  Possible errors:
 *
 *    - ``-ENOENT``: l0dable not present at given path.
 *    - ``-EIO``: Read failed: l0dable corrupted or truncated.
 *    - ``-ENOEXEC``: Corrupted/invalid l0dable.
 *    - ``-ENOMEM``: l0dable too large to fit in RAM.
 */
int l0der_load_path(const char *path, struct l0dable_info *l0dable);
