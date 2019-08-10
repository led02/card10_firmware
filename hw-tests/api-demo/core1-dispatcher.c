#include "board.h"
#include "gpio.h"
#include "mxc_delay.h"
#include "api.h"
#include "tmr_utils.h"
#include "leds.h"

#include "api/api_dispatcher.h"

static const gpio_cfg_t motor_pin = {
	PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE
};

void api_set_buzzer(uint8_t state)
{
	if (state) {
		printf("API: Turning motor ON!\n");
		GPIO_OutSet(&motor_pin);
	} else {
		printf("API: Turning motor OFF!\n");
		GPIO_OutClr(&motor_pin);
	}
}

void api_set_led(uint8_t led, led_color_t color)
{
	printf("API: Changing color of led %d.\n", led);
	printf("Color { r: %3d, g: %3d, b: %3d }\n",
	       color.red,
	       color.green,
	       color.blue);
	leds_set(led, color.red, color.green, color.blue);
	leds_update();
}

void api_test(char test0, short test1, int test2, long test3)
{
	printf("test0: %x, test1: %d, test2: %x, test3: %lx\n",
	       test0,
	       (int)test1,
	       test2,
	       test3);
}

int main(void)
{
	api_init(NULL);
	leds_init();

	while (1) {
		api_dispatcher();
		TMR_Delay(MXC_TMR1, MSEC(100), 0);
	}

#if 0
	// Enable rxev on core1
	MXC_GCR->evten |= 0x20;
	for (int i = 0; 1; i++) {
		__asm volatile("wfe");
		printf("core1: Hello! %d\n", i);
	}
#endif
}
