/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "pmic.h"
#include "leds.h"
#include "card10.h"

#include "gfx.h"
#include "display.h"

#include "tmr_utils.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <Heart.h>

/***** Definitions *****/

/***** Globals *****/
static __attribute__((unused))
const gpio_cfg_t motor_pin = { PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE };

int main(void)
{
	int count = 0;

	card10_init();
	card10_diag();

	gfx_copy_region(
		&display_screen,
		0,
		0,
		160,
		80,
		GFX_RAW,
		sizeof(Heart),
		(const void *)(Heart)
	);
	gfx_update(&display_screen);

	for (int i = 0; i < 11; i++) {
		//        leds_set_dim(i, 1);
	}

	int __attribute__((unused)) h = 0;
	while (1) {
#if 0

#define NUM 15
        for(int i=0; i<NUM; i++) {
            if(i < 12) {
                leds_set_hsv(i, (h + 360/NUM * i) % 360, 1., 1./8);
            } else {
                leds_set_hsv(i, (h + 360/NUM * i) % 360, 1., 1.);
            }
        }

        leds_update();
        TMR_Delay(MXC_TMR0, MSEC(10), 0);
        h++;
#endif
#if 1
		pmic_set_led(0, 31);
		pmic_set_led(1, 0);
		pmic_set_led(2, 0);
		TMR_Delay(MXC_TMR0, MSEC(200), 0);

		pmic_set_led(0, 0);
		pmic_set_led(1, 31);
		pmic_set_led(2, 0);
		TMR_Delay(MXC_TMR0, MSEC(200), 0);

		pmic_set_led(0, 0);
		pmic_set_led(1, 0);
		pmic_set_led(2, 31);
		TMR_Delay(MXC_TMR0, MSEC(200), 0);

		pmic_set_led(0, 0);
		pmic_set_led(1, 0);
		pmic_set_led(2, 0);
		//TMR_Delay(MXC_TMR0, MSEC(200), 0);
#endif
#if 0
        //TMR_Delay(MXC_TMR0, MSEC(600), 0);
        GPIO_OutSet(&motor_pin);
        TMR_Delay(MXC_TMR0, MSEC(30), 0);
        GPIO_OutClr(&motor_pin);
        TMR_Delay(MXC_TMR0, MSEC(200), 0);
#endif
#if 0
        LED_Off(0);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);
        LED_On(0);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);
#endif
#if 1
		TMR_Delay(MXC_TMR0, MSEC(1000), 0);
		printf("count = %d\n", count++);
#endif
	}
}
