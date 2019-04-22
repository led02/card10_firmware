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
 * $Date: 2018-09-04 17:13:32 -0500 (Tue, 04 Sep 2018) $
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
#include "mcr_regs.h"

/***** Definitions *****/

/***** Globals *****/
volatile uint32_t badData;
volatile uint32_t eccFlag;
volatile uint32_t eccErr;
volatile uint32_t eccDErr;
volatile uint32_t eccAddr;


/***** Functions *****/
void ECC_IRQHandler(void) 
{
    eccErr = MXC_GCR->eccerr;
    eccDErr = MXC_GCR->eccnded;
    eccAddr = MXC_GCR->eccerrad;
	eccFlag = 1;

    MXC_GCR->eccerr = 0xFFFFFFFF;
    MXC_GCR->eccnded = 0xFFFFFFFF;
}

// *****************************************************************************
int main(void)
{
    printf("------------------------------------------------------\n");
    printf("ECC Example\n");
    printf("This example will corrupt a word of data\n");
    printf("and ensure that the ECC throws an error when it's read\n");
    
    // Clear all ECC Errors
    MXC_GCR->eccerr = 0xFFFFFFFF;
    MXC_GCR->eccnded = 0xFFFFFFFF;

	// Enable SysRAM ECC and interrupts
    MXC_MCR->eccen |= 1;
    MXC_GCR->eccirqen |= 0x3F;
    NVIC_EnableIRQ(ECC_IRQn);

    badData = 0xDEADBEEF;
    printf("--------\nData before Corruption: 0x%08x\n", badData);
    printf("Address of data: 0x%08x\n", &badData);

    // Disable IRQ and Corrupt Data
    MXC_MCR->eccen &= ~1;
    badData = 0xDEADBEEE;
    MXC_MCR->eccen |= 1;

    printf("--------\nData after 1bit error: 0x%08x\n", badData);
    printf("ECC Error:              0x%08x\n", eccErr);
    printf("ECC Not Double Error:   0x%08x\n", eccDErr);
    printf("ECC Error Address:      0x%08x\n", eccAddr);
    if(eccFlag) {
    	printf("An ECC Error was found!\n");
    	eccFlag = 0;
    }
    
    MXC_MCR->eccen &= ~1;
    badData = 0xDEADBEEC;
    MXC_MCR->eccen |= 1;

    printf("--------\nData after 2 bit error: 0x%08x\n", badData);
    printf("ECC Error:              0x%08x\n", eccErr);
    printf("ECC Not Double Error:   0x%08x\n", eccDErr);
    printf("ECC Error Address:      0x%08x\n", eccAddr);
    if(eccFlag) {
    	printf("An ECC Error was found!\n");
    	eccFlag = 0;
    }

    printf("Example Complete\n");
    while(1);
}
