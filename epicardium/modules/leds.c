#include "leds.h"

void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b)
{
	leds_set(led, r, g, b);
	leds_update();
}