#include <stdio.h>
#include <stdlib.h>

#include "max32665.h"
#include "uart.h"
#include "cdcacm.h"

#include "card10.h"
#include "pmic.h"
#include "leds.h"
#include "api/dispatcher.h"
#include "modules/modules.h"
#include "modules/log.h"

#include <Heart.h>
#include "GUI_Paint.h"

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t dispatcher_task_id;

/*
 * API dispatcher task.  This task will sleep until an API call is issued and
 * then wake up to dispatch it.
 */
void vApiDispatcher(void *pvParameters)
{
	while (1) {
		if (api_dispatcher_poll()) {
			api_dispatcher_exec();
		}
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

int main(void)
{
	card10_init();
	card10_diag();

	Paint_DrawImage(Heart, 0, 0, 160, 80);
	LCD_Update();

	/* TODO: Move this to its own function */
	SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

	cdcacm_init();
	fatfs_init();

	LOG_DEBUG("startup", "Initializing tasks ...");

	/* Serial */
	if (xTaskCreate(
		    vSerialTask,
		    (const char *)"Serial",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    tskIDLE_PRIORITY + 1,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "Serial");
		abort();
	}

	/* PMIC */
	if (xTaskCreate(
		    vPmicTask,
		    (const char *)"PMIC",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    tskIDLE_PRIORITY + 1,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "PMIC");
		abort();
	}

	/* API */
	if (xTaskCreate(
		    vApiDispatcher,
		    (const char *)"API Dispatcher",
		    configMINIMAL_STACK_SIZE * 16,
		    NULL,
		    tskIDLE_PRIORITY + 2,
		    &dispatcher_task_id) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "API Dispatcher");
		abort();
	}

	LOG_INFO("startup", "Initializing dispatcher ...");
	api_dispatcher_init();

	LOG_INFO("startup", "Starting core1 payload ...");
	core1_start();

	LOG_INFO("startup", "Starting FreeRTOS ...");
	vTaskStartScheduler();

	LOG_CRIT("startup", "FreeRTOS did not start due to unknown error!");
	abort();
}
