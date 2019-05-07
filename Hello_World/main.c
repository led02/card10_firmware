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
#include "MAX77650-Arduino-Library.h"

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

void pmic_init(void)
{
    uint8_t didm = MAX77650_getDIDM();
    uint8_t cid = MAX77650_getChipID();
    printf("MAX7765x DIDM: 0x%02x CID: 0x%02x\n", didm, cid);

    //MAX77650_setIP_SBB0(0b11);  //Limit output of SBB0 to 500mA
    MAX77650_setTV_SBB0(0b101000); //Set output Voltage of SBB0 to 1.8V
    MAX77650_setADE_SBB0(0b0); //Disable Active Discharge at SBB0 Output
    MAX77650_setEN_SBB0(0b110); //Enable SBB0 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

    //MAX77650_setIP_SBB1(0b11);  //Limit output of SBB1 to 500mA
#if BOARD_EVKIT
    MAX77650_setTV_SBB1(0b100000); //Set output Voltage of SBB1 to 1.2V
#else
    MAX77650_setTV_SBB1(0b001001); //Set output Voltage of SBB1 to 3.3V
#endif
    MAX77650_setADE_SBB1(0b0); //Disable Active Discharge at SBB1 Output
    MAX77650_setEN_SBB1(0b110); //Enable SBB1 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

    //MAX77650_setIP_SBB2(0b11);  //Limit output of SBB2 to 500mA
#if BOARD_EVKIT
    MAX77650_setTV_SBB2(0b110010); //Set output Voltage of SBB2 to 3.3V
#else
    MAX77650_setTV_SBB2(0b110100); //Set output Voltage of SBB2 to 5.0V
#endif
    MAX77650_setADE_SBB2(0b0); //Disable Active Discharge at SBB2 Output
    MAX77650_setEN_SBB2(0b110); //Enable SBB2 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states


    // Prepare the PMIC LEDs
    MAX77650_setLED_FS0(0b01);
    MAX77650_setINV_LED0(false);    //LED red: phase operation
    MAX77650_setBRT_LED0(0b00000);  //LED red: brightness
    MAX77650_setP_LED0(0b1111);     //LED red: LED period
    MAX77650_setD_LED0(0b1111);     //LED red: LED duty-cycle

    MAX77650_setLED_FS1(0b01);
    MAX77650_setINV_LED1(false);    //LED green: phase operation
    MAX77650_setBRT_LED1(0b00000);  //LED green: brightness
    MAX77650_setP_LED1(0b1111);     //LED green: LED period
    MAX77650_setD_LED1(0b1111);     //LED green: LED duty-cycle

    MAX77650_setLED_FS2(0b01);
    MAX77650_setINV_LED2(false);    //LED blue: phase operation
    MAX77650_setBRT_LED2(0b00000);  //LED blue: brightness
    MAX77650_setP_LED2(0b1111);     //LED blue: LED period
    MAX77650_setD_LED2(0b1111);     //LED blue: LED duty-cycle

    MAX77650_setEN_LED_MSTR(true);  //LEDs Master enable
}

void set_led(uint8_t led, uint8_t val)
{
    if(led == 0) {
        MAX77650_setLED_FS0(val > 0 ? 0b01 : 0);
        MAX77650_setBRT_LED0(val);
    }
    if(led == 1) {
        MAX77650_setLED_FS1(val > 0 ? 0b01 : 0);
        MAX77650_setBRT_LED1(val);
    }
    if(led == 2) {
        MAX77650_setLED_FS2(val > 0 ? 0b01 : 0);
        MAX77650_setBRT_LED2(val);
    }

}

// *****************************************************************************
int main(void)
{
    int count = 0;

    printf("Hello World!\n");

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
        set_led(0, 33);
        set_led(1, 0);
        set_led(2, 0);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);

        set_led(0, 0);
        set_led(1, 33);
        set_led(2, 0);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);

        set_led(0, 0);
        set_led(1, 0);
        set_led(2, 33);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);

        set_led(0, 0);
        set_led(1, 0);
        set_led(2, 0);
        GPIO_OutSet(&motor_pin);
        TMR_Delay(MXC_TMR0, MSEC(500), 0);
        GPIO_OutClr(&motor_pin);
        printf("count = %d\n", count++);
    }
}
