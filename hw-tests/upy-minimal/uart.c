#include <unistd.h>
#include "py/mpconfig.h"
#include "uart.h"

/*
 * Core UART functions to implement for a port
 */

extern mxc_uart_regs_t *ConsoleUart;

// Receive single character
int mp_hal_stdin_rx_chr(void)
{
	return UART_ReadByte(ConsoleUart);
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
	UART_Write(ConsoleUart, (uint8_t *)str, len);
}

void mp_hal_set_interrupt_char(char c)
{
	return;
}
