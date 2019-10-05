/*
 * Panic
 * =====
 *
 * Under some conditions the firmware should crash and reboot automatically.
 * This module provides the necessary facilities to do so.
 *
 * Note that a panic should indicate **only** logic-errors in the firmware or
 * unrecoverable hardware conditions.
 */

#include "modules/log.h"
#include "modules/modules.h"

#include "card10.h"
#include "card10-version.h"

#include <stdio.h>
#include <stdarg.h>

void __attribute__((noreturn)) panic(const char *format, ...)
{
	/* Turn off interrupts.  We won't get back from here anyway. */
	__asm volatile("cpsid	i" ::: "memory");

	/*
	 * Turn off asynchronous printing because that won't ever work from
	 * here ...
	 */
	serial_return_to_synchronous();

	printf("\x1b[31;1m           --- SYSTEM PANIC ---\n"
	       "\x1b[0;31m         ---                  ---\n"
	       "       ---                      ---\n"
	       "\x1b[0m A fatal error occured:\n     \x1b[1m");

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	printf("\n"
	       "\x1b[0m\n"
	       " Firmware Version:\n"
	       "\x1b[35m     %s\n",
	       CARD10_VERSION);

	printf("\x1b[0m\n"
	       " Stack Trace:\n"
	       "\x1b[36m     %p\n",
	       __builtin_return_address(0));

	printf("\x1b[33m\n"
	       " Please report this error to the card10 firmware team!\n"
	       "\x1b[0m -> https://git.card10.badge.events.ccc.de/card10/firmware/issues/new?issue <-\n"
	       "\x1b[31m           --- ====== ===== ---\x1b[0m\n");

	for (int i = 0; i < 96000000; i++) {
		__asm volatile("nop");
	}

	card10_reset();
}

void __attribute__((noreturn)) __assert_func(
	const char *file, int line, const char *func, const char *failedexpr
) {
	panic("Assertion failure:\n"
	      "       \"%s\"\n"
	      "       failed in \"%s:%d\",\n"
	      "       function: %s()",
	      failedexpr,
	      file,
	      line,
	      func);
}
