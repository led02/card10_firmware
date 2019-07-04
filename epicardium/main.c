#include <stdio.h>
#include <stdlib.h>

#include "max32665.h"
#include "uart.h"
#include "cdcacm.h"

#include "card10.h"
#include "pmic.h"
#include "leds.h"
#include "api/dispatcher.h"
#include "serial.h"

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t dispatcher_task_id;

/* TODO: Move out of main.c */
void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b)
{
	leds_set(led, r, g, b);
	leds_update();
}

/*
 * API dispatcher task.  This task will sleep until an API call is issued and
 * then wake up to dispatch it.
 */
void vApiDispatcher(void*pvParameters)
{
	while (1) {
		if (api_dispatcher_poll()) {
			api_dispatcher_exec();
		}
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

static void pmic_button(bool falling)
{
	if (falling) {
		MXC_GCR->rstr0 = MXC_F_GCR_RSTR0_SYSTEM;
	}
}

int main(void)
{
	card10_init();
	card10_diag();

	pmic_set_button_callback(pmic_button);

	cdcacm_init();

	printf("=> Initializing tasks ...\n");

	/* Serial */
	if (xTaskCreate(
		vSerialTask,
		(const char*)"Serial",
		configMINIMAL_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY + 1,
		NULL
	) != pdPASS) {
		printf("Failed to create serial-comms task!\n");
		abort();
	}

	if (xTaskCreate(
		vApiDispatcher,
		(const char*)"API Dispatcher",
		configMINIMAL_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY  + 2,
		&dispatcher_task_id
	) != pdPASS) {
		printf("Failed to create api dispatcher task!\n");
		abort();
	}

	printf("=> Initializing dispatcher ...\n");
	api_dispatcher_init();

	printf("=> Starting core1 payload ...\n");
	core1_start();

	printf("=> Starting FreeRTOS ...\n");
	vTaskStartScheduler();
	printf("ERROR: FreeRTOS did not start due to unknown error!\n");
}
