#include <stdint.h>

#include "py/lexer.h"
#include "py/mpconfig.h"
#include "py/mperrno.h"
#include "py/obj.h"
#include "py/runtime.h"

/******************************************************************************
 * Serial Communication
 */

/* TODO: Use API boundary instead of direct communication */
#include "uart.h"
extern mxc_uart_regs_t * ConsoleUart;

/* Receive single character */
int mp_hal_stdin_rx_chr(void)
{
	return UART_ReadByte(ConsoleUart);
}

/* Send string of given length */
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
	UART_Write(ConsoleUart, (uint8_t*)str, len);
}

/******************************************************************************
 * Fatal Errors
 */

void NORETURN nlr_jump_fail(void *val)
{
	/* TODO: Report error and restart */
	while (1) {
	}
}

/******************************************************************************
 * Stubs
 */

mp_lexer_t *mp_lexer_new_from_file(const char *filename)
{
	/* TODO: Do we need an implementation for this? */
	mp_raise_OSError(MP_ENOENT);
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs)
{
	/* TODO: Once fs is implemented, get this working as well */
	return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

/******************************************************************************
 * TODO: Remove
 */

int _getpid(void)
{
	;
}

int _kill(int pid, int f)
{
	;
}

void _exit(int r)
{
	;
}
