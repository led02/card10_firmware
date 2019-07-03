#include <stdint.h>
#include <stdio.h>

#include "serial.h"

#include "cdcacm.h"
#include "uart.h"
#include "tmr_utils.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern mxc_uart_regs_t * ConsoleUart;

static QueueHandle_t read_queue;

void epic_uart_write_str(char*str, intptr_t length)
{
	UART_Write(ConsoleUart, (uint8_t*)str, length);
	cdcacm_write((uint8_t*)str, length);
}

char epic_uart_read_chr(void)
{
	char chr;
	xQueueReceive(read_queue, &chr, portMAX_DELAY);
	return chr;
}

void vSerialTask(void*pvParameters)
{
	static uint8_t buffer[sizeof(char) * SERIAL_READ_BUFFER_SIZE];
	static StaticQueue_t read_queue_data;

	/* Setup read queue */
	read_queue = xQueueCreateStatic(
		SERIAL_READ_BUFFER_SIZE,
		sizeof(char),
		buffer,
		&read_queue_data
	);

	/* Setup UART interrupt */
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_DisableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);

	while (1) {
		char chr;

		/* TODO: Wait for interrupt on either device */
		vTaskDelay(portTICK_PERIOD_MS * 10);

		if(UART_NumReadAvail(ConsoleUart) > 0) {
			chr = UART_ReadByte(ConsoleUart);
		} else if(cdcacm_num_read_avail() > 0) {
			chr = cdcacm_read();
		} else {
			continue;
		}

		if (chr == 0x3) {
			/* Control-C */
			TMR_TO_Start(MXC_TMR5, 1, 0);
		}

		if (xQueueSend(read_queue, &chr, 100) == errQUEUE_FULL) {
			/* Queue overran, wait a bit */
			vTaskDelay(portTICK_PERIOD_MS * 50);
		}
	}
}
