/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "pmic.h"
#include "leds.h"
#include "card10.h"

#include "GUI_Paint.h"

#include "tmr_utils.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <Heart.h>

/***** Definitions *****/

/***** Globals *****/
static const gpio_cfg_t motor_pin = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};

int main(void)
{
    int count = 0;

    card10_init();
    card10_diag();

    Paint_DrawImage(Heart, 0, 0, 160, 80);

    int h = 0;
    while (1) {
#if 0
        leds_set_hsv(0, h, 1., 1.);
        leds_set_hsv(1, (h + 90) % 360, 1., 1.);
        leds_set_hsv(2, (h + 180) % 360, 1., 1.);
        leds_set_hsv(3, (h + 270) % 360, 1., 1.);
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
