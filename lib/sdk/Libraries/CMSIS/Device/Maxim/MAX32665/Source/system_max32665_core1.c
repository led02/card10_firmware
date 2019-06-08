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
 * $Date: 2018-11-07 19:23:01 +0000 (Wed, 07 Nov 2018) $
 * $Revision: 39007 $
 *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "max32665.h"
#include "mxc_sys.h"
#include "gcr_regs.h"
#include "icc_regs.h"
#include "pwrseq_regs.h"

// Backup mode entry point
extern void Reset_Handler(void);

extern void (* const __isr_vector[])(void);

// Part defaults to HIRC/2 out of reset
uint32_t SystemCoreClock = HIRC_FREQ >> 1; 

__weak void SystemCoreClockUpdate(void)
{
    uint32_t base_freq, div, clk_src;

    // Determine the clock source and frequency
    clk_src = (MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL);
    switch (clk_src)
    {
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC:
            base_freq = HIRC_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_XTAL32M:
            base_freq = XTAL32M_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_LIRC8:
            base_freq = LIRC8_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC96:
            base_freq = HIRC96_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC8:
            base_freq = HIRC8_FREQ;
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_XTAL32K:
            base_freq = XTAL32K_FREQ;
            break;
        default:
	    // Values 001 and 111 are reserved, and should never be encountered.
	    base_freq = HIRC_FREQ;
            break;
    }
    // Clock divider is retrieved to compute system clock
    div = (MXC_GCR->clkcn & MXC_F_GCR_CLKCN_PSC) >> MXC_F_GCR_CLKCN_PSC_POS;

    SystemCoreClock = base_freq >> div;
}

/* This function is called before C runtime initialization and can be
 * implemented by the application for early initializations. If a value other
 * than '0' is returned, the C runtime initialization will be skipped.
 *
 * You may over-ride this function in your program by defining a custom 
 *  PreInit(), but care should be taken to reproduce the initialization steps
 *  or a non-functional system may result.
 */
__weak int PreInit(void)
{
    // Do nothing
    return 0;
}

// This function can be implemented by the application to initialize the board
__weak int Board_Init(void)
{
    // Do nothing
    return 0;
}

/* This function is called just before control is transferred to main().
 *
 * You may over-ride this function in your program by defining a custom 
 *  SystemInit(), but care should be taken to reproduce the initialization
 *  steps or a non-functional system may result.
 */
__weak void SystemInit(void)
{
    /* Configure the interrupt controller to use the application vector 
     * table in flash. Initially, VTOR points to the ROM's table.
     */
    SCB->VTOR = (unsigned long)&__isr_vector;

    /* Enable FPU on Cortex-M4, which occupies coprocessor slots 10 & 11
     * Grant full access, per "Table B3-24 CPACR bit assignments".
     * DDI0403D "ARMv7-M Architecture Reference Manual"
     */
    SCB->CPACR |= SCB_CPACR_CP10_Msk | SCB_CPACR_CP11_Msk;
    __DSB();
    __ISB();

    // Enable ICache1 Clock
    MXC_GCR->perckcn1 &= ~(1 << 22);

    // Invalidate cache and wait until ready
    MXC_ICC1->invalidate = 1;
    while (!(MXC_ICC1->cache_ctrl & MXC_F_ICC_CACHE_CTRL_CACHE_RDY));

    // Enable Cache
    MXC_ICC1->cache_ctrl |= MXC_F_ICC_CACHE_CTRL_CACHE_EN;

    SystemCoreClockUpdate();

    // No board init on core1
    //Board_Init();
}
