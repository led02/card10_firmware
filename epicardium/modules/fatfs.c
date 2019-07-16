/*
 * support routines for FatFs
 */

#include <stddef.h> //NULL
#include <stdio.h>
#include <stdbool.h>
#include <ff.h>

#include <FreeRTOS.h>
#include <semphr.h>

static bool mount(void);


DIR dir;
FATFS FatFs;

static volatile struct {
	bool initiaized;
}s_state = {
	.initiaized = false,
};

bool mount()
{
    FRESULT res;
    res = f_mount(&FatFs,"/",0);
    if(res != FR_OK) {
        printf("f_mount error %d\n", res);
        return false;
    }

    res = f_opendir(&dir, "0:");
    if(res != FR_OK) {
        printf("f_opendir error %d\n", res);
        return false;
    }

    return true;
}

void fatfs_init() {
	if(mount()) {
		s_state.initiaized = true;
		printf("FatFs mounted\n");
	}
}

int ff_cre_syncobj (	/* 1:Function succeeded, 0:Could not create the sync object */
	BYTE vol,			/* Corresponding volume (logical drive number) */
	FF_SYNC_t *sobj		/* Pointer to return the created sync object */
)
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

int ff_del_syncobj (	/* 1:Function succeeded, 0:Could not delete due to an error */
	FF_SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
)
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

int ff_req_grant (	/* 1:Got a grant to access the volume, 0:Could not get a grant */
	FF_SYNC_t sobj	/* Sync object to wait */
)
{
	/* FreeRTOS */
	return (int)(xSemaphoreTake(sobj, FF_FS_TIMEOUT) == pdTRUE);
}


/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	FF_SYNC_t sobj	/* Sync object to be signaled */
)
{
	/* FreeRTOS */
	xSemaphoreGive(sobj);
}
