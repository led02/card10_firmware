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
 * $Date: 2018-09-11 18:35:25 +0000 (Tue, 11 Sep 2018) $
 * $Revision: 37794 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   SPIXR example writing to SRAM Microchip 23LC1024
 * @details Setup, Initialize, Write, and verifies the data written to SRAM. 
 *          This example is showing how to configure the SRAM 23LC1024 and 
 *          uses the SPIXR library to write and read data from it in Quad mode. 
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mxc_config.h"
#include "spixr.h"
#include "board.h"
#include "uart.h"

/***** Definitions *****/
// RAM Vendor Specific Commands
#define A1024_READ      0x03
#define A1024_WRITE     0x02
#define A1024_EQIO      0x38

// RAM Vendor Specific Values
#define BUFFER_SIZE     16

#define A1024_ADDRESS   0x80000000
/***** Globals *****/
spixr_cfg_t init_cfg = {
    MXC_V_SPIXR_CTRL1_SS_SS0,        /* Slave select 0                   */
    0x08,                           /* Number of bits per character     */
    SPIXR_QUAD,                      /* SPI Data Width                   */
    0x04,                           /* num of system clocks between SS active & first serial clock edge     */
    0x08,                           /* num of system clocks between last serial clock edge and ss inactive  */
    0x10,                           /* num of system clocks between transactions (read / write)             */
    1,                        /* Baud freq                        */
    3                               /* Baud scale                       */
};
int fail = 0;

/***** Functions *****/
/******************************************************************************/
void setup(void)
{
    uint8_t quad_cmd =  A1024_EQIO; /* pre-defined command to use quad mode         */

    // Enable the SPIXR to talk to RAM 
    sys_cfg_spixr_t cfg;
    cfg.scache_flag = 1;
    cfg.crypto_flag = 0;
    SPIXR_Enable(&cfg);                  /* Enable the SPIXR communication mode           */

    // // Initialize the desired configuration
    if(SPIXR_Config(&init_cfg) == E_NO_ERROR) {   
        // printf("SPIXR was initialized properly.\n");
    }   else {
        Console_Init();
        printf("SPIXR was not initialized properly.\n");
        printf("Example Failed\n");
        while(1);
    }

    // /* Hide this with function in SPIXR.C later */
    MXC_SPIXR->dma &= ~MXC_F_SPIXR_DMA_RX_DMA_EN;
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_TX_FIFO_EN;
    MXC_SPIXR->ctrl3 &= ~MXC_F_SPIXR_CTRL3_DATA_WIDTH;

    // // Setup to communicate in quad mode
    SPIXR_Send_Command (&quad_cmd, 1, 1);
    // // Wait until quad cmd is sent
    while(SPIXR_Busy());

    MXC_SPIXR->ctrl3 &= ~MXC_F_SPIXR_CTRL3_DATA_WIDTH;
    MXC_SPIXR->ctrl3 |= MXC_S_SPIXR_CTRL3_DATA_WIDTH_QUAD;
    MXC_SPIXR->ctrl3 &= ~MXC_F_SPIXR_CTRL3_THREE_WIRE;

    MXC_SPIXR->dma = 0x00;           /* Disable the FIFOs for transparent operation  */
    MXC_SPIXR->xmem_ctrl = (0x01 << MXC_F_SPIXR_XMEM_CTRL_DUMMY_CLK_POS)      |
                          (A1024_READ << MXC_F_SPIXR_XMEM_CTRL_RD_CMD_POS)   |
                          (A1024_WRITE << MXC_F_SPIXR_XMEM_CTRL_WR_CMD_POS)  |
                          MXC_F_SPIXR_XMEM_CTRL_XMEM_EN;
 
    // // Do some sort of checking mechanism to see whether it is being initialized properly or no
    // // Return 1 if success and 0 if not

}

// *****************************************************************************

int main(void)
{
    // Defining Variable(s) to write & store data to RAM
    uint8_t write_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];
    uint8_t *address = (uint8_t*) A1024_ADDRESS;;                            /* Variable to store address of RAM */
    int temp,i;


    printf("\n***** SPIXR Example *****\n");
    printf("This example communicates with an MX25\n");
    printf("SPI ram on the EvKit using Quad SPI mode\n");
    printf("And the SPIXR peripheral\n\n");
    printf("The console will be shutdown during the example\n\n");

    // Configure the SPIXR
    printf("Setting up the SPIXR\n");
    while (UART_Busy(MXC_UART_GET_UART(CONSOLE_UART)));
    fflush(stdout);
    Console_Shutdown();
    setup();

    // Initialize & write pseudo-random data to be written to the RAM
    srand(0);
    for(i = 0; i < BUFFER_SIZE; i++) { 
        temp = rand(); 
        write_buffer[i] = temp;
        // Write the data to the RAM
        *(address + i) = temp; 
    }

    // Read data from RAM 
    for(i = 0; i < BUFFER_SIZE; i++) {
        read_buffer[i] = *(address + i);   
    }

    // Disable the SPIXR 
    SPIXR_Disable();
    Console_Init();

    // Verify data being read from RAM
    if(memcmp(write_buffer, read_buffer, BUFFER_SIZE)) {
        printf("Data is not verified.\n\n");
        fail++;
    } else {
        printf("Data is verified.\n\n");
    }


    printf("\n");
    if(fail == 0) {
        printf("Example Succeeded\n");
    } else {
        printf("Example Failed\n");
    }

    while(1);
}