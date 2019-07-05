#include <stdio.h>
#include <stdlib.h>

#include "max32665.h"
#include "uart.h"
#include "cdcacm.h"

#include "card10.h"
#include "pmic.h"
#include "leds.h"
#include "api/dispatcher.h"
#include "modules/serial.h"

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t dispatcher_task_id;

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
		printf("Resetting ...\n");
		/*
		 * Give the UART fifo time to clear.
		 * TODO: Do this properly
		 */
		for (int i = 0; i < 0x1000000; i++) {
			__asm volatile("nop");
		}
		MXC_GCR->rstr0 = MXC_F_GCR_RSTR0_SYSTEM;
	}
}

int main(void)
{
	card10_init();
	card10_diag();

	/* TODO: Move this to its own function */
	SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

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
