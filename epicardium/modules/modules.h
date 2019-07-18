#ifndef MODULES_H
#define MODULES_H

/* ---------- FAT fs ------------------------------------------------------ */
/* max no. of descriptors (file & directory) that can be open at a time */
#define EPIC_FAT_MAX_OPENED 16
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
