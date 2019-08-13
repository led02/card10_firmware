/*
 * Implementation of efs_ API functions for a FatFS specific
 * EpicFileSystem
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ff.h>
#include <diskio.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include "fs/internal.h"
#include "modules/filesystem.h"
#include "epicardium.h"
#include "card10.h"
#include "modules/log.h"

#define SSLOG_INFO(...) LOG_INFO("fatfs", __VA_ARGS__)
#define SSLOG_ERR(...) LOG_ERR("fatfs", __VA_ARGS__)

#ifndef EPIC_FAT_STATIC_SEMAPHORE
#define EPIC_FAT_STATIC_SEMAPHORE 0
#endif

/* clang-format off */
#define EPIC_FAT_MAX_OPENED           (1 << (EPIC_FAT_FD_INDEX_BITS))
#define EPIC_FAT_FD_GENERATION_BITS   (31 - (EPIC_FAT_FD_INDEX_BITS))
#define EPIC_FAT_FD_INDEX_MASK        (uint32_t)((1u << EPIC_FAT_FD_INDEX_BITS) - 1)
#define EPIC_FAT_FD_INDEX(fd)         ((uint32_t)(fd)&EPIC_FAT_FD_INDEX_MASK)
#define EPIC_FAT_FD_GENERATION(fd)    ((uint32_t)(fd) >> EPIC_FAT_FD_INDEX_BITS)
#define EPIC_FAT_FD_MAX_GENERATION    (uint32_t)((1u << EPIC_FAT_FD_GENERATION_BITS) - 1)
#define EPIC_FAT_FD(idx, gen)         (int)(((uint32_t)(gen) << EPIC_FAT_FD_INDEX_BITS) \
                                      | ((uint32_t)(idx)&EPIC_FAT_FD_INDEX_MASK))
/* clang-format on */

struct FatObject {
	uint32_t generation;
	enum epic_stat_type type;
	union {
		FIL file;
		DIR dir;
	};
};

struct EpicFileSystem {
	struct FatObject pool[EPIC_FAT_MAX_OPENED];
	uint32_t generationCount;
	bool initialized;
	FATFS FatFs;
};

// this table converts from FRESULT to POSIX errno
static const int s_libffToErrno[20];

static const char *f_get_rc_string(FRESULT rc);
static bool globalLockAccquire();
static void globalLockRelease();
static void efs_close_all(EpicFileSystem *fs);

static bool efs_get_opened(
	EpicFileSystem *fs,
	int i,
	enum epic_stat_type expected,
	struct FatObject **res,
	int *rc
);

static EpicFileSystem s_globalFileSystem;

#if (EPIC_FAT_STATIC_SEMAPHORE == 1)
static StaticSemaphore_t s_globalLockBuffer;
#endif

static SemaphoreHandle_t s_globalLock = NULL;

void fatfs_init()
{
	static volatile bool s_initCalled = false;
	//this has to be called vefore any tasks have been started!
	// ...not 100% water-tight though, since tick count might be zero even after vTaskStartScheduler
	// has been called...
	assert(xTaskGetTickCount() == configINITIAL_TICK_COUNT);
	assert(!s_initCalled);
	s_initCalled = true;

#if (EPIC_FAT_STATIC_SEMAPHORE == 1)
	s_globalLock = xSemaphoreCreateMutexStatic(&s_globalLockBuffer);
#else
	s_globalLock = xSemaphoreCreateMutex();
#endif
	s_globalFileSystem.generationCount = 1;
	fatfs_attach();
}

/*
 * NOTE about attach/detach:
 *
 * while in detach, we're calling diskio_deinitialize (a function that is
 * originally not present in libff's diskio.h), we do not need to call
 * diskio_initialize in attach, since it will implicitly be called by
 * any f_ operation, via libff's find_volume for volumes that have not
 * been mounted yet.
 *
 */
