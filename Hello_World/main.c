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
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "led.h"
#include "board.h"
#include "tmr_utils.h"
#include "i2c.h"

/***** Definitions *****/

#define I2C_DEVICE	    MXC_I2C0_BUS0

/***** Globals *****/

/***** Functions *****/
#if 0
void I2C0_IRQHandler(void)
{
    I2C_Handler(I2C_DEVICE);
    return;
}
#endif
// *****************************************************************************
int main(void)
{
    int count = 0;

    printf("Hello World!\n");

    //Setup the I2CM
    I2C_Shutdown(I2C_DEVICE);
    I2C_Init(I2C_DEVICE, I2C_FAST_MODE, NULL);
 #if 0
    NVIC_EnableIRQ(I2C0_IRQn); // Not sure if we actually need this when not doing async requests
 #endif

    uint8_t dummy[1] = {0};
    // "7-bit addresses 0b0000xxx and 0b1111xxx are reserved"
    for (int addr = 0x8; addr < 0x78; ++addr) {
        // A 0 byte write does not seem to work so always send a single byte.
        int res = I2C_MasterWrite(I2C_DEVICE, addr << 1, dummy, 1, 0);
        if(res == 1) {
            printf("Found (7 bit) address 0x%02x\n", addr);
        }
    }

    while (1) {
        LED_On(0);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);
        LED_Off(0);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);
        printf("count = %d\n", count++);
    }
}
