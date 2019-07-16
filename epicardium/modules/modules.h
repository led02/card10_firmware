#ifndef MODULES_H
#define MODULES_H

/* FatFS */
void fatfs_init(void);

/* ---------- Serial ------------------------------------------------------- */
#define SERIAL_READ_BUFFER_SIZE 128
void vSerialTask(void *pvParameters);

/* ---------- PMIC --------------------------------------------------------- */
/* In 1/10s */
#define PMIC_PRESS_SLEEP           20
#define PMIC_PRESS_POWEROFF        40
void vPmicTask(void *pvParameters);

#endif /* MODULES_H */

