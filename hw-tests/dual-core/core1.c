#include "board.h"
#include "gpio.h"
#include "mxc_delay.h"

static const __attribute__((unused))
gpio_cfg_t motor_pin = { PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE };

int main(void)
{
	// Enable rxev on core1
	MXC_GCR->evten |= 0x20;
	for (int i = 0; 1; i++) {
		__asm volatile("wfe");
		printf("core1: Hello! %d\n", i);

#if 0
		GPIO_OutSet(&motor_pin);
		mxc_delay(30000);
		GPIO_OutClr(&motor_pin);
#endif
	}
}
