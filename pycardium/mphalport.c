#include "epicardium.h"
#include "api/common.h"

#include "max32665.h"
#include "mxc_delay.h"
#include "tmr.h"

/* stdarg.h must be included before mpprint.h */
#include <stdarg.h>

#include "py/lexer.h"
#include "py/mpconfig.h"
#include "py/mperrno.h"
#include "py/mpstate.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mpprint.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Initialize everything for MicroPython */
void pycardium_hal_init(void)
{
	/* TMR5 is used for interrupts from Epicardium */
	NVIC_EnableIRQ(TMR5_IRQn);

	/*
	 * Enable UART RX Interrupt so Pycardium can sleep until
	 * a character becomes available.
	 */
	epic_interrupt_enable(EPIC_INT_UART_RX);
}

/******************************************************************************
 * Serial Communication
 */

/* Receive single character */
int mp_hal_stdin_rx_chr(void)
{
	int chr;
	while ((chr = epic_uart_read_char()) < 0) {
		__WFI();
	}
	return chr;
}

/* Send a string */
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
	epic_uart_write_str(str, len);
}

/* Send a string, but replace \n with \n\r */
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len)
{
	/*
	 * Only print one line at a time.  Insert `\r` between lines so
	 * they are properly displayed on the serial console.
	 */
	size_t i, last = 0;
	for (i = 0; i < len; i++) {
		if (str[i] == '\n') {
			epic_uart_write_str(&str[last], i - last);
			epic_uart_write_str("\r", 1);
			last = i;
		}
	}
	epic_uart_write_str(&str[last], len - last);
}

/* Send a zero-terminated string */
void mp_hal_stdout_tx_str(const char *str)
{
	mp_hal_stdout_tx_strn(str, strlen(str));
}

/* Used by MicroPython for debug output */
int DEBUG_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int ret = mp_vprintf(MP_PYTHON_PRINTER, fmt, args);
	va_end(args);
	return ret;
}

void __attribute__((noreturn)) sbrk_is_not_implemented___see_issue_44(void);
intptr_t _sbrk(int incr)
{
	sbrk_is_not_implemented___see_issue_44();
}

void epic_isr_ctrl_c(void)
{
	/* Taken from lib/micropython/micropython/lib/utils/interrupt_char.c */
	MP_STATE_VM(mp_pending_exception) =
		MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_kbd_exception));
#if MICROPY_ENABLE_SCHEDULER
	if (MP_STATE_VM(sched_state) == MP_SCHED_IDLE) {
		MP_STATE_VM(sched_state) = MP_SCHED_PENDING;
	}
#endif
}

void mp_hal_set_interrupt_char(char c)
{
	if (c != -1) {
		mp_obj_exception_clear_traceback(
			MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_kbd_exception))
		);
	}

	if (c == 0x03) {
		epic_interrupt_enable(EPIC_INT_CTRL_C);
	} else {
		epic_interrupt_disable(EPIC_INT_CTRL_C);
	}
}

/******************************************************************************
 * Time & Delay
 */

void mp_hal_delay_ms(mp_uint_t ms)
{
	mxc_delay(ms * 1000);
}

void mp_hal_delay_us(mp_uint_t us)
{
	mxc_delay(us);
}

mp_uint_t mp_hal_ticks_ms(void)
{
	return 0;
}

/******************************************************************************
 * Fatal Errors
 */

extern NORETURN void *Reset_Handler(void);

void NORETURN nlr_jump_fail(void *val)
{
	char msg[] = " >>> nlr_jump_fail <<<\r\n";
	epic_uart_write_str(msg, sizeof(msg));

	Reset_Handler();
}
