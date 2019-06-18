#include <stdio.h>
#include "card10.h"
#include "uart.h"
#include "api/dispatcher.h"

extern mxc_uart_regs_t * ConsoleUart;

void epic_uart_write_str(char*str, intptr_t length)
{
	UART_Write(ConsoleUart, (uint8_t*)str, length);
}

char epic_uart_read_chr(void)
{
	return UART_ReadByte(ConsoleUart);
}

void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b)
{
	leds_set(led, r, g, b);
	leds_update();
}

int main(void)
{
	card10_init();
	card10_diag();

	printf("Initializing dispatcher ...\n");
	api_dispatcher_init();

	printf("Staring core1 payload ...\n");
	core1_start();

	while(1) {
		__WFE();
		api_dispatcher_poll();
	}
}
