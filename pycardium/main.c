#include "epicardium.h"
#include "api/caller.h"
#include "mphalport.h"
#include "card10-version.h"

#include "max32665.h"

#include "lib/utils/pyexec.h"
#include "py/gc.h"
#include "py/runtime.h"
#include "py/stackctrl.h"

#include <stdio.h>

/* Defined in linker script */
extern void *__StackTop, *__StackLimit;
extern void *__HeapBase, *__HeapLimit;

static const char header[] =
	"--------------------------------\r\n"
	"          Pycardium\r\n"
	" Version: " CARD10_VERSION
	"\r\n"
	"--------------------------------\r\n";

int main(void)
{
	char script_name[128] = { 0 };
	int cnt = api_fetch_args(script_name, sizeof(script_name));

	pycardium_hal_init();

	epic_uart_write_str(header, sizeof(header));

	if (cnt < 0) {
		printf("pycardium: Error fetching args: %d\n", cnt);
	} else if (cnt > 0) {
		printf("  Loading %s ...\n", script_name);
	}

	mp_stack_set_top(&__StackTop);
	mp_stack_set_limit((mp_int_t)&__StackLimit);

	while (1) {
		gc_init(&__HeapBase + 1024 * 10, &__HeapLimit);

		mp_init();

		if (cnt > 0) {
			pyexec_file_if_exists(script_name);
		}

		pyexec_friendly_repl();

		mp_deinit();
	}
}

void HardFault_Handler(void)
{
	epic_exit(255);
}

void gc_collect(void)
{
	void *sp = (void *)__get_MSP();

	gc_collect_start();
	gc_collect_root(
		sp,
		((mp_uint_t)&__StackTop - (mp_uint_t)sp) / sizeof(mp_uint_t)
	);
	gc_collect_end();
}
