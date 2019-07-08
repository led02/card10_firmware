#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

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
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
	epic_uart_write_str(str, len);
}

/* Used by MicroPython for debug output */
int DEBUG_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int ret = vprintf(fmt, args);
	va_end(args);
	return ret;
}

bool do_interrupt = false;

/* Timer Interrupt used for control char notification */
void TMR5_IRQHandler(void)
{
	TMR_IntClear(MXC_TMR5);

	if (do_interrupt) {
		/* Taken from lib/micropython/micropython/lib/utils/interrupt_char.c */
		MP_STATE_VM(mp_pending_exception) =
			MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_kbd_exception));
#if MICROPY_ENABLE_SCHEDULER
		if (MP_STATE_VM(sched_state) == MP_SCHED_IDLE) {
			MP_STATE_VM(sched_state) = MP_SCHED_PENDING;
		}
#endif
	}
}

void mp_hal_set_interrupt_char(char c)
{
	if (c != -1) {
		mp_obj_exception_clear_traceback(
			MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_kbd_exception))
		);
	}
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

extern NORETURN void *Reset_Handler(void);

void NORETURN nlr_jump_fail(void *val)
{
	char msg[] = " >>> nlr_jump_fail <<<\r\n";
	epic_uart_write_str(msg, sizeof(msg));

	Reset_Handler();
}

/******************************************************************************
 * Stubs
 */

mp_lexer_t *mp_lexer_new_from_file(const char *filename)
{
	/* TODO: Do we need an implementation for this? */
	mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path)
{
	return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs)
{
	/* TODO: Once fs is implemented, get this working as well */
	mp_raise_NotImplementedError ("FS is not yet implemented");
	return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);
