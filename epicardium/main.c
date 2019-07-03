#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "cdcacm.h"

#include "card10.h"
#include "leds.h"
#include "api/dispatcher.h"
#include "serial.h"

#include "FreeRTOS.h"
#include "task.h"

void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b)
{
	leds_set(led, r, g, b);
	leds_update();
}

void vApiDispatcher(void*pvParameters)
{
	while (1) {
		api_dispatcher_poll();
		vTaskDelay(portTICK_PERIOD_MS * 10);
	}
}

void vApplicationGetIdleTaskMemory(
	StaticTask_t**ppxIdleTaskTCBBuffer,
	StackType_t**ppxIdleTaskStackBuffer,
	uint32_t *pulIdleTaskStackSize)
{
	/*
	 * If the buffers to be provided to the Idle task are declared inside this
	 * function then they must be declared static - otherwise they will be allocated on
	 * the stack and so not exists after this function exits.
	 */
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/*
	 * Pass out a pointer to the StaticTask_t structure in which the Idle task's
	 * ktate will be stored.
	 */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/*
	 * Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	 * Note that, as the array is necessarily of type StackType_t,
	 * configMINIMAL_STACK_SIZE is specified in words, not bytes.
	 */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

int main(void)
{
	card10_init();
	card10_diag();

	cdcacm_init();

	printf("Initializing tasks ...\n");

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
		NULL
	) != pdPASS) {
		printf("Failed to create api dispatcher task!\n");
		abort();
	}

	printf("Initializing dispatcher ...\n");
	api_dispatcher_init();

	printf("Staring core1 payload ...\n");
	core1_start();

	vTaskStartScheduler();
	printf("ERROR: FreeRTOS did not start due to above error!\n");

#if 0
	while(1) {
		__WFE();
		api_dispatcher_poll();
	}
#endif
}
