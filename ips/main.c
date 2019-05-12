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
#include <string.h>
#include "mxc_config.h"
#include "led.h"
#include "board.h"
#include "tmr_utils.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "gpio.h"
#include "oled96.h"
#include "bhy.h"
#include "Bosch_PCB_7183_di03_BMI160_BMM150-7183_di03.2.1.11696_170103.h"
#include "bhy_uc_driver.h"
#include "pmic.h"
#include "bme680.h"
#include "bma400.h"
#include "bosch.h"
#include "LCD/LCD_Driver.h"
#include "GUI_DEV/GUI_Paint.h"
#include "Fonts/fonts.h"
#include "image/image.h"


/***** Definitions *****/

#define I2C_DEVICE        MXC_I2C0_BUS0

#define SPI SPI0
#define SPI_SPEED       1000000  // Bit Rate

/***** Globals *****/
static const gpio_cfg_t motor_pin = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};
const gpio_cfg_t DEV_RST_PIN = {PORT_1, PIN_6, GPIO_FUNC_OUT, GPIO_PAD_NONE};
const gpio_cfg_t DEV_DC_PIN = {PORT_1, PIN_7, GPIO_FUNC_OUT, GPIO_PAD_NONE};

/***** Functions *****/

// *****************************************************************************
int main(void)
{
    int count = 0;

    printf("Hello World!\n");

#if 0
    //Setup the I2CM
    I2C_Shutdown(MXC_I2C0_BUS0);
    I2C_Init(MXC_I2C0_BUS0, I2C_FAST_MODE, NULL);

    I2C_Shutdown(MXC_I2C1_BUS0);
    I2C_Init(MXC_I2C1_BUS0, I2C_FAST_MODE, NULL);

    pmic_init();
    pmic_set_led(0, 0);
    pmic_set_led(1, 0);
    pmic_set_led(2, 0);
    TMR_Delay(MXC_TMR0, MSEC(1000), 0);
#endif

    // Enable SPI
    sys_cfg_spi_t spi17y_master_cfg;

    spi17y_master_cfg.map = MAP_A;
    spi17y_master_cfg.ss0 = Enable;
    spi17y_master_cfg.ss1 = Disable;
    spi17y_master_cfg.ss2 = Disable;

    if (SPI_Init(SPI, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
        printf("Error configuring SPI\n");
        while (1);
    }

    GPIO_Config(&DEV_RST_PIN);
    GPIO_Config(&DEV_DC_PIN);

    LCD_SetBacklight(500);
    LCD_Init();
    LCD_Clear(BLACK);

    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT,0,WHITE);
    Paint_Clear(WHITE);
    //Paint_SetRotate(180);
    Paint_DrawString_EN(0, 0, "123", &Font24, 0x000f, 0xfff0);
    Paint_DrawString_EN(0, 23, "ABC", &Font24, BLUE, CYAN);
    Paint_DrawString_CN(20,42, "Î¢Ñ©µç×Ó",  &Font24CN, WHITE, RED);
    Paint_DrawRectangle(70, 10, 100, 40, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2 );
    Paint_DrawLine(70, 10, 100, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
    Paint_DrawLine(100, 10, 70, 40, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
    Paint_DrawCircle(85,25, 22, GREEN, DRAW_FILL_EMPTY, DOT_PIXEL_2X2);

    Paint_DrawImage(gImage_40X40,120, 0,40, 40);
    DEV_Delay_ms(2000);
    Paint_DrawImage(gImage_160X80,0, 0, 160, 80);

    while (1) {
        TMR_Delay(MXC_TMR0, MSEC(1000), 0);
        printf("count = %d\n", count++);
    }
}