int fatfs_attach()
{
	FRESULT ff_res;
	int rc = 0;
	if (globalLockAccquire()) {
		EpicFileSystem *fs = &s_globalFileSystem;
		if (!fs->initialized) {
			ff_res = f_mount(&fs->FatFs, "/", 0);
			if (ff_res == FR_OK) {
				fs->initialized = true;
				SSLOG_INFO("FatFs mounted\n");
			} else {
				SSLOG_ERR(
					"f_mount error %s\n",
					f_get_rc_string(ff_res)
				);
				rc = -s_libffToErrno[ff_res];
			}
		}

		globalLockRelease();
	} else {
		SSLOG_ERR("Failed to lock\n");
	}
	return rc;
}

void fatfs_detach()
{
	FRESULT ff_res;
	EpicFileSystem *fs;
	if (efs_lock_global(&fs) == 0) {
		efs_close_all(fs);

		//unmount by passing NULL as fs object, will destroy our sync object via ff_del_syncobj
		ff_res = f_mount(NULL, "/", 0);
		if (ff_res != FR_OK) {
			SSLOG_ERR(
				"f_mount (unmount) error %s\n",
				f_get_rc_string(ff_res)
			);
		}

		fs->initialized = false;
		disk_deinitialize();
		SSLOG_INFO("detached\n");
		efs_unlock_global(fs);
	}
}

static const char *f_get_rc_string(FRESULT rc)
{
	static const TCHAR *rcstrings =
		_T("OK\0DISK_ERR\0INT_ERR\0NOT_READY\0NO_FILE\0NO_PATH\0INVALID_NAME\0")
		_T("DENIED\0EXIST\0INVALID_OBJECT\0WRITE_PROTECTED\0INVALID_DRIVE\0")
		_T("NOT_ENABLED\0NO_FILESYSTEM\0MKFS_ABORTED\0TIMEOUT\0LOCKED\0")
		_T("NOT_ENOUGH_CORE\0TOO_MANY_OPEN_FILES\0INVALID_PARAMETER\0");

	FRESULT i;
	const char *p = rcstrings;

	for (i = 0; i != rc && *p; i++) {
		while (*p++)
			;
	}
	return p;
}

static bool globalLockAccquire()
{
	return (int)(xSemaphoreTake(s_globalLock, FF_FS_TIMEOUT) == pdTRUE);
}

static void globalLockRelease()
{
	xSemaphoreGive(s_globalLock);
}

int efs_lock_global(EpicFileSystem **fs)
{
	*fs = NULL;
	if (!globalLockAccquire()) {
		return -EBUSY;
	}
	if (!s_globalFileSystem.initialized) {
		globalLockRelease();
		return -ENODEV;
	}
	*fs = &s_globalFileSystem;
	return 0;
}

void efs_unlock_global(EpicFileSystem *fs)
{
	(void)fs;
	globalLockRelease();
}

static bool efs_get_opened(
	EpicFileSystem *fs,
	int fd,
	enum epic_stat_type expected,
	struct FatObject **obj,
	int *rc
) {
	uint32_t index      = EPIC_FAT_FD_INDEX(fd);
	uint32_t generation = EPIC_FAT_FD_GENERATION(fd);

	*obj = NULL;
	*rc  = -EBADF;

	if (index >= EPIC_FAT_MAX_OPENED ||
	    generation >= EPIC_FAT_FD_MAX_GENERATION) {
		return false;
	}
	if (fs->pool[index].type != expected ||
	    fs->pool[index].generation != generation) {
		return false;
	}

	*obj = &fs->pool[index];
	*rc  = 0;
	return true;
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
		case 'b': //fallthrough intentional
			break;
		default:
			return false;
		}
	}
	return true;
}

