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
    
    // Switch to fast clock on startup
    MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_RDY));
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
    
    // Wait for clock switch and disable unused clocks
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC_EN);

    /* Enable FPU on Cortex-M4, which occupies coprocessor slots 10 & 11
     * Grant full access, per "Table B3-24 CPACR bit assignments".
     * DDI0403D "ARMv7-M Architecture Reference Manual"
     */
    SCB->CPACR |= SCB_CPACR_CP10_Msk | SCB_CPACR_CP11_Msk;
    __DSB();
    __ISB();

    // Initialize backup mode entry point to safe default value.
    MXC_PWRSEQ->gp0 = (uint32_t)(Reset_Handler) | 1;

    // FIXME Pre-production parts: Enable TME, disable ICache Read Buffer, disable TME
    *(uint32_t *)0x40000c00 = 1;
    *(uint32_t *)0x4000040c = (1<<6);
    *(uint32_t *)0x40000c00 = 0;

    // Flush and enable instruction cache
    MXC_ICC0->invalidate = 1;
    while (!(MXC_ICC0->cache_ctrl & MXC_F_ICC_CACHE_CTRL_CACHE_RDY));
    MXC_ICC0->cache_ctrl |= MXC_F_ICC_CACHE_CTRL_CACHE_EN;
    while (!(MXC_ICC0->cache_ctrl & MXC_F_ICC_CACHE_CTRL_CACHE_RDY));

    SystemCoreClockUpdate();

    // Set all GPIO to 25K pullup mode by default
    SYS_ClockEnable(SYS_PERIPH_CLOCK_GPIO0); 
    SYS_ClockEnable(SYS_PERIPH_CLOCK_GPIO1); 

    // All GPIO on port 0 to 1.8 V first
    MXC_GPIO0->vssel = 0;
    MXC_GPIO0->vssel |= (1UL << 0) | (1UL << 1) | (1UL << 2) | (1UL << 3) | (1UL << 4) | (1UL << 5); // Flash
    MXC_GPIO0->vssel |= (1UL << 6) | ( 1UL << 7); // USB I2C
    MXC_GPIO0->vssel |= (1UL << 8); // Motor FET
    MXC_GPIO0->vssel |= (1UL << 9) | (1UL << 10); // USB UART
    MXC_GPIO0->vssel |= (1UL << 20) | (1UL << 21) | (1UL << 22) | (1UL << 29); // Wristband
    MXC_GPIO0->vssel |= (1UL << 17) | (1UL << 23) | (1UL << 28); // GPIO to TOP
    MXC_GPIO0->vssel |= (1UL << 31); // ECG Switch

    MXC_GPIO0->ps |= 0xFFFFFFFF;
    MXC_GPIO0->pad_cfg1 |= 0xFFFFFFFF;
    MXC_GPIO0->pad_cfg2 &= ~(0xFFFFFFFF);

    // All GPIO on port 1 to 1.8 V first
    MXC_GPIO1->vssel = 0;
    MXC_GPIO1->vssel |= (1UL << 0) | (1UL << 1) | (1UL << 2) | (1UL << 3) | (1UL << 4) | (1UL << 5); // SDHC
    MXC_GPIO1->vssel |= (1 << 6) | (1 << 7); // GPIO to TOP
    MXC_GPIO1->vssel |= (1 << 14) | (1 << 15); // GPIO for RGB LEDs

    MXC_GPIO1->vssel |= (1 << 8) | (1 << 9) |  (1 << 10) | (1 << 11); // TODO: TMP for devboard

    MXC_GPIO1->ps |= 0xFFFFFFFF;
    MXC_GPIO1->pad_cfg1 |= 0xFFFFFFFF;
    MXC_GPIO1->pad_cfg2 &= ~(0xFFFFFFFF);

    Board_Init();
}

#if defined ( __CC_ARM )
/* Global variable initialization does not occur until post scatterload in Keil tools.*/

/* External function called after our post scatterload function implementation. */
extern void $Super$$__main_after_scatterload(void);

/**
 * @brief   Initialization function for SystemCoreClock and Board_Init.
 * @details $Sub$$__main_after_scatterload is called during system startup in the Keil
 *          toolset. Global variable and static variable space must be set up by the compiler
 *          prior to using these memory spaces. Setting up the SystemCoreClock and Board_Init
 *          require global memory for variable storage and are called from this function in
 *          the Keil tool chain.
 */
void $Sub$$__main_after_scatterload(void)
{
    SystemInit();
    $Super$$__main_after_scatterload();
}
#endif /* __CC_ARM */
