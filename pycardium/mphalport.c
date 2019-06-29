#include <stdint.h>

#include "py/lexer.h"
#include "py/mpconfig.h"
#include "py/mperrno.h"
#include "py/mpstate.h"
#include "py/obj.h"
#include "py/runtime.h"

#include "mxc_delay.h"
#include "max32665.h"
#include "tmr.h"

#include "epicardium.h"

/******************************************************************************
 * Serial Communication
 */

/* Receive single character */
int mp_hal_stdin_rx_chr(void)
{
	return (int)epic_uart_read_chr();
}

/* Send string of given length */
void mp_hal_stdout_tx_strn(const char* str, mp_uint_t len)
{
	epic_uart_write_str(str, len);
}

bool do_interrupt = false;

/* Timer Interrupt used for control char notification */
void TMR5_IRQHandler(void)
{
	TMR_IntClear(MXC_TMR5);

	if (do_interrupt) {
		/* Taken from lib/micropython/micropython/ports/unix/unix_mphal.c */
		mp_obj_exception_clear_traceback(
			MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_kbd_exception))
		);
		nlr_raise(MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_kbd_exception)));
	}
}

void mp_hal_set_interrupt_char(char c)
{
	do_interrupt = (c == 0x03);
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

/******************************************************************************
 * Fatal Errors
 */

void NORETURN nlr_jump_fail(void* val)
{
	/* TODO: Report error and restart */
	char msg[] = " >>> nlr_jump_fail <<<\r\n";
	epic_uart_write_str(msg, sizeof(msg));

	while (1) {
		__asm("nop");
	}
}

/******************************************************************************
 * Stubs
 */

mp_lexer_t* mp_lexer_new_from_file(const char* filename)
{
	/* TODO: Do we need an implementation for this? */
	mp_raise_OSError(MP_ENOENT);
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t* args, mp_map_t* kwargs)
{
	/* TODO: Once fs is implemented, get this working as well */
	return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);
