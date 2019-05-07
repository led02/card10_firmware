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
#include <stdlib.h>
#include "mxc_config.h"
#include "led.h"
#include "board.h"
#include "tmr_utils.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "MAX30003.h"
#include "oled96.h"
#include "stdbool.h"

/***** Definitions *****/

#define I2C_DEVICE	    MXC_I2C0_BUS0

#define SPI SPI0
#define SPI_SPEED       10000000  // Bit Rate


/***** Globals *****/

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

void ecg_write_reg(uint8_t reg, uint32_t data)
{
    printf("write %02x %06x\n", reg, data);
    spi_req_t req;
    uint8_t tx_data[] = {(reg << 1) | 0 , data >> 16, (data >> 8 ) & 0xFF, data & 0xFF};
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
}

void ecg_config(void)
{
    // Reset ECG to clear registers
    ecg_write_reg(SW_RST , 0);

    // General config register setting
    union GeneralConfiguration_u CNFG_GEN_r;
    CNFG_GEN_r.bits.en_ecg = 1;     // Enable ECG channel
    CNFG_GEN_r.bits.rbiasn = 1;     // Enable resistive bias on negative input
    CNFG_GEN_r.bits.rbiasp = 1;     // Enable resistive bias on positive input
    CNFG_GEN_r.bits.en_rbias = 1;   // Enable resistive bias
    CNFG_GEN_r.bits.imag = 2;       // Current magnitude = 10nA
    CNFG_GEN_r.bits.en_dcloff = 1;  // Enable DC lead-off detection
    ecg_write_reg(CNFG_GEN , CNFG_GEN_r.all);


    // ECG Config register setting
    union ECGConfiguration_u CNFG_ECG_r;
    CNFG_ECG_r.bits.dlpf = 1;       // Digital LPF cutoff = 40Hz
    CNFG_ECG_r.bits.dhpf = 1;       // Digital HPF cutoff = 0.5Hz
    //CNFG_ECG_r.bits.gain = 3;       // ECG gain = 160V/V
    CNFG_ECG_r.bits.gain = 0;
    CNFG_ECG_r.bits.rate = 2;       // Sample rate = 128 sps
    ecg_write_reg(CNFG_ECG , CNFG_ECG_r.all);


    //R-to-R configuration
    union RtoR1Configuration_u CNFG_RTOR_r;
    CNFG_RTOR_r.bits.en_rtor = 1;           // Enable R-to-R detection
    ecg_write_reg(CNFG_RTOR1 , CNFG_RTOR_r.all);


    //Manage interrupts register setting
    union ManageInterrupts_u MNG_INT_r;
    MNG_INT_r.bits.efit = 0b00011;          // Assert EINT w/ 4 unread samples
    MNG_INT_r.bits.clr_rrint = 0b01;        // Clear R-to-R on RTOR reg. read back
    ecg_write_reg(MNGR_INT , MNG_INT_r.all);


    //Enable interrupts register setting
    union EnableInterrupts_u EN_INT_r;
    EN_INT_r.all = 0;
    EN_INT_r.bits.en_eint = 1;              // Enable EINT interrupt
    EN_INT_r.bits.en_rrint = 0;             // Disable R-to-R interrupt
    EN_INT_r.bits.intb_type = 3;            // Open-drain NMOS with internal pullup
    ecg_write_reg(EN_INT , EN_INT_r.all);


    //Dyanmic modes config
    union ManageDynamicModes_u MNG_DYN_r;
    MNG_DYN_r.bits.fast = 0;                // Fast recovery mode disabled
    ecg_write_reg(MNGR_DYN , MNG_DYN_r.all);

    // MUX Config
    union MuxConfiguration_u CNFG_MUX_r;
    CNFG_MUX_r.bits.openn = 0;          // Connect ECGN to AFE channel
    CNFG_MUX_r.bits.openp = 0;          // Connect ECGP to AFE channel
    ecg_write_reg(CNFG_EMUX , CNFG_MUX_r.all);

    return;
}

uint8_t content[1024];

void clear(void)
{
    memset(content, 0x00, 1024);
}


void set(uint8_t index, int8_t val)
{
    uint8_t *p = &content[index];

    if(val < -31) val = -31;
    if(val > 32) val = 32;

    int8_t pos = -val + 32;
    p += (pos / 8) * 128;

    *p |= (1 << (pos % 8));
}

