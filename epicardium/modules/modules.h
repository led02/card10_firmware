#ifndef MODULES_H
#define MODULES_H

#include "FreeRTOS.h"
#include "semphr.h"

#include <stdint.h>

/* ---------- Dispatcher --------------------------------------------------- */
void vApiDispatcher(void *pvParameters);
extern SemaphoreHandle_t api_mutex;
extern TaskHandle_t dispatcher_task_id;

/* ---------- Hardware Init & Reset ---------------------------------------- */
int hardware_early_init(void);
int hardware_init(void);
int hardware_reset(void);

/* ---------- Lifecycle ---------------------------------------------------- */
void vLifecycleTask(void *pvParameters);
void return_to_menu(void);

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

/* ---------- Display ------------------------------------------------------ */
/* Forces an unlock of the display. Only to be used in Epicardium */
void disp_forcelock();

#endif /* MODULES_H */
