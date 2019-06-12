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
 * $Date: 2018-08-10 14:36:39 +0000 (Fri, 10 Aug 2018) $
 * $Revision: 36825 $
 *
 ******************************************************************************/

/*
 * @file    main.c
 * @brief   Demonstrates a watchdog timer in run mode
 *
 * @details When the program starts LED3 blinks three times and stops.
 *          Then LED0 start blinking continuously.
 *          Open a terminal program to see interrupt messages.
 *
 *          SW1: Push SW1 to trigger a watchdog reset. This will reset the watchdog before
 *               the wait period has expired and trigger an interrupt.
 *
 *          SW2: Push SW2 to trigger a delay and see LED0 stop blinking momentarily.
 *               This delay long enough for the timeout period to expire and trigger an interrupt.
 *
 *          SW3: Push SW3 to trigger a longer delay and see the program restart by blinking LED3 three times.
 *               This delay is long enough for the reset period to expire and trigger a reset.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "board.h"
#include "mxc_sys.h"
#include "wdt.h"
#include "tmr_utils.h"


/***** Definitions *****/
/***** Globals *****/
//use push buttons defined in board.h
extern const gpio_cfg_t pb_pin[];

#define SW1 0
#define SW2 1

/***** Functions *****/

// *****************************************************************************
void watchdog_timeout_handler()
{
    //get and clear flag
	WDT_GetIntFlag(MXC_WDT0);
    WDT_ClearIntFlag(MXC_WDT0);
    printf("TIMEOUT! \n");
}

// *****************************************************************************
void WDT0_IRQHandler(void){

	watchdog_timeout_handler();
}
// *****************************************************************************
void WDT_Setup()
{
    WDT_Enable(MXC_WDT0, 0);
    WDT_Enable(MXC_WDT0, 1);
}

// *****************************************************************************
int main(void)
{
    sys_cfg_wdt_t wdt_cfg = NULL;
    WDT_Init(MXC_WDT0, wdt_cfg);

	if(WDT_GetResetFlag(MXC_WDT0)) {
	        WDT_ClearResetFlag(MXC_WDT0);
	        WDT_EnableReset(MXC_WDT0, 0);
	        WDT_Enable(MXC_WDT0, 0);
	        printf("Watchdog reset\n");
	    }

    printf("\n************** Watchdog Timer Demo ****************\n");
    printf("Press a button to create watchdog interrupt or reset:\n");
    printf("SW1 = reset program\n");
    printf("SW2 = timeout interrupt\n");
    //LED_Off(0);

    //blink LED3 three times at startup
    int numBlinks = 3;

    while(numBlinks) {
        //LED_On(3);
        TMR_Delay(MXC_TMR0, MSEC(100), 0);
       // LED_Off(3);
        TMR_Delay(MXC_TMR0, MSEC(100), 0);
        numBlinks--;
    }

    //setup watchdog
    WDT_Setup();

    while(1) {

        //Push SW1 to reset watchdog
        if(GPIO_InGet(&pb_pin[SW1]) == 0) {
            printf("Resetting Program...\n");
            WDT_SetResetPeriod(MXC_WDT0, WDT_PERIOD_2_18);
            WDT_EnableReset(MXC_WDT0, 1);
            WDT_EnableInt(MXC_WDT0, 0);
            WDT_ResetTimer(MXC_WDT0);
            while(1);

        }

        //Push SW2 to start longer delay - shows Interrupt before the reset happens
        if(GPIO_InGet(&pb_pin[SW2]) == 0) {
            printf("Enabling Timeout Interrupt...\n");
            WDT_SetResetPeriod(MXC_WDT0, WDT_PERIOD_2_27);
            WDT_SetIntPeriod(MXC_WDT0, WDT_PERIOD_2_26);
            WDT_EnableReset(MXC_WDT0, 1);
        	WDT_EnableInt(MXC_WDT0, 1);
            NVIC_EnableIRQ(WDT0_IRQn);
            while(1);
        }

        //blink LED0
        TMR_Delay(MXC_TMR0, MSEC(100), 0);
        //LED_On(0);
        TMR_Delay(MXC_TMR0, MSEC(100), 0);
        //LED_Off(0);

        //Reset watchdog
        WDT_ResetTimer(MXC_WDT0);
    }
}
