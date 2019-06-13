#ifndef _API_H
#define _API_H
#include <stdint.h>

#ifndef API
#  define API(id, def) def
#endif

#define API_BUZZER 0x35c3
API(API_BUZZER, void api_set_buzzer(uint8_t state));

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color_t;

#define API_LED 0xc0ffee
API(API_LED, void api_set_led(uint8_t led, led_color_t color));

#endif /* _API_H */
