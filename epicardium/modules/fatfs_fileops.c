#include <errno.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include <ff.h>

#include "modules.h"
#include "epicardium.h"

#define EPIC_FAT_FD_GENERATION_BITS (31 - (EPIC_FAT_FD_INDEX_BITS))
#define EPIC_FAT_MAX_OPENED (1 << (EPIC_FAT_FD_INDEX_BITS))
#define EPIC_FAT_FD_INDEX_MASK (uint32_t)((1u << EPIC_FAT_FD_INDEX_BITS) - 1)
#define EPIC_FAT_FD_INDEX(fd) ((uint32_t)(fd)&EPIC_FAT_FD_INDEX_MASK)
#define EPIC_FAT_FD_GENERATION(fd) ((uint32_t)(fd) >> EPIC_FAT_FD_INDEX_BITS)
#define EPIC_FAT_FD_MAX_GENERATION                                             \
	(uint32_t)((1u << EPIC_FAT_FD_GENERATION_BITS) - 1)
#define EPIC_FAT_FD(idx, gen)                                                  \
	(int)(((uint32_t)(gen) << EPIC_FAT_FD_INDEX_BITS) |                    \
	      ((uint32_t)(idx)&EPIC_FAT_FD_INDEX_MASK))

// this table converts from FRESULT to POSIX errno
const int fresult_to_errno_table[20] = {
	[FR_OK]                  = 0,
	[FR_DISK_ERR]            = EIO,
	[FR_INT_ERR]             = EIO,
	[FR_NOT_READY]           = EBUSY,
	[FR_NO_FILE]             = ENOENT,
	[FR_NO_PATH]             = ENOENT,
	[FR_INVALID_NAME]        = EINVAL,
	[FR_DENIED]              = EACCES,
	[FR_EXIST]               = EEXIST,
	[FR_INVALID_OBJECT]      = EINVAL,
	[FR_WRITE_PROTECTED]     = EROFS,
	[FR_INVALID_DRIVE]       = ENODEV,
	[FR_NOT_ENABLED]         = ENODEV,
	[FR_NO_FILESYSTEM]       = ENODEV,
	[FR_MKFS_ABORTED]        = EIO,
	[FR_TIMEOUT]             = EIO,
	[FR_LOCKED]              = EIO,
	[FR_NOT_ENOUGH_CORE]     = ENOMEM,
	[FR_TOO_MANY_OPEN_FILES] = EMFILE,
	[FR_INVALID_PARAMETER]   = EINVAL,
};

struct FatObject {
	uint32_t generation;
	enum epic_stat_type type;
	union {
		FIL file;
		DIR dir;
	};
};

static int
get_fat_object(int i, enum epic_stat_type expected, struct FatObject **res);

static struct FatObject s_openedObjects[EPIC_FAT_MAX_OPENED];
static uint32_t s_fatfs_generationCount = 1;

int get_fat_object(int fd, enum epic_stat_type expected, struct FatObject **res)
{
	uint32_t index      = EPIC_FAT_FD_INDEX(fd);
	uint32_t generation = EPIC_FAT_FD_GENERATION(fd);
	if (index >= EPIC_FAT_MAX_OPENED) {
		*res = NULL;
		return EBADF;
	}
	if (generation >= EPIC_FAT_FD_MAX_GENERATION) {
		*res = NULL;
		return EBADF;
	}
	if (s_openedObjects[index].type != expected) {
		*res = NULL;
		return EBADF;
	}
	if (s_openedObjects[index].generation != generation) {
		*res = NULL;
		return EBADF;
	}
	*res = &s_openedObjects[index];
	return 0;
}

/* here we're trying to mirror glibc's behaviour:
 * any combination of rwax parses but only the first of those flags wins:
 *    - rw, ra, rr all open read-only
 * a `+` at any position but the first turns into read-write
 * any other character at any position yields EINVAL
 */
