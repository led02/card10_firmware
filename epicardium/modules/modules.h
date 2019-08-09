#ifndef MODULES_H
#define MODULES_H

#include <stdint.h>

/* ---------- Serial ------------------------------------------------------- */
#define SERIAL_READ_BUFFER_SIZE 128
void vSerialTask(void *pvParameters);

void serial_enqueue_char(char chr);

/* ---------- PMIC --------------------------------------------------------- */
/* In 1/10s */
#define PMIC_PRESS_SLEEP           20
#define PMIC_PRESS_POWEROFF        40
void vPmicTask(void *pvParameters);

/* ---------- BLE ---------------------------------------------------------- */
void ble_uart_write(uint8_t *pValue, uint8_t len);

// Forces an unlock of the display. Only to be used in epicardium
void disp_forcelock();
#endif /* MODULES_H */
