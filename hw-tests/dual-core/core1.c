#include "board.h"
#include "gpio.h"
#include "mxc_delay.h"

static const gpio_cfg_t motor_pin = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};

int main(void)
{
	for (int i = 0; 1; i++) {
		printf("Hello from core 1! %d\n", i);

#if 0
		GPIO_OutSet(&motor_pin);
		mxc_delay(30000);
		GPIO_OutClr(&motor_pin);
#endif
		mxc_delay(970000);
	}
}
