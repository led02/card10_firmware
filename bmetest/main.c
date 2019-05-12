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
#include "gpio.h"
#include "oled96.h"
#include "pmic.h"
#include "bme680.h"
#include "bosch.h"

/***** Definitions *****/
// *****************************************************************************
int main(void)
{
    printf("Hello World!\n");
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

    struct bme680_dev gas_sensor;
    gas_sensor.dev_id = BME680_I2C_ADDR_PRIMARY;
    gas_sensor.intf = BME680_I2C_INTF;
    gas_sensor.read = card10_bosch_i2c_read;
    gas_sensor.write = card10_bosch_i2c_write;
    gas_sensor.delay_ms = card10_bosch_delay;
    /* amb_temp can be set to 25 prior to configuring the gas sensor
     * or by performing a few temperature readings without operating the gas sensor.
     */
    gas_sensor.amb_temp = 25;

    int8_t rslt = BME680_OK;
    rslt = bme680_init(&gas_sensor);

    if(rslt != BME680_OK) {
        printf("Failed to init BME680\n");
    }
    uint8_t set_required_settings;

    /* Set the temperature, pressure and humidity settings */
    gas_sensor.tph_sett.os_hum = BME680_OS_2X;
    gas_sensor.tph_sett.os_pres = BME680_OS_4X;
    gas_sensor.tph_sett.os_temp = BME680_OS_8X;
    gas_sensor.tph_sett.filter = BME680_FILTER_SIZE_3;

    /* Set the remaining gas sensor settings and link the heating profile */
    gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    /* Create a ramp heat waveform in 3 steps */
    gas_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
    gas_sensor.gas_sett.heatr_dur = 150; /* milliseconds */

    /* Select the power mode */
    /* Must be set before writing the sensor configuration */
    gas_sensor.power_mode = BME680_FORCED_MODE; 

    /* Set the required sensor settings needed */
    set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL 
        | BME680_GAS_SENSOR_SEL;

    /* Set the desired sensor configuration */
    rslt = bme680_set_sensor_settings(set_required_settings,&gas_sensor);

    /* Set the power mode */
    rslt = bme680_set_sensor_mode(&gas_sensor);


    while (1) {
        TMR_Delay(MXC_TMR0, MSEC(1000), 0);

        struct bme680_field_data data;
        rslt = bme680_get_sensor_data(&data, &gas_sensor);

        printf("T: %.2f degC, P: %.2f hPa, H %.2f %%rH ", data.temperature / 100.0l,
            data.pressure / 100.0l, data.humidity / 1000.0l );

        //printf("%.2f,%.2f,%.2f\n", data.temperature / 100.0f,
        //    data.pressure / 100.0f, data.humidity / 1000.0f );
        /* Avoid using measurements from an unstable heating setup */
        if(data.status & BME680_GASM_VALID_MSK)
            printf(", G: %d ohms", data.gas_resistance);

        printf("\n");

        /* Trigger the next measurement if you would like to read data out continuously */
        if (gas_sensor.power_mode == BME680_FORCED_MODE) {
            rslt = bme680_set_sensor_mode(&gas_sensor);
        }
    }
}
