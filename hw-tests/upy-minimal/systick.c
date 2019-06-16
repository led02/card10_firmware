#include "py/mpconfig.h"
#include "mxc_delay.h"

void mp_hal_delay_ms(mp_uint_t ms) {
	mxc_delay (ms * 1000); // TODO check return value
}

void mp_hal_delay_us(mp_uint_t us) {
	mxc_delay (us); // TODO check return value
}

mp_uint_t mp_hal_ticks_ms(void) {
	return 0;
}

mp_uint_t mp_hal_ticks_us(void) {
	return 0;
}

mp_uint_t mp_hal_ticks_cpu(void) {
	return 0;
}