static inline bool parse_mode(const char *mstring, int *mode)
{
	switch (mstring[0]) {
	case 'r':
		*mode = FA_READ;
		break;
	case 'w':
		*mode = FA_CREATE_ALWAYS | FA_WRITE;
		break;
	case 'x':
		//in constrast to FA_CREATE_ALWAYS, FA_CREATE_NEW fails for existing files
		*mode = FA_WRITE | FA_CREATE_NEW;
		break;
	case 'a':
		//in constrast to FA_CREATE_ALWAYS, FA_CREATE_NEW fails for existing files
		*mode = FA_WRITE | FA_OPEN_APPEND;
		break;
	default:
		return false;
	}
	while (*mstring) {
		switch (*mstring++) {
		case '+': //turns any of r,w,x into read&write
			*mode |= FA_READ | FA_WRITE;
			break;
		case 'r': //fallthrough intentional
		case 'w': //fallthrough intentional
		case 'a': //fallthrough intentional
		case 'x': //fallthrough intentional
			break;
		default:
			return false;
		}
	}
	return true;
}

int epic_file_open(const char *filename, const char *modeString)
{
	struct FatObject *o = NULL;
	uint32_t index, generation;
	int mode = 0;
	int res;

	//find free object to use
	for (index = 0; index < EPIC_FAT_MAX_OPENED; ++index) {
		if (s_openedObjects[index].type == EPICSTAT_NONE) {
			break;
		}
	}
	if (index == EPIC_FAT_MAX_OPENED) {
		return -fresult_to_errno_table[FR_TOO_MANY_OPEN_FILES];
	}
	generation = s_fatfs_generationCount++;
	if (generation == EPIC_FAT_FD_MAX_GENERATION) {
		s_fatfs_generationCount = 1;
	}
	o = &s_openedObjects[index];

	if (!parse_mode(modeString, &mode)) {
		return -EINVAL;
	}

	res = f_open(&o->file, filename, mode);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}
	o->type       = EPICSTAT_FILE;
	o->generation = generation;

	// for 'a' mode, we must begin at the end of the file
	if ((mode & FA_OPEN_APPEND) != 0) {
		f_lseek(&o->file, f_size(&o->file));
	}

	return EPIC_FAT_FD(index, generation);
}

int epic_file_close(int fd)
{
	int res;
	struct FatObject *o;
	res = get_fat_object(fd, EPICSTAT_FILE, &o);
	if (res) {
		return -res;
	}

	res = f_close(&o->file);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	o->type       = EPICSTAT_NONE;
	o->generation = 0;
	return 0;
}

int epic_file_read(int fd, void *buf, size_t nbytes)
{
	unsigned int nread = 0;

	int res;
	struct FatObject *o;
	res = get_fat_object(fd, EPICSTAT_FILE, &o);
	if (res) {
		return -res;
	}

	res = f_read(&o->file, buf, nbytes, &nread);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	return (int)nread;
}

int epic_file_write(int fd, const void *buf, size_t nbytes)
{
	unsigned int nwritten = 0;

	int res;
	struct FatObject *o;
	res = get_fat_object(fd, EPICSTAT_FILE, &o);
	if (res) {
		return -res;
	}
	res = f_write(&o->file, buf, nbytes, &nwritten);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	return (int)nwritten;
}

int epic_file_flush(int fd)
{
	int res;
	struct FatObject *o;
	res = get_fat_object(fd, EPICSTAT_FILE, &o);
	if (res) {
		return -res;
	}
	res = f_sync(&o->file);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	return 0;
}

int epic_file_seek(int fd, long offset, int whence)
{
	int res;
	struct FatObject *o;
	res = get_fat_object(fd, EPICSTAT_FILE, &o);
	if (res) {
		return -res;
	}
	switch (whence) {
	case SEEK_SET:
		res = f_lseek(&o->file, offset);
		break;

	case SEEK_CUR:
		res = f_lseek(&o->file, f_tell(&o->file) + offset);
		break;

	case SEEK_END:
		res = f_lseek(&o->file, f_size(&o->file) + offset);
		break;
	default:
		return -EINVAL;
	}
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	return 0;
}

int epic_file_tell(int fd)
{
	int res;
	struct FatObject *o;
	res = get_fat_object(fd, EPICSTAT_FILE, &o);
	if (res) {
		return -res;
	}
	//f_tell simply accesses fp->fptr so no errors are expected - return directly
	return f_tell(&o->file);
}

int epic_file_stat(const char *filename, struct epic_stat *stat)
{
	int res;
	FILINFO finfo;
	res = f_stat(filename, &finfo);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	if (finfo.fattrib & AM_DIR) {
		stat->type = EPICSTAT_DIR;
	} else {
		stat->type = EPICSTAT_FILE;
	}

	return 0;
}
