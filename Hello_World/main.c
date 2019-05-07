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

/***** Definitions *****/

#define I2C_DEVICE	    MXC_I2C0_BUS0

#define SPI SPI0
#define SPI_SPEED       1000000  // Bit Rate

/***** Globals *****/
static const gpio_cfg_t motor_pin = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};

/***** Functions *****/
#if 0
void I2C0_IRQHandler(void)
{
    I2C_Handler(I2C_DEVICE);
    return;
}
#endif

uint32_t ecg_read_reg(uint8_t reg)
{
    spi_req_t req;
    uint8_t tx_data[] = {(reg << 1) | 1, 0, 0, 0};
    uint8_t rx_data[] = {0, 0, 0, 0};
    req.tx_data = tx_data;
    req.rx_data = rx_data;
    req.len = 4;
    req.bits = 8;
    req.width = SPI17Y_WIDTH_1;
    req.ssel = 0;
    req.deass = 1;
    req.ssel_pol = SPI17Y_POL_LOW;
    req.tx_num = 0;
    req.rx_num = 0;

    SPI_MasterTrans(SPI, &req);

    return (rx_data[1] << 16) | (rx_data[2] << 8) | rx_data[3];
}

// *****************************************************************************
int main(void)
{
    int count = 0;

    printf("Hello World!\n");
    TMR_Delay(MXC_TMR0, MSEC(1000), 0);

    //Setup the I2CM
    I2C_Shutdown(MXC_I2C0_BUS0);
    I2C_Init(MXC_I2C0_BUS0, I2C_FAST_MODE, NULL);

    I2C_Shutdown(MXC_I2C1_BUS0);
    I2C_Init(MXC_I2C1_BUS0, I2C_FAST_MODE, NULL);

 #if 0
    NVIC_EnableIRQ(I2C0_IRQn); // Not sure if we actually need this when not doing async requests
 #endif

    uint8_t dummy[1] = {0};
    // "7-bit addresses 0b0000xxx and 0b1111xxx are reserved"
    for (int addr = 0x8; addr < 0x78; ++addr) {
        // A 0 byte write does not seem to work so always send a single byte.
        int res = I2C_MasterWrite(MXC_I2C0_BUS0, addr << 1, dummy, 1, 0);
        if(res == 1) {
            printf("Found (7 bit) address 0x%02x on I2C0\n", addr);
        }

        res = I2C_MasterWrite(MXC_I2C1_BUS0, addr << 1, dummy, 1, 0);
        if(res == 1) {
            printf("Found (7 bit) address 0x%02x on I2C1\n", addr);
        }
    }

    oledInit(0x3c, 0, 0);
    oledFill(0x00);
    oledWriteString(0, 0, "Hello", 0);
    oledWriteString(0, 2, "my name is", 0);
    oledWriteString(0, 4, "card10", 1);

    if(bhy_driver_init(bhy1_fw)) {
        printf("Failed to init bhy\n");
    }


    // Enable 32 kHz output
    RTC_SquareWave(MXC_RTC, SQUARE_WAVE_ENABLED, F_32KHZ, NOISE_IMMUNE_MODE, NULL);

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

    for(int i=0; i<0x20; i++) {
        uint32_t val = ecg_read_reg(i);
        printf("%02x: 0x%06x\n", i, val);
    }

    pmic_init();

    while (1) {
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
        GPIO_OutSet(&motor_pin);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);
        GPIO_OutClr(&motor_pin);
        TMR_Delay(MXC_TMR0, MSEC(1000), 0);
        printf("count = %d\n", count++);
    }
}
