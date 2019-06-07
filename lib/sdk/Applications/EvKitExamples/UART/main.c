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
 *******************************************************************************
 */

/**
 * @file    main.c
 * @brief   Main for UART example.
 * @details This example loops back the TX to the RX on UART1. For this example
 *          you must connect a jumper across P0.20 to P0.21. UART_BAUD and the BUFF_SIZE
 *          can be changed in this example.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_sys.h"
#include "uart.h"
#include "board.h"
#include "pb.h"

/***** Definitions *****/
#define UART_BAUD           1800000
#define BUFF_SIZE           512

/***** Globals *****/
volatile int read_flag;
volatile int write_flag;
volatile int buttonPressed;

/***** Functions *****/

/******************************************************************************/
void read_cb(uart_req_t* req, int error)
{
    read_flag = error;
}

/******************************************************************************/
void write_cb(uart_req_t* req, int error)
{
    write_flag = error;
}

/******************************************************************************/
void UART2_IRQHandler(void)
{
    UART_Handler(MXC_UART2);
}

void buttonHandler()
{
    buttonPressed = 1;
}

/******************************************************************************/
int main(void)
{
    int error, i, fail = 0;
    uint8_t txdata[BUFF_SIZE];
    uint8_t rxdata[BUFF_SIZE];
    sys_cfg_uart_t sys_cfg;
    printf("\n\n***** UART Loopback Example *****\n");
    printf("This example sends and receives data in a\n");
    printf("loopback configuration using a single UART peripheral\n");
    printf("\nYou will need to connect RX2 and TX2,\n");
    printf("located to the right of the LCD\n\n");
    printf("Push SW2 to continue\n\n");
    
    buttonPressed = 0;
    PB_RegisterCallback(0, (pb_callback)buttonHandler);
    while(!buttonPressed);

    printf("UART Baud \t: %d Hz\n", UART_BAUD);
    printf("Test Length \t: %d bytes\n\n", BUFF_SIZE);

    // Initialize the data buffers
    for(i = 0; i < BUFF_SIZE; i++) {
        txdata[i] = i;
    }
    memset(rxdata, 0x0, BUFF_SIZE);

    // Setup the interrupt
    NVIC_ClearPendingIRQ(MXC_UART_GET_IRQ(2));
    NVIC_DisableIRQ(MXC_UART_GET_IRQ(2));
    NVIC_SetPriority(MXC_UART_GET_IRQ(2), 1);
    NVIC_EnableIRQ(MXC_UART_GET_IRQ(2));

    // Initialize the UART
    uart_cfg_t cfg;
    cfg.parity = UART_PARITY_DISABLE;
    cfg.size = UART_DATA_SIZE_8_BITS;
    cfg.stop = UART_STOP_1;
    cfg.flow = UART_FLOW_CTRL_DIS;
    cfg.pol = UART_FLOW_POL_DIS;
    cfg.baud = UART_BAUD;
    cfg.clksel = UART_CLKSEL_SYSTEM;
    sys_cfg.map = MAP_B;
    sys_cfg.flow = Disable; 
        
    error = UART_Init(MXC_UART2, &cfg, &sys_cfg);

    if(error != E_NO_ERROR) {
        printf("Error initializing UART %d\n", error);
        printf("Example Failed\n");
        while(1) {}
    } else {
        printf("UART Initialized\n");
    }

    // Setup the asynchronous requests
    uart_req_t read_req;
    read_req.data = rxdata;
    read_req.len = BUFF_SIZE;
    read_req.callback = read_cb;

    uart_req_t write_req;
    write_req.data = txdata;
    write_req.len = BUFF_SIZE;
    write_req.callback = write_cb;

    read_flag = 1;
    write_flag = 1;

    error = UART_ReadAsync(MXC_UART2, &read_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        printf("Example Failed\n");
        while(1) {}
    }

    error = UART_WriteAsync(MXC_UART2, &write_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async write %d\n", error);
        printf("Example Failed\n");
        while(1) {}
    }

    while(write_flag == 1) {}
    if(write_flag != E_NO_ERROR) {
        printf("Error with UART_WriteAsync callback\n");
        fail++;
    }

    while(read_flag == 1) {}
    if(read_flag != E_NO_ERROR) {
        printf("Error with UART_ReadAsync callback %d\n", read_flag);
        fail++;
    }

    if((error = memcmp(rxdata, txdata, BUFF_SIZE)) != 0) {
        printf("Error verifying Data %d\n", error);
        fail++;
    } else {
        printf("Data verified\n");
    }

    printf("\n");
    if(fail == 0) {
        printf("Example Succeeded\n");
    } else {
        printf("Example Failed\n");
    }
    while(1) {}
}
