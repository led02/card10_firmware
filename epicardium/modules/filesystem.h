#ifndef EPICARDIUM_MODULE_FILESYSTEM_INCLUDED
#define EPICARDIUM_MODULE_FILESYSTEM_INCLUDED

/* ---------- FAT fs ------------------------------------------------------ */

#include <stdbool.h>
#include "epicardium.h"

/**
 * module initialization - to be called once at startup before any FreeRTOS tasks
 * have been started
 *
 * calls fatfs_attach
 */
void fatfs_init(void);

/**
 * initialize and mount the FLASH storage
 * 
 * NOTE: not safe to be called from an ISR
 */
int fatfs_attach(void);


/**
 * asynchronously attach the FLASH storage
 * 
 * safe to be called from an ISR
 */
void fatfs_schedule_attach(void);

/** close all opened FDs, sync and deinitialize FLASH layer */
void fatfs_detach(void);

/** close all onpened FDs 
 * TODO: add ability to close FDs opened by core0/core1 only
 */
#define EPICARDIUM_COREMASK_0		0x01
#define EPICARDIUM_COREMASK_1		0x02
#define EPICARDIUM_COREMASK_BOTH	0x03

void fatfs_close_all(int coreMask);

#endif//EPICARDIUM_MODULE_FILESYSTEM_INCLUDED
