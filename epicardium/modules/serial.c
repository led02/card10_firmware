#include <stdint.h>
#include <stdio.h>

#include "max32665.h"
#include "cdcacm.h"
#include "uart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "modules.h"
#include "modules/log.h"
#include "api/interrupt-sender.h"

/* Task ID for the serial handler */
TaskHandle_t serial_task_id = NULL;

/* The serial console in use (UART0) */
extern mxc_uart_regs_t *ConsoleUart;
/* Read queue, filled by both UART and CDCACM */
static QueueHandle_t read_queue;

/*
 * API-call to write a string.  Output goes to both CDCACM and UART
 */
void epic_uart_write_str(const char *str, intptr_t length)
{
	UART_Write(ConsoleUart, (uint8_t *)str, length);
	cdcacm_write((uint8_t *)str, length);
}

/*
 * Blocking API-call to read a character from the queue.
 */
char epic_uart_read_chr(void)
{
	char chr;
	xQueueReceive(read_queue, &chr, portMAX_DELAY);
	return chr;
}

/* Interrupt handler needed for SDK UART implementation */
void UART0_IRQHandler(void)
{
	UART_Handler(ConsoleUart);
}

static void uart_callback(uart_req_t *req, int error)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(serial_task_id, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void enqueue_char(char chr)
{
	if (chr == 0x3) {
		/* Control-C */
		api_interrupt_trigger(API_INT_CTRL_C);
	}

	if (chr == 0x0e) {
		/* Control-N */
		api_interrupt_trigger(API_INT_BHI160);
	}

	if (xQueueSend(read_queue, &chr, 100) == errQUEUE_FULL) {
		/* Queue overran, wait a bit */
		vTaskDelay(portTICK_PERIOD_MS * 50);
	}
}

void vSerialTask(void *pvParameters)
{
	static uint8_t buffer[sizeof(char) * SERIAL_READ_BUFFER_SIZE];
	static StaticQueue_t read_queue_data;

	serial_task_id = xTaskGetCurrentTaskHandle();

	/* Setup read queue */
	read_queue = xQueueCreateStatic(
		SERIAL_READ_BUFFER_SIZE, sizeof(char), buffer, &read_queue_data
	);

	/* Setup UART interrupt */
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_DisableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn, 6);
	NVIC_EnableIRQ(UART0_IRQn);

	unsigned char data;
	uart_req_t read_req = {
		.data     = &data,
		.len      = 1,
		.callback = uart_callback,
	};

	while (1) {
		int ret = UART_ReadAsync(ConsoleUart, &read_req);
		if (ret != E_NO_ERROR && ret != E_BUSY) {
			LOG_ERR("serial", "error reading uart: %d", ret);
			vTaskDelay(portMAX_DELAY);
		}

		ulTaskNotifyTake(pdTRUE, portTICK_PERIOD_MS * 1000);

		if (read_req.num > 0) {
			enqueue_char(*read_req.data);
		}

		while (UART_NumReadAvail(ConsoleUart) > 0) {
			enqueue_char(UART_ReadByte(ConsoleUart));
		}

		while (cdcacm_num_read_avail() > 0) {
			enqueue_char(cdcacm_read());
		}
	}
}
