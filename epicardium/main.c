#include "modules/modules.h"
#include "modules/log.h"
#include "modules/filesystem.h"
#include "modules/config.h"
#include "card10-version.h"

#include "gfx.h"
#include "display.h"
#include "version-splash.h"

#include "FreeRTOS.h"
#include "task.h"
#include "mxc_delay.h"

#include <stdlib.h>
#include <string.h>

int main(void)
{
	watchdog_init();

	LOG_INFO("startup", "Epicardium startup ...");
	LOG_INFO("startup", "Version " CARD10_VERSION);

	LOG_DEBUG("startup", "Initializing hardware ...");
	hardware_early_init();

	load_config();

	/*
	 * Version Splash
	 */
	const char *version_buf = CARD10_VERSION;
	mxc_delay(500000);
	epic_disp_clear(0x0000);
	if (strcmp(CARD10_VERSION, "v1.10") == 0) {
		gfx_copy_region_raw(
			&display_screen, 0, 0, 160, 80, 2, version_splash
		);
	} else {
		const int off = (160 - (int)strlen(version_buf) * 14) / 2;
		epic_disp_print(10, 20, "Epicardium", 0xfe20, 0x0000);
		epic_disp_print(
			off > 0 ? off : 0, 40, version_buf, 0xfe20, 0x0000
		);
	}
	epic_disp_update();
	mxc_delay(2000000);

	LOG_DEBUG("startup", "Initializing tasks ...");

	/* Serial */
	if (xTaskCreate(
		    vSerialTask,
		    (const char *)"Serial",
		    configMINIMAL_STACK_SIZE * 2,
		    NULL,
		    tskIDLE_PRIORITY + 3,
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

	/* BHI160 */
	if (xTaskCreate(
		    vBhi160Task,
		    (const char *)"BHI160 Driver",
		    configMINIMAL_STACK_SIZE * 2,
		    NULL,
		    tskIDLE_PRIORITY + 1,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "BHI160");
		abort();
	}

	/* MAX30001 */
	if (xTaskCreate(
		    vMAX30001Task,
		    (const char *)"MAX30001 Driver",
		    configMINIMAL_STACK_SIZE * 2,
		    NULL,
		    tskIDLE_PRIORITY + 1,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "MAX30001");
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
	if (ble_shall_start()) {
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
	}

	/* LEDs */
	if (xTaskCreate(
		    vLedTask,
		    (const char *)"LED",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    tskIDLE_PRIORITY + 1,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "LED");
		abort();
	}

	/* Lifecycle */
	if (xTaskCreate(
		    vLifecycleTask,
		    (const char *)"Lifecycle",
		    configMINIMAL_STACK_SIZE * 4,
		    NULL,
		    tskIDLE_PRIORITY + 3,
		    NULL) != pdPASS) {
		LOG_CRIT("startup", "Failed to create %s task!", "Lifecycle");
		abort();
	}

	/*
	 * Initialize serial driver data structures.
	 */
	serial_init();

	LOG_DEBUG("startup", "Starting FreeRTOS ...");
	vTaskStartScheduler();

	LOG_CRIT("startup", "FreeRTOS did not start due to unknown error!");
	abort();
}
