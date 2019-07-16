/*
 * support routines for FatFs
 */

#include <stddef.h> //NULL
#include <stdio.h>
#include <stdbool.h>
#include <ff.h>

#include <FreeRTOS.h>
#include <semphr.h>

static const TCHAR *rcstrings =
	_T("OK\0DISK_ERR\0INT_ERR\0NOT_READY\0NO_FILE\0NO_PATH\0INVALID_NAME\0")
	_T("DENIED\0EXIST\0INVALID_OBJECT\0WRITE_PROTECTED\0INVALID_DRIVE\0")
	_T("NOT_ENABLED\0NO_FILESYSTEM\0MKFS_ABORTED\0TIMEOUT\0LOCKED\0")
	_T("NOT_ENOUGH_CORE\0TOO_MANY_OPEN_FILES\0INVALID_PARAMETER\0");

static bool mount(void);

DIR dir;
FATFS FatFs;

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
	*sobj = xSemaphoreCreateMutex();
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
	printf("%s\n", __PRETTY_FUNCTION__);
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
