#include "epicardium.h"
#include "api/interrupt-sender.h"
#include "modules/log.h"
#include "modules/modules.h"

#include "max32665.h"
#include "usb/cdcacm.h"
#include "uart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stream_buffer.h"

#include <stdint.h>
#include <stdio.h>

/* The serial console in use (UART0) */
extern mxc_uart_regs_t *ConsoleUart;

/* Task ID for the serial handler */
TaskHandle_t serial_task_id = NULL;

/* Read queue, filled by both UART and CDCACM */
static QueueHandle_t read_queue;
/* Stream Buffer for handling all writes to serial */
static StreamBufferHandle_t write_stream_buffer = NULL;

void serial_init()
{
	/* Setup read queue */
	static uint8_t buffer[sizeof(char) * SERIAL_READ_BUFFER_SIZE];
	static StaticQueue_t read_queue_data;
	read_queue = xQueueCreateStatic(
		SERIAL_READ_BUFFER_SIZE, sizeof(char), buffer, &read_queue_data
	);

	/* Setup write queue */
	static uint8_t ucWrite_stream_buffer[SERIAL_WRITE_STREAM_BUFFER_SIZE];
	static StaticStreamBuffer_t xStream_buffer_struct;
	write_stream_buffer = xStreamBufferCreateStatic(
		sizeof(ucWrite_stream_buffer),
		1,
		ucWrite_stream_buffer,
		&xStream_buffer_struct
	);
}

/*
 * API-call to write a string.  Output goes to both CDCACM and UART
 */
void epic_uart_write_str(const char *str, intptr_t length)
{
	if (length == 0) {
		return;
	}

	/*
	 * Check if the stream buffer is even initialized yet
	 */
	if (write_stream_buffer == NULL) {
		UART_Write(ConsoleUart, (uint8_t *)str, length);
		cdcacm_write((uint8_t *)str, length);
		return;
	}

	if (xPortIsInsideInterrupt()) {
		BaseType_t resched1 = pdFALSE;
		BaseType_t resched2 = pdFALSE;

		/*
		 * Enter a critial section so no other task can write to the
		 * stream buffer.
		 */
		uint32_t basepri = __get_BASEPRI();
		taskENTER_CRITICAL_FROM_ISR();

		xStreamBufferSendFromISR(
			write_stream_buffer, str, length, &resched1
		);

		taskEXIT_CRITICAL_FROM_ISR(basepri);

		if (serial_task_id != NULL) {
			xTaskNotifyFromISR(
				serial_task_id,
				SERIAL_WRITE_NOTIFY,
				eSetBits,
				&resched2
			);
		}

		/* Yield if this write woke up a higher priority task */
		portYIELD_FROM_ISR(resched1 || resched2);
	} else {
		size_t bytes_sent = 0;
		size_t index      = 0;
		do {
			taskENTER_CRITICAL();
			/*
			 * Wait time needs to be zero, because we are in a
			 * critical section.
			 */
			bytes_sent = xStreamBufferSend(
				write_stream_buffer,
				&str[index],
				length - index,
				0
			);
			index += bytes_sent;
			taskEXIT_CRITICAL();
			if (serial_task_id != NULL) {
				xTaskNotify(
					serial_task_id,
					SERIAL_WRITE_NOTIFY,
					eSetBits
				);
				portYIELD();
			}
		} while (index < length);
	}
}

/*
 * API-call to read a character from the queue.
 */
int epic_uart_read_char(void)
{
	char chr;
	if (xQueueReceive(read_queue, &chr, 0) == pdTRUE) {
		return (int)chr;
	}
	return (-1);
}

/*
 * API-call to read data from the queue.
 */
int epic_uart_read_str(char *buf, size_t cnt)
{
	size_t i = 0;

	for (i = 0; i < cnt; i++) {
		if (xQueueReceive(read_queue, &buf[i], 0) != pdTRUE) {
			break;
		}
	}

	return i;
}

long _write_epicardium(int fd, const char *buf, size_t cnt)
{
	/*
	 * Only print one line at a time.  Insert `\r` between lines so they are
	 * properly displayed on the serial console.
	 */
	size_t i, last = 0;
	for (i = 0; i < cnt; i++) {
		if (buf[i] == '\n') {
			epic_uart_write_str(&buf[last], i - last);
			epic_uart_write_str("\r", 1);
			last = i;
		}
	}
	epic_uart_write_str(&buf[last], cnt - last);
	return cnt;
}

/* Interrupt handler needed for SDK UART implementation */
void UART0_IRQHandler(void)
{
	UART_Handler(ConsoleUart);
}

static void uart_callback(uart_req_t *req, int error)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xTaskNotifyFromISR(
		serial_task_id,
		SERIAL_READ_NOTIFY,
		eSetBits,
		&xHigherPriorityTaskWoken
	);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void serial_enqueue_char(char chr)
{
	if (chr == 0x3) {
		/* Control-C */
		api_interrupt_trigger(EPIC_INT_CTRL_C);
	}

	if (xQueueSend(read_queue, &chr, 100) == errQUEUE_FULL) {
		/* Queue overran, wait a bit */
		vTaskDelay(portTICK_PERIOD_MS * 50);
	}

	api_interrupt_trigger(EPIC_INT_UART_RX);
}

void vSerialTask(void *pvParameters)
{
	serial_task_id = xTaskGetCurrentTaskHandle();

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

	uint8_t rx_data[20];
	size_t received_bytes;

	while (1) {
		int ret = UART_ReadAsync(ConsoleUart, &read_req);
		if (ret != E_NO_ERROR && ret != E_BUSY) {
			LOG_ERR("serial", "error reading uart: %d", ret);
			vTaskDelay(portMAX_DELAY);
		}

		ret = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if (ret & SERIAL_WRITE_NOTIFY) {
			do {
				received_bytes = xStreamBufferReceive(
					write_stream_buffer,
					(void *)rx_data,
					sizeof(rx_data),
					0
				);

				if (received_bytes == 0) {
					break;
				}

				/*
				 * The SDK-driver for UART is not reentrant
				 * which means we need to perform UART writes
				 * in a critical section.
				 */
				taskENTER_CRITICAL();
				UART_Write(
					ConsoleUart,
					(uint8_t *)&rx_data,
					received_bytes
				);
				taskEXIT_CRITICAL();

				cdcacm_write(
					(uint8_t *)&rx_data, received_bytes
				);
				ble_uart_write(
					(uint8_t *)&rx_data, received_bytes
				);
			} while (received_bytes > 0);
		}

		if (ret & SERIAL_READ_NOTIFY) {
			if (read_req.num > 0) {
				serial_enqueue_char(*read_req.data);
			}

			while (UART_NumReadAvail(ConsoleUart) > 0) {
				serial_enqueue_char(UART_ReadByte(ConsoleUart));
			}

			while (cdcacm_num_read_avail() > 0) {
				serial_enqueue_char(cdcacm_read());
			}
		}
	}
}
