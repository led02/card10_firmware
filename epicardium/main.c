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

static TaskHandle_t dispatcher_task_id;

/* TODO: Move out of main.c */
void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b)
{
	leds_set(led, r, g, b);
	leds_update();
}

/*
 * This hook is called before FreeRTOS enters tickless idle.
 */
void pre_idle_sleep(TickType_t xExpectedIdleTime)
{
	if (xExpectedIdleTime > 0) {
		/*
		 * WFE because the other core should be able to notify
		 * epicardium if it wants to issue an API call.
		 */

		/*
		 * TODO: Ensure this is actually correct and does not have any
		 * race conditions.
		 */
		__asm volatile( "cpsie i" ::: "memory" );
		__asm volatile( "dsb" ::: "memory" );
		__asm volatile( "isb" );
		__asm volatile( "wfe" );
		__asm volatile( "dsb" ::: "memory" );
		__asm volatile( "isb" );
		__asm volatile( "cpsid i" ::: "memory" );
		__asm volatile( "dsb" );
		__asm volatile( "isb" );
	}
}

/*
 * This hook is called after FreeRTOS exits tickless idle.
 */
void post_idle_sleep(TickType_t xExpectedIdleTime)
{
	/* Check whether a new API call was issued. */
	if (api_dispatcher_poll_once()) {
		xTaskNotifyGive(dispatcher_task_id);
	}
}

#if 0
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
	if (xExpectedIdleTime > 0) {
		__WFE();
		if (api_dispatcher_poll()) {
			xTaskNotifyGive(dispatcher_task_id);
		}
	}
}
#endif

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
		&dispatcher_task_id
	) != pdPASS) {
		printf("Failed to create api dispatcher task!\n");
		abort();
	}

	printf("Initializing dispatcher ...\n");
	api_dispatcher_init();

	printf("Staring core1 payload ...\n");
	core1_start();

	vTaskStartScheduler();
	printf("ERROR: FreeRTOS did not start due to unknown error!\n");
}
