#include "modules/modules.h"
#include "modules/log.h"
#include "modules/filesystem.h"
#include "card10-version.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>
#include <string.h>

TaskHandle_t dispatcher_task_id;

void vBleTask(void *pvParameters);

#define BLEMAXCFGBYTES 100
int bleShallStart(void)
{
	int bleConfigFile = epic_file_open("ble.txt", "r");
	if (bleConfigFile < 0) {
		LOG_INFO(
			"startup",
			"can not open ble.txt -> BLE is not started"
		);
		epic_file_close(bleConfigFile);
		return 0;
	}

	char cfgBuf[BLEMAXCFGBYTES + 1];
	int readNum = epic_file_read(bleConfigFile, cfgBuf, BLEMAXCFGBYTES);
	epic_file_close(bleConfigFile);
	if (readNum < 0) {
		LOG_INFO(
			"startup",
			"can not read ble.txt -> BLE is not started"
		);
		return 0;
	}
	cfgBuf[readNum] = '\0';

	char bleActiveStr[]              = "active=true";
	cfgBuf[sizeof(bleActiveStr) - 1] = '\0';

	if (strcmp(cfgBuf, "active=true") != 0) {
		LOG_INFO(
			"startup",
			"ble.txt is not \"active=true\" -> BLE is not started"
		);
		return 0;
	}

	LOG_INFO("startup", "ble.txt is \"active=true\" -> BLE is starting");
	return 1;
}

int main(void)
{
	LOG_INFO("startup", "Epicardium startup ...");
	LOG_INFO("startup", "Version " CARD10_VERSION);

	LOG_INFO("startup", "Initializing hardware ...");
	hardware_early_init();

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
	if (bleShallStart()) {
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
