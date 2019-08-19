#include <stdio.h>
#include <stdlib.h>

#include <ff.h>

#include "max32665.h"
#include "uart.h"
#include "cdcacm.h"
#include "gpio.h"

#include "card10.h"
#include "pmic.h"
#include "leds.h"
#include "api/dispatcher.h"
#include "modules/modules.h"
#include "modules/log.h"
#include "modules/stream.h"
#include "modules/filesystem.h"
#include "api/interrupt-sender.h"

#include "gfx.h"
#include "display.h"
#include "card10-version.h"

#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t dispatcher_task_id;

void vBleTask(void *pvParameters);

int main(void)
{
	LOG_INFO("startup", "Epicardium startup ...");
	LOG_INFO("startup", "Version " CARD10_VERSION);

	hardware_early_init();

#ifdef CARD10_DEBUG_CORE1
	LOG_WARN("startup", "Core 1 Debugger Mode");
	static const gpio_cfg_t swclk = {
		PORT_0,
		PIN_7,
		GPIO_FUNC_ALT3,
		GPIO_PAD_NONE,
	};
	static const gpio_cfg_t swdio = {
		PORT_0,
		PIN_6,
		GPIO_FUNC_ALT3,
		GPIO_PAD_NONE,
	};

	GPIO_Config(&swclk);
	GPIO_Config(&swdio);
#endif /* CARD10_DEBUG_CORE1 */

	/* TODO: Move this to its own function */
	SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

	if (cdcacm_init() < 0) {
		LOG_ERR("startup", "USB-Serial unavailable");
	}

	fatfs_init();
	api_interrupt_init();
	stream_init();

	LOG_INFO("startup", "Initializing dispatcher ...");
	api_dispatcher_init();

	LOG_INFO("startup", "Initializing tasks ...");

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
		    tskIDLE_PRIORITY + 4,
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
		LOG_CRIT(
			"startup",
			"Failed to create %s task!",
			"API Dispatcher"
		);
		abort();
	}

	/* BLE */
	if (xTaskCreate(
		    vBleTask,
		    (const char *)"BLE",
		    configMINIMAL_STACK_SIZE * 10,
		    NULL,
		    tskIDLE_PRIORITY + 1,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "BLE");
		abort();
	}

	/* light sensor */
	LOG_INFO("startup", "starting light sensor ...");
	epic_light_sensor_run();

	/* Lifecycle */
	if (xTaskCreate(
		    vLifecycleTask,
		    (const char *)"Lifecycle",
		    configMINIMAL_STACK_SIZE * 4,
		    NULL,
		    tskIDLE_PRIORITY + 1,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "Lifecycle");
		abort();
	}

	LOG_INFO("startup", "Starting FreeRTOS ...");
	vTaskStartScheduler();

	LOG_CRIT("startup", "FreeRTOS did not start due to unknown error!");
	abort();
}
