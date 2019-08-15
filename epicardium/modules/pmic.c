#include <stdio.h>

#include "max32665.h"
#include "gcr_regs.h"
#include "pmic.h"
#include "MAX77650-Arduino-Library.h"
#include "card10.h"

#include "FreeRTOS.h"
#include "task.h"

#include "epicardium.h"
#include "modules.h"
#include "modules/log.h"

/* Task ID for the pmic handler */
static TaskHandle_t pmic_task_id = NULL;

void pmic_interrupt_callback(void *_)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (pmic_task_id != NULL) {
		vTaskNotifyGiveFromISR(pmic_task_id, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void vPmicTask(void *pvParameters)
{
	pmic_task_id = xTaskGetCurrentTaskHandle();

	TickType_t button_start_tick = 0;

	while (1) {
		if (button_start_tick == 0) {
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		} else {
			ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100));
		}

		TickType_t duration = xTaskGetTickCount() - button_start_tick;

		if (button_start_tick != 0 && duration > pdMS_TO_TICKS(1000)) {
			LOG_WARN("pmic", "Poweroff");
			MAX77650_setSFT_RST(0x2);
		}

		uint8_t int_flag = MAX77650_getINT_GLBL();

		if (int_flag & MAX77650_INT_nEN_F) {
			/* Button was pressed */
			button_start_tick = xTaskGetTickCount();
		}
		if (int_flag & MAX77650_INT_nEN_R) {
			/* Button was released */
			button_start_tick = 0;
			if (duration < pdMS_TO_TICKS(400)) {
				return_to_menu();
			} else {
				LOG_WARN("pmic", "Resetting ...");
				card10_reset();
			}
		}

		/* TODO: Remove when all interrupts are handled */
		if (int_flag & ~(MAX77650_INT_nEN_F | MAX77650_INT_nEN_R)) {
			LOG_WARN(
				"pmic",
				"Unhandled PMIC Interrupt: %x",
				int_flag
			);
		}
	}
}
