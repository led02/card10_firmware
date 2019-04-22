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
 * $Date: 2018-08-10 17:11:51 -0500 (Fri, 10 Aug 2018) $
 * $Revision: 36872 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "led.h"
#include "board.h"
#include "tmr_utils.h"
#include "mcr_regs.h"
#include "sema.h"
#include "core1.h"

/***** Definitions *****/
extern uint32_t __isr_vector_core1;
extern uint32_t __StackTop_Core1;
extern uint32_t __StackLimit_Core1;
int count0 = 0;
int count1 = 0;
/***** Globals *****/

/***** Functions *****/

// Similar to Core 0, the entry point for Core 1
// is Core1Main()
// Execution begins when the CPU1 Clock is enabled
int Core1_Main(void) {
    while (1) {
        // Wait for Core 0 to release the semaphore
        while(SEMA_GetSema(0) == E_BUSY) {}

        // Hold the semaphore during these delays to stall Core 0
        LED_On(0);
        LED_Off(1);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);

        // Print the updated value from Core 0
        printf("Core 1: Ping: %d\n", count1);
        LED_Off(0);
        LED_On(1);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);

        // Update the count and release the semaphore
        // causing Core 0 to print the new count
        count0++;
        SEMA_FreeSema(0);
    }
}

// *****************************************************************************
int main(void)
{
    sys_cfg_sema_t sema_cfg = NULL;
    printf("***** Dual Core Example *****\n");
    printf("Runs the 'Hello World' exampled by splitting the\n");
    printf("lights and console uart between core 0 and core 1.\n");
    printf("halting this example with a debugger will not stop core 1.\n\n");

    SEMA_Init(sema_cfg);
    Core1_Start();

    while(1) {
        // Wait for Core 1 to update count and release the semaphore
        while(SEMA_GetSema(0) == E_BUSY) {}
        SEMA_GetSema(0);
        printf("Core 0: Pong: %d\n", count0);
        count1++;
        SEMA_FreeSema(0);

        // Delay after releasing the semaphore to avoid a race condition
        TMR_Delay(MXC_TMR1, MSEC(10), 0);
    }
}