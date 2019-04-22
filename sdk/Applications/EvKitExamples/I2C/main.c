/**
 * @file        main.c
 * @brief       I2C Loopback Example 
 * @details     This example uses the I2C Master to read/write from/to the I2C Slave. For
 *              this example you must connect P0.6 to P0.14 (SDA) and P0.7 to P0.15 (SCL). The Master
 *              will use P0.6 and P0.7. The Slave will use P0.14 and P0.15. You must also
 *              connect the pull-up jumpers (JP23 and JP24) to the proper I/O voltage.
 *              Refer to JP27 to determine the I/O voltage.
 * @note        Other devices on the EvKit will be using the same bus. This example cannot be combined with
 *              a PMIC or bluetooth example because the I2C Slave uses GPIO pins for those devices.
 */

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
 * $Date: 2019-02-26 21:50:33 +0000 (Tue, 26 Feb 2019) $
 * $Revision: 41252 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "pb.h"
#include "i2c.h"

/***** Definitions *****/
#define I2C_MASTER	    MXC_I2C0_BUS0

#define I2C_SLAVE	    MXC_I2C1_BUS0
#define I2C_SLAVE_ADDR	(0x51<<1)

/***** Globals *****/
static uint8_t txdata[16];
static uint8_t rxdata[16];

volatile int i2c_flag;
volatile int i2c_flag1;
volatile int buttonPressed;

int fail = 0;

/***** Functions *****/
//Slave interrupt handler
void I2C1_IRQHandler(void) 
{
    I2C_Handler(I2C_SLAVE);
    return;
}

//Master interrupt handler
void I2C0_IRQHandler(void)
{
    I2C_Handler(I2C_MASTER);
    return;
}

void buttonHandler()
{
    buttonPressed = 1;
}

//I2C callback 
void i2c_callback(i2c_req_t *req, int error)
{
    i2c_flag = error;
    return;
}

//Prints out human-friendly format to read txdata and rxdata
void print_data(void)
{
    int i;
    printf("TxData: 0x");
    for(i = 0; i < sizeof(txdata); ++i) {
        printf("%01x", txdata[i]);
    }

    printf("\nRxData: 0x");
    for(i = 0; i < sizeof(rxdata); ++i) {
        printf("%01x", rxdata[i]);
    }

    printf("\n");

    return;
}

//Compare txdata and rxdata to see if they are the same
void verify(void)
{
    int i, fails = 0;
    for(i = 0; i < 16; ++i) {
        if(txdata[i] != rxdata[i]) {
            ++fails;
	}
    }
    if(fails > 0) {
        printf("Fail.\n");
        fail += 1;
    }    
    else {
        printf("Pass.\n");
    }

    return;
}

// *****************************************************************************
int main(void)
{
    printf("\n***** I2C Loopback Example *****\n");
    printf("This example uses one I2C peripheral as a master\n");
    printf("to read and write to another I2C which acts as a slave.\n");
    printf("\nYou will need to connect P0.6 to P0.14 (SDA) and\n");
    printf("P0.7 to P0.15 (SCL).\n");
    printf("Push SW2 to continue\n");

    buttonPressed = 0;
    PB_RegisterCallback(0, (pb_callback)buttonHandler);
    while(!buttonPressed);

    int error, i = 0;

    //Setup the I2CM
    I2C_Shutdown(I2C_MASTER);
    I2C_Init(I2C_MASTER, I2C_FAST_MODE, NULL);
    NVIC_EnableIRQ(I2C0_IRQn);

    //Setup the I2CS
    I2C_Shutdown(I2C_SLAVE);
    I2C_Init(I2C_SLAVE, I2C_FAST_MODE, NULL);
    NVIC_EnableIRQ(I2C1_IRQn);	

    //MASTER WRITE SLAVE READ **********************************************
    printf("\nMaster writes, Slave reads\n");
    //Initialize test data
    for(i = 0; i < 16; i++) {
        txdata[i] = i;
        rxdata[i] = 0;
    }

    //Prepare SlaveAsync
    i2c_req_t req;
    req.addr = I2C_SLAVE_ADDR;
    req.tx_data = txdata;
    req.tx_len = 16;
    req.rx_data = rxdata;
    req.rx_len = 16;
    req.restart = 0;
    req.callback = i2c_callback;

    i2c_flag = 1;
    if((error = I2C_SlaveAsync(I2C_SLAVE, &req)) != E_NO_ERROR) {
        printf("Error starting async read: %d\n", error);
        printf("Example Failed\n");
        while(1);
    }

    if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, txdata, 16, 0)) != 16) {
        printf("Error writing: %d\n", error);
        printf("Example Failed\n");
        while(1);
    }

    //Wait for callback
    while(i2c_flag == 1);

    //Check callback flag
    if(i2c_flag != E_NO_ERROR) {
    printf("Error with i2c_callback: %d\n", i2c_flag);
    }

    printf("Result data:\n");
    print_data();
    verify();

    //Initialize new test data
    for(i = 0; i < 16; i++) {
        txdata[i] = 15-i;
        rxdata[i] = 0;
    }

    //SLAVE WRITE MASTER READ **********************************************
    printf("\nSlave writes, Master reads\n");

    i2c_flag = 1;
    if((error = I2C_SlaveAsync(I2C_SLAVE, &req)) != E_NO_ERROR) {
        printf("Error starting async read: %d\n", error);
        printf("Example Failed\n");
        while(1);
    }

    if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, rxdata, 16, 0)) != 16) {
        printf("Error reading: %d\n", error);
        printf("Example Failed\n");
        while(1);
    }

    //Wait for callback
    while(i2c_flag == 1);

    //Check callback flag
    if(i2c_flag != E_NO_ERROR) {
        printf("Error with i2c_callback: %d\n", i2c_flag);
        printf("Example Failed\n");
        while(1);
    }

    printf("Result data:\n");
    print_data();
    verify();


    printf("\n");
    if(fail == 0) {
        printf("Example Succeeded\n");
    } else {
        printf("Example Failed\n");
    }

    return 0;
}
