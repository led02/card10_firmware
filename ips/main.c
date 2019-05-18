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
#include "tmr_utils.h"
#include "gpio.h"
#include "GUI_DEV/GUI_Paint.h"
#include "Fonts/fonts.h"
#include "image/image.h"
#include "tmr.h"

#include "card10.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>


// *****************************************************************************
int main(void)
{
    int count = 0;

    card10_init();
    card10_diag();

    Paint_DrawString_EN(0, 0, "123", &Font24, 0x000f, 0xfff0);
    Paint_DrawString_EN(0, 23, "ABC", &Font24, BLUE, CYAN);
    Paint_DrawString_CN(20,42, "Î¢Ñ©µç×Ó",  &Font24CN, WHITE, RED);
    Paint_DrawRectangle(70, 10, 100, 40, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2 );
    Paint_DrawLine(70, 10, 100, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
    Paint_DrawLine(100, 10, 70, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
    Paint_DrawCircle(85,25, 22, GREEN, DRAW_FILL_EMPTY, DOT_PIXEL_2X2);

    Paint_DrawImage(gImage_40X40,120, 0,40, 40);
    DEV_Delay_ms(10000);
    Paint_DrawImage(gImage_160X80,0, 0, 160, 80);

    while (1) {
        TMR_Delay(MXC_TMR0, MSEC(1000), 0);
        printf("count = %d\n", count++);
    }
}
