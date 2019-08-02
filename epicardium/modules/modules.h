#ifndef MODULES_H
#define MODULES_H

/* ---------- FAT fs ------------------------------------------------------ */
/* Number of bits to use for indexing into our internal pool of files/directories
 * This indirectly specifies the size of the pool as 1^EPIC_FAT_FD_INDEX_BITS
 * Increase if number of open file descriptors is not enough, but be aware of
 * memory usage of the pool!
 */
#define EPIC_FAT_FD_INDEX_BITS 8
#define EPIC_FAT_STATIC_SEMAPHORE 1
void fatfs_init(void);

/* ---------- Serial ------------------------------------------------------- */
#define SERIAL_READ_BUFFER_SIZE 128
void vSerialTask(void *pvParameters);

/* ---------- PMIC --------------------------------------------------------- */
/* In 1/10s */
#define PMIC_PRESS_SLEEP           20
#define PMIC_PRESS_POWEROFF        40
void vPmicTask(void *pvParameters);

// Forces an unlock of the display. Only to be used in epicardium
void disp_forcelock();

#endif /* MODULES_H */
