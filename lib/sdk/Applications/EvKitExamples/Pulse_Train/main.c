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
 * $Date: 2018-08-13 21:05:24 +0000 (Mon, 13 Aug 2018) $
 * $Revision: 36926 $
 *
 ******************************************************************************/

/**
 * @file main.c
 * @brief Configures and starts four different pulse trains on GPIO LEDs.
 * @details LED0 - PT11 setup as 2Hz continuous signal that outputs 10110b
 *          LED1 - PT12 setup as 10Hz continuous square wave
 *          SW1 - push button setup to stop/start pulse trains
 *
 * @note Interrupts for pulse trains are enabled but the interrupt handler only clears the flags.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "board.h"
#include "pb.h"
#include "pt.h"

/***** Definitions *****/
#define    ALL_PT    0xFFFF

/***** Globals *****/
#define PTG MXC_PTG_BUS0
/***** Functions *****/

// *****************************************************************************
static void PB_Start_Stop_handler()
{
    uint32_t enablePTMask;
    
    //Check if any pulse trains are running
    if (PT_IsActiveMulti(PTG, ALL_PT)) {
        //stop all pulse trains
        PT_StopMulti(PTG, ALL_PT);
    } else {
        //start PT8, PT9 an PT10
        enablePTMask = ENABLE_PT14 | ENABLE_PT15;
        PT_StartMulti(PTG, enablePTMask);
        PT_Start(MXC_PT14_BUS0);
        PT_Start(MXC_PT15_BUS0);
    }
}

// *****************************************************************************
void PT_IRQHandler(void)
{
    printf("flags = 0x%08x\n", PT_GetFlags(PTG));
    
    PT_ClearFlags(PTG, ALL_PT);
}

// *****************************************************************************
void ContinuousPulseTrain(void)
{
    //Setup GPIO to PT output function
    //GPIO P1.14 uses PT14
    
    //setup PT configuration
    pt_pt_cfg_t ptConfig;
    ptConfig.bps = 2;           //bit rate
    ptConfig.ptLength = 5;      //bits
    ptConfig.pattern = 0x16;
    ptConfig.loop = 0;          //continuous loop
    ptConfig.loopDelay = 0;
    
    PT_PTConfig(MXC_PT14_BUS0, &ptConfig, &gpio_cfg_pt14);
    
    //start PT11
    PT_Start(MXC_PT14_BUS0);
}

// *****************************************************************************
void SquareWave(void)
{
    //Setup GPIO to PT output function
    //GPIO P1.15 uses PT15
    
    uint32_t freq = 10;//Hz
    PT_SqrWaveConfig(MXC_PT15_BUS0, freq, &gpio_cfg_pt15);
    
    //start PT12
    PT_Start(MXC_PT15_BUS0);
}

// *****************************************************************************
int main(void)
{

    printf("\n*************** Pulse Train Demo ****************\n");
    printf("LED0 = Outputs continuous pattern of 10110b at 2bps\n");
    printf("LED1 = Outputs 10Hz continuous square wave\n");
    printf("SW2  = Stop/Start all pulse trains\n");
    
    //Setup push button to start/stop All pulse train
    PB_RegisterCallback(0, (pb_callback)PB_Start_Stop_handler);
    
    NVIC_EnableIRQ(PT_IRQn);          //enabled default interrupt handler
    PT_EnableINTMulti(PTG, ALL_PT);        //enabled interrupts for all PT
    PT_Init(PTG, NULL);                 //initialize pulse trains
    
    //configure and start pulse trains
    ContinuousPulseTrain();
    SquareWave();
    
    while (1) {}
}
