#include "py/mpconfig.h"

/* TODO: Replace this with a proper implementation */
static inline mp_uint_t mp_hal_ticks_ms(void)
{
	return 0;
}

void mp_hal_set_interrupt_char(char c);
