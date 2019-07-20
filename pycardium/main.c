#include "py/runtime.h"
#include "py/gc.h"
#include "lib/utils/pyexec.h"
#include "py/stackctrl.h"
#include <stdio.h>

#include "max32665.h"

/* Defined in linker script */
extern void *__StackTop, *__StackLimit;
extern void *__HeapBase, *__HeapLimit;

int main(void)
{
	mp_stack_set_top(&__StackTop);
	mp_stack_set_limit((mp_int_t)&__StackLimit);

	/* TMR5 is used to notify on keyboard interrupt */
	NVIC_EnableIRQ(TMR5_IRQn);

	while (1) {
		gc_init(&__HeapBase + 1024 * 10, &__HeapLimit);

		mp_init();
		pyexec_friendly_repl();
		mp_deinit();
	}
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
