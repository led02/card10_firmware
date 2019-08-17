#include "epicardium.h"

#include "portexpander.h"
#include "MAX77650-Arduino-Library.h"

#include <stdint.h>

static const uint8_t pin_mask[] = {
	[BUTTON_LEFT_BOTTOM]  = 1 << 5,
	[BUTTON_RIGHT_BOTTOM] = 1 << 3,
	[BUTTON_RIGHT_TOP]    = 1 << 6,
};

uint8_t epic_buttons_read(uint8_t mask)
{
	uint8_t ret = 0;
	if (portexpander_detected() && (mask & 0x3)) {
		uint8_t pin_status = ~portexpander_get();

		for (uint8_t m = 1; m < 0x8; m <<= 1) {
			if (mask & m && pin_status & pin_mask[m]) {
				ret |= m;
			}
		}
	}

	if (mask & BUTTON_RESET && MAX77650_getDebounceStatusnEN0()) {
		ret |= BUTTON_RESET;
	}

	return ret;
}
