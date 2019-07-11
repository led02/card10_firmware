#include <stdio.h>

#include "max32665.h"
#include "gcr_regs.h"
#include "pmic.h"
#include "MAX77650-Arduino-Library.h"

#include "FreeRTOS.h"
#include "task.h"

#include "epicardium.h"
#include "modules.h"

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
	int count = 0;
	portTickType delay = portMAX_DELAY;
	pmic_task_id = xTaskGetCurrentTaskHandle();

	while (1) {
		ulTaskNotifyTake(pdTRUE, delay);

		if (count == PMIC_PRESS_SLEEP) {
			printf("pmic: Sleep\n"
			       "[[ Unimplemented ]]\n");
		}

		if (count == PMIC_PRESS_POWEROFF) {
			printf("pmic: Poweroff\n");
			MAX77650_setSFT_RST(0x2);
		}

		uint8_t int_flag = MAX77650_getINT_GLBL();

		if (int_flag & MAX77650_INT_nEN_F) {
			/* Button was pressed */
			count = 0;
			delay = portTICK_PERIOD_MS * 100;
		}
		if (int_flag & MAX77650_INT_nEN_R) {
			/* Button was pressed */
			if (count < PMIC_PRESS_SLEEP) {
				printf("pmic: Reset\n");
				/*
				 * Give the UART fifo time to clear.
				 * TODO: Do this properly
				 */
				for (int i = 0; i < 0x1000000; i++) {
					__asm volatile("nop");
				}
				MXC_GCR->rstr0 = MXC_F_GCR_RSTR0_SYSTEM;
			}

			count = 0;
			delay = portMAX_DELAY;
		}

		/* TODO: Remove when all interrupts are handled */
		if (int_flag & ~(MAX77650_INT_nEN_F | MAX77650_INT_nEN_R)) {
			printf("=====> WARNING <=====\n"
			       "* Unhandled PMIC Interrupt: %x\n",
			       int_flag);
		}

		if (delay != portMAX_DELAY) {
			count += 1;
		}
	}
}
