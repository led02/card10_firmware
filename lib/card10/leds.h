#ifndef LED_H

#include <stdint.h>

void leds_set_dim(uint8_t led, uint8_t dim);
void leds_set(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
void leds_set_hsv(uint8_t led, float h, float s, float v);
void leds_update(void);
void leds_init(void);

#endif
