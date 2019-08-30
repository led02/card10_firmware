#ifndef LED_H

#include <stdint.h>
#include <stdbool.h>

void leds_set_dim_top(uint8_t value);
void leds_set_dim_bottom(uint8_t value);
void leds_prep(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
void leds_get_rgb(uint8_t led, uint8_t * rgb);
void leds_prep_hsv(uint8_t led, float h, float s, float v);
void leds_update_power(void);
void leds_update(void);
void leds_init(void);
void leds_powersave(bool eco);
void leds_flashlight(bool power);
void leds_set_gamma_table(uint8_t rgb_channel, uint8_t table[256]);

#endif
