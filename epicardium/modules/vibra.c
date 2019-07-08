#include "gpio.h"

static const gpio_cfg_t motor_pin = {
	PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE
};

void epic_vibra_set(int status)
{
	if (status) {
		GPIO_OutSet(&motor_pin);
	} else {
		GPIO_OutClr(&motor_pin);
	}
}