int16_t samples[256];

void update(void)
{
    clear();
    int16_t scale = 0;
    for(int i=0; i<256; i++) {
        if(abs(samples[i]) > scale) {
            scale = abs(samples[i]);
        }
    }

    scale /= 32;

    for(int i=0; i<128; i++) {
        set(i, ((samples[i*2] + samples[i*2 + 1]) / scale) / 2);
    }

    oledset(content);
}

uint8_t sample_count = 0;

void add_sample(int16_t sample)
{
    memmove(samples, samples + 1, sizeof(*samples) * 255);
    samples[255] = sample;
    sample_count++;

    if(sample_count == 5) {
        update();
        sample_count = 0;
    }
}

volatile bool ecgFIFOIntFlag = false;
void ecgFIFO_callback(void *data) {
    ecgFIFOIntFlag = true;
}

// *****************************************************************************
int main(void)
{
    //int count = 0;

    printf("Hello World!\n");
    TMR_Delay(MXC_TMR0, MSEC(1000), 0);

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

    oledInit(0x3c, 0, 0);
    oledFill(0x00);
    oledWriteString(0, 0, " card10", 1);

    TMR_Delay(MXC_TMR0, MSEC(1500), 0);

    // Enable 32 kHz output
    //RTC_SquareWave(MXC_RTC, SQUARE_WAVE_ENABLED, F_32KHZ, NOISE_IMMUNE_MODE, NULL);

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

    ecg_config();

    for(int i=0; i<0x20; i++) {
        uint32_t val = ecg_read_reg(i);
        printf("%02x: 0x%06x\n", i, val);
    }

    ecg_write_reg(SYNCH, 0);

    uint32_t ecgFIFO, readECGSamples, idx, ETAG[32], status;
    int16_t ecgSample[32];
    const int EINT_STATUS_MASK =  1 << 23;
    const int FIFO_OVF_MASK =  0x7;
    const int FIFO_VALID_SAMPLE_MASK =  0x0;
    const int FIFO_FAST_SAMPLE_MASK =  0x1;
    const int ETAG_BITS_MASK = 0x7;

    const gpio_cfg_t interrupt_pin = {PORT_1, PIN_12, GPIO_FUNC_IN, GPIO_PAD_PULL_UP};
    GPIO_Config(&interrupt_pin);
    GPIO_RegisterCallback(&interrupt_pin, ecgFIFO_callback, NULL);
    GPIO_IntConfig(&interrupt_pin, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(&interrupt_pin);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(PORT_1));


    while(1) {

#if 1
        // Read back ECG samples from the FIFO
        if( ecgFIFOIntFlag ) {
            ecgFIFOIntFlag = false;

            //printf("Int\n");
            status = ecg_read_reg( STATUS );      // Read the STATUS register

            // Check if EINT interrupt asserted
            if ( ( status & EINT_STATUS_MASK ) == EINT_STATUS_MASK ) {

                readECGSamples = 0;                        // Reset sample counter

                do {
                    ecgFIFO = ecg_read_reg(ECG_FIFO );       // Read FIFO
                    ecgSample[readECGSamples] = ecgFIFO >> 8;                  // Isolate voltage data
                    ETAG[readECGSamples] = ( ecgFIFO >> 3 ) & ETAG_BITS_MASK;  // Isolate ETAG
                    readECGSamples++;                                          // Increment sample counter

                // Check that sample is not last sample in FIFO
                } while ( ETAG[readECGSamples-1] == FIFO_VALID_SAMPLE_MASK ||
                          ETAG[readECGSamples-1] == FIFO_FAST_SAMPLE_MASK );

                // Check if FIFO has overflowed
                if( ETAG[readECGSamples - 1] == FIFO_OVF_MASK ){
                    ecg_write_reg(FIFO_RST , 0); // Reset FIFO
                    //printf("OV\n");
                    LED_On(0);
                    //rLed = 1;//notifies the user that an over flow occured
                }

                // Print results
                for( idx = 0; idx < readECGSamples; idx++ ) {
                    //printf("%6d\r\n", ecgSample[idx]);
                    add_sample(ecgSample[idx]);
                }

            }
        }
#endif
    }
}
