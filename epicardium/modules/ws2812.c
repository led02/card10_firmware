#include "leds.h"
#include "pmic.h"
#include "FreeRTOS.h"
#include "task.h"
#include "epicardium.h"
#include "max32665.h"
#include "gpio.h"
#include "modules.h"

#include <stdbool.h>

#define OVERHEAD 33

#define EPIC_WS2812_ZERO_HIGH_TICKS (34 - OVERHEAD)
#define EPIC_WS2812_ZERO_LOW_TICKS (86 - OVERHEAD)
#define EPIC_WS2812_ONE_HIGH_TICKS (86 - OVERHEAD)
#define EPIC_WS2812_ONE_LOW_TICKS (34 - OVERHEAD)
#define EPIC_WS2812_RESET_TCKS (4800 - OVERHEAD)

static volatile uint32_t counter = 0;

static inline __attribute__((always_inline)) void
epic_ws2812_delay_ticks(uint32_t ticks)
{
	counter = ticks;
	while (--counter) {
	};
}

static inline __attribute__((always_inline)) void
epic_ws2812_transmit_bit(gpio_cfg_t *pin, uint8_t bit)
{
	if (bit != 0) {
		GPIO_OutSet(pin);
		epic_ws2812_delay_ticks(EPIC_WS2812_ONE_HIGH_TICKS);
		GPIO_OutClr(pin);
		epic_ws2812_delay_ticks(EPIC_WS2812_ONE_LOW_TICKS);
	} else {
		GPIO_OutSet(pin);
		epic_ws2812_delay_ticks(EPIC_WS2812_ZERO_HIGH_TICKS);
		GPIO_OutClr(pin);
		epic_ws2812_delay_ticks(EPIC_WS2812_ZERO_LOW_TICKS);
	}
}

static inline __attribute__((always_inline)) void
epic_ws2812_transmit_byte(gpio_cfg_t *pin, uint8_t byte)
{
	epic_ws2812_transmit_bit(pin, byte & 0b10000000);
	epic_ws2812_transmit_bit(pin, byte & 0b01000000);
	epic_ws2812_transmit_bit(pin, byte & 0b00100000);
	epic_ws2812_transmit_bit(pin, byte & 0b00010000);
	epic_ws2812_transmit_bit(pin, byte & 0b00001000);
	epic_ws2812_transmit_bit(pin, byte & 0b00000100);
	epic_ws2812_transmit_bit(pin, byte & 0b00000010);
	epic_ws2812_transmit_bit(pin, byte & 0b00000001);
}

void epic_ws2812_write(uint8_t pin, uint8_t *pixels, uint32_t n_bytes)
{
	uint8_t *pixels_end = pixels + n_bytes;
	gpio_cfg_t *pin_cfg = &gpio_configs[pin];

	taskENTER_CRITICAL();

	epic_gpio_set_pin_mode(pin, EPIC_GPIO_MODE_OUT);

	do {
		epic_ws2812_transmit_byte(pin_cfg, *pixels);
	} while (++pixels != pixels_end);

	GPIO_OutClr(pin_cfg);
	epic_ws2812_delay_ticks(EPIC_WS2812_RESET_TCKS);

	taskEXIT_CRITICAL();
}