int efs_open(EpicFileSystem *fs, const char *filename, const char *modeString)
{
	struct FatObject *o = NULL;
	uint32_t index, generation;
	int mode = 0;
	int res;

	//find free object to use
	for (index = 0; index < EPIC_FAT_MAX_OPENED; ++index) {
		if (fs->pool[index].type == EPICSTAT_NONE) {
			break;
		}
	}
	if (index == EPIC_FAT_MAX_OPENED) {
		return -s_libffToErrno[FR_TOO_MANY_OPEN_FILES];
	}

	generation = fs->generationCount++;
	if (generation == EPIC_FAT_FD_MAX_GENERATION) {
		fs->generationCount = 1;
	}
	o = &fs->pool[index];

	if (!parse_mode(modeString, &mode)) {
		return -EINVAL;
	}

	res = f_open(&o->file, filename, mode);
	if (res != FR_OK) {
		return -s_libffToErrno[res];
	}

	o->type       = EPICSTAT_FILE;
	o->generation = generation;

	// for 'a' mode, we must begin at the end of the file
	if ((mode & FA_OPEN_APPEND) != 0) {
		f_lseek(&o->file, f_size(&o->file));
	}

	return EPIC_FAT_FD(index, generation);
}

int efs_close(EpicFileSystem *fs, int fd)
{
	int res;
	struct FatObject *o;
	if (efs_get_opened(fs, fd, EPICSTAT_FILE, &o, &res)) {
		res = f_close(&o->file);
		if (res != FR_OK) {
			return -s_libffToErrno[res];
		}

		o->type       = EPICSTAT_NONE;
		o->generation = 0;
	}
	return res;
}

void efs_close_all(EpicFileSystem *fs)
{
	for (int i = 0; i < EPIC_FAT_MAX_OPENED; ++i) {
		switch (fs->pool[i].type) {
		case EPICSTAT_FILE:
			f_close(&fs->pool[i].file);
			break;
		case EPICSTAT_DIR:
			//NYI
			break;
		case EPICSTAT_NONE:
			break;
		}
		fs->pool[i].type       = EPICSTAT_NONE;
		fs->pool[i].generation = 0;
	}
}

int efs_read(EpicFileSystem *fs, int fd, void *buf, size_t nbytes)
{
	unsigned int nread = 0;

	int res;
	struct FatObject *o;
	if (efs_get_opened(fs, fd, EPICSTAT_FILE, &o, &res)) {
		res = f_read(&o->file, buf, nbytes, &nread);
		if (res != FR_OK) {
			return -s_libffToErrno[res];
		}
		res = (int)nread;
	}

	return res;
}

int efs_write(EpicFileSystem *fs, int fd, const void *buf, size_t nbytes)
{
	unsigned int nwritten = 0;

	int res;
	struct FatObject *o;
	if (efs_get_opened(fs, fd, EPICSTAT_FILE, &o, &res)) {
		res = f_write(&o->file, buf, nbytes, &nwritten);
		if (res != FR_OK) {
			res = -s_libffToErrno[res];
		} else {
			res = (int)nwritten;
		}
	}
	return res;
}

int efs_flush(EpicFileSystem *fs, int fd)
{
	int res = 0;
	struct FatObject *o;
	if (efs_get_opened(fs, fd, EPICSTAT_FILE, &o, &res)) {
		res = f_sync(&o->file);
		if (res != FR_OK) {
			res = -s_libffToErrno[res];
		}
	}

	return res;
}

int efs_seek(EpicFileSystem *fs, int fd, long offset, int whence)
{
	int res = 0;
	struct FatObject *o;
	if (efs_get_opened(fs, fd, EPICSTAT_FILE, &o, &res)) {
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
		res = -s_libffToErrno[res];
	}
	return res;
}

int efs_tell(EpicFileSystem *fs, int fd)
{
	int res;
	struct FatObject *o;
	if (efs_get_opened(fs, fd, EPICSTAT_FILE, &o, &res)) {
		//f_tell simply accesses fp->fptr so no errors are expected - return directly
		res = f_tell(&o->file);
	}
	return res;
}

int efs_stat(EpicFileSystem *fs, const char *filename, struct epic_stat *stat)
{
	int res = 0;
	FILINFO finfo;
	res = f_stat(filename, &finfo);
	if (res == 0) {
		if (finfo.fattrib & AM_DIR) {
			stat->type = EPICSTAT_DIR;
		} else {
			stat->type = EPICSTAT_FILE;
		}
	}
	return -s_libffToErrno[res];
}

static const int s_libffToErrno[20] = {
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
