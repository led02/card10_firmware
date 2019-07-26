/*
 * support routines for FatFs
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ff.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include "modules.h"
#include "epicardium.h"

#ifndef EPIC_FAT_STATIC_SEMAPHORE
#define EPIC_FAT_STATIC_SEMAPHORE 0
#endif

static const TCHAR *rcstrings =
	_T("OK\0DISK_ERR\0INT_ERR\0NOT_READY\0NO_FILE\0NO_PATH\0INVALID_NAME\0")
	_T("DENIED\0EXIST\0INVALID_OBJECT\0WRITE_PROTECTED\0INVALID_DRIVE\0")
	_T("NOT_ENABLED\0NO_FILESYSTEM\0MKFS_ABORTED\0TIMEOUT\0LOCKED\0")
	_T("NOT_ENOUGH_CORE\0TOO_MANY_OPEN_FILES\0INVALID_PARAMETER\0");

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

enum FatObjectType { FO_Nil, FO_File, FO_Dir };
struct FatObject {
	enum FatObjectType type;
	union {
		FIL file;
		DIR dir;
	};
};

static bool mount(void);
static int
get_fat_object(int i, enum FatObjectType expected, struct FatObject **res);

DIR dir;
FATFS FatFs;
static struct FatObject s_openedObjects[EPIC_FAT_MAX_OPENED];

#if (EPIC_FAT_STATIC_SEMAPHORE == 1)
StaticSemaphore_t xSemaphoreBuffer;
#endif

static volatile struct {
	bool initiaized;
} s_state = {
	.initiaized = false,
};

void fatfs_init()
{
	if (mount()) {
		s_state.initiaized = true;
		printf("FatFs mounted\n");
	}
}

const char *f_get_rc_string(FRESULT rc)
{
	FRESULT i;
	const char *p = rcstrings;

	for (i = 0; i != rc && *p; i++) {
		while (*p++)
			;
	}
	return p;
}

static bool mount()
{
	FRESULT res;
	res = f_mount(&FatFs, "/", 0);
	if (res != FR_OK) {
		printf("f_mount error %s\n", f_get_rc_string(res));
		return false;
	}

	res = f_opendir(&dir, "0:");
	if (res != FR_OK) {
		printf("f_opendir error %s\n", f_get_rc_string(res));
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------*/
/* Create a Synchronization Object */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to create a new
/  synchronization object for the volume, such as semaphore and mutex.
/  When a 0 is returned, the f_mount() function fails with FR_INT_ERR.
*/

/*
 * Return value:
 *   - 1: Function succeeded
 *   - 0: Could not create the sync object
 */
int ff_cre_syncobj(BYTE vol, FF_SYNC_t *sobj)
{
#if (EPIC_FAT_STATIC_SEMAPHORE == 1)
	*sobj = xSemaphoreCreateMutexStatic(&xSemaphoreBuffer);
#else
	*sobj = xSemaphoreCreateMutex();
#endif //EPIC_FAT_STATIC_SEMAPHORE

	return (int)(*sobj != NULL);
}

/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to delete a synchronization
/  object that created with ff_cre_syncobj() function. When a 0 is returned,
/  the f_mount() function fails with FR_INT_ERR.
*/

/*
 * Return value:
 *   - 1: Function succeeded
 *   - 0: Could not delete due to an error
 */
int ff_del_syncobj(FF_SYNC_t sobj)
{
	/* FreeRTOS */
	vSemaphoreDelete(sobj);
	return 1;
}

/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a 0 is returned, the file function fails with FR_TIMEOUT.
*/

/*
 * Return value:
 *   - 1: Got a grant to access the volume
 *   - 0: Could not get a grant
 */
int ff_req_grant(FF_SYNC_t sobj)
{
	/* FreeRTOS */
	return (int)(xSemaphoreTake(sobj, FF_FS_TIMEOUT) == pdTRUE);
}

/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant(FF_SYNC_t sobj)
{
	/* FreeRTOS */
	xSemaphoreGive(sobj);
}

int get_fat_object(int i, enum FatObjectType expected, struct FatObject **res)
{
	if (i < 0 || i >= EPIC_FAT_MAX_OPENED) {
		*res = NULL;
		return EBADF;
	}
	if (s_openedObjects[i].type != expected) {
		*res = NULL;
		return EBADF;
	}
	*res = &s_openedObjects[i];
	return 0;
}

int32_t epic_file_open(const char *filename, const char *modeString)
{
	struct FatObject *o = NULL;
	const char *mode_s  = modeString;
	int i;
	int mode = 0;

	//find free object to use
	for (i = 0; i < EPIC_FAT_MAX_OPENED; ++i) {
		if (s_openedObjects[i].type == FO_Nil) {
			break;
		}
	}
	if (i == EPIC_FAT_MAX_OPENED) {
		return -fresult_to_errno_table[FR_TOO_MANY_OPEN_FILES];
	}
	o = &s_openedObjects[i];

	while (*mode_s) {
		switch (*mode_s++) {
		case 'r':
			mode |= FA_READ;
			break;
		case 'w':
			mode |= FA_WRITE | FA_CREATE_ALWAYS;
			break;
		case 'x':
			mode |= FA_WRITE | FA_CREATE_NEW;
			break;
		case 'a':
			mode |= FA_WRITE | FA_OPEN_ALWAYS;
			break;
		case '+':
			mode |= FA_READ | FA_WRITE;
			break;
		}
	}

	int res = f_open(&o->file, filename, mode);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}
	o->type = FO_File;

	// for 'a' mode, we must begin at the end of the file
	if ((mode & FA_OPEN_ALWAYS) != 0) {
		f_lseek(&o->file, f_size(&o->file));
	}

	return i;
}

int32_t epic_file_close(int32_t fd)
{
	int res;
	struct FatObject *o;
	res = get_fat_object(fd, FO_File, &o);
	if (res) {
		return -res;
	}

	res = f_close(&o->file);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	o->type = FO_Nil;
	return 0;
}

int32_t epic_file_read(int32_t fd, void *buf, uint32_t nbytes)
{
	unsigned int nread = 0;

	int res;
	struct FatObject *o;
	res = get_fat_object(fd, FO_File, &o);
	if (res) {
		return -res;
	}

	res = f_read(&o->file, buf, nbytes, &nread);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	return nread;
}

int32_t epic_file_write(int32_t fd, const void *buf, uint32_t nbytes)
{
	unsigned int nwritten = 0;

	int res;
	struct FatObject *o;
	res = get_fat_object(fd, FO_File, &o);
	if (res) {
		return -res;
	}
	res = f_write(&o->file, buf, nbytes, &nwritten);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	return nwritten;
}

int32_t epic_file_flush(int32_t fd)
{
	int res;
	struct FatObject *o;
	res = get_fat_object(fd, FO_File, &o);
	if (res) {
		return -res;
	}
	res = f_sync(&o->file);
	if (res != FR_OK) {
		return -fresult_to_errno_table[res];
	}

	return 0;
}

int32_t epic_file_stat(const char *filename, epic_stat_t *stat)
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
