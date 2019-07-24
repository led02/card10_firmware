#include "epicardium.h"

#include <stdarg.h>
#include <stdio.h>

#include "py/mpprint.h"
#include "py/mphal.h"

#include <string.h>

int printf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int ret = mp_vprintf(MP_PYTHON_PRINTER, fmt, ap);
	va_end(ap);
	return ret;
}

#ifdef putc
#undef putc
#endif /* putc */
int putc(int c, FILE *f)
{
	char chr = (char)c;
	mp_hal_stdout_tx_strn_cooked(&chr, 1);
	return c;
}

#ifdef putchar
#undef putchar
#endif /* putchar */
int putchar(int c)
{
	char chr = (char)c;
	mp_hal_stdout_tx_strn_cooked(&chr, 1);
	return c;
}

int puts(const char *s)
{
	int length = strlen(s);
	if (length) {
		mp_hal_stdout_tx_strn_cooked(s, length);
	}
	return length;
}

/* Used by mp_hal_move_cursor_back() */
int snprintf(char *str, size_t size, const char *format, ...)
{
	/* TODO: What should we do with this? */
	return -EIO;
}

/* Use by assert() */
int fiprintf(FILE *f, const char *fmt, ...)
{
	/* TODO: Maybe printing everything to UART isn't the correct thing? */
	va_list ap;
	va_start(ap, fmt);
	int ret = mp_vprintf(MP_PYTHON_PRINTER, fmt, ap);
	va_end(ap);
	return ret;
}

int raise(int sig)
{
	/* Ignore signals */
	return 0;
}
