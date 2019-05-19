/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-09-04 22:13:32 +0000 (Tue, 04 Sep 2018) $
 * $Revision: 37649 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
 */

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
