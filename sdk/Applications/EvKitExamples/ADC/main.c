/**
 * @file    main.c
 * @brief   ADC demo application
 * @details Continuously monitors the ADC channels
 */

/* *****************************************************************************
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
* $Date: 2016-03-11 11:46:02 -0600 (Fri, 11 Mar 2016) $
* $Revision: 21838 $
*
**************************************************************************** */

/* **** Includes **** */
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "led.h"
#include "adc.h"
#include "tmr_utils.h"

/* **** Definitions **** */

/* Use Interrupt Mode */
#define USE_INTERRUPTS

/* Uncomment if using polling mode */
#undef USE_INTERRUPTS

/* **** Globals **** */
#ifdef USE_INTERRUPTS
    volatile unsigned int adc_done = 0;
#endif

/* **** Functions **** */

#ifdef USE_INTERRUPTS
void ADC_IRQHandler(void)
{
    ADC_ClearFlags(MXC_F_ADC_INTR_DONE_IF);
    
    /* Signal bottom half that data is ready */
    adc_done = 1;
    
    return;
}
#endif

int main(void)
{
    uint16_t adc_val[4];
    unsigned int overflow[4];
    uint8_t fmtstr[40];
    const sys_cfg_adc_t sys_adc_cfg = NULL; /* No system specific configuration needed. */
    const sys_cfg_tmr_t sys_tmr_cfg = NULL; /* No system specific configuration needed. */
    
    printf("***** ADC Example *****\n");
    printf("Samples channels 0-3 continuously\n");
    
    /* Initialize ADC */
    if (ADC_Init(0x9, &sys_adc_cfg) != E_NO_ERROR) {
        printf("Error Bad Parameter\n");
        while (1);
    }
    // Configure Inputs as ADC inputs
    GPIO_Config(&gpio_cfg_adc0);
    GPIO_Config(&gpio_cfg_adc1);
    GPIO_Config(&gpio_cfg_adc2);
    GPIO_Config(&gpio_cfg_adc3);

    /* Set up LIMIT0 to monitor high and low trip points */
    ADC_SetLimit(ADC_LIMIT_0, ADC_CH_1, 1, 0x25, 1, 0x300);
    
#ifdef USE_INTERRUPTS
    NVIC_EnableIRQ(ADC_IRQn);
#endif
    
    while (1) {
        /* Flash LED when starting ADC cycle */
        LED_On(0);
        TMR_Delay(MXC_TMR0, MSEC(10), &sys_tmr_cfg);
        LED_Off(0);
        
        /* Convert channel 0 */
#ifdef USE_INTERRUPTS
        adc_done = 0;
        ADC_StartConvert(ADC_CH_0, 0, 0);
        while (!adc_done);
#else
        ADC_StartConvert(ADC_CH_0, 0, 0);
#endif
        overflow[0] = (ADC_GetData(&adc_val[0]) == E_OVERFLOW ? 1 : 0);
        
        /* Convert channel 1 */
#ifdef USE_INTERRUPTS
        adc_done = 0;
        ADC_StartConvert(ADC_CH_1, 0, 0);
        while (!adc_done);
#else
        ADC_StartConvert(ADC_CH_1, 0, 0);
#endif
        overflow[1] = (ADC_GetData(&adc_val[1]) == E_OVERFLOW ? 1 : 0);
        
        /* Convert channel 2 */
#ifdef USE_INTERRUPTS
        adc_done = 0;
        ADC_StartConvert(ADC_CH_2, 0, 0);
        while (!adc_done);
#else
        ADC_StartConvert(ADC_CH_2, 0, 0);
#endif
        overflow[2] = (ADC_GetData(&adc_val[2]) == E_OVERFLOW ? 1 : 0);
        
        /* Convert channel 3 */
#ifdef USE_INTERRUPTS
        adc_done = 0;
        ADC_StartConvert(ADC_CH_3, 0, 0);
        while (!adc_done);
#else
        ADC_StartConvert(ADC_CH_3, 0, 0);
#endif
        overflow[3] = (ADC_GetData(&adc_val[3]) == E_OVERFLOW ? 1 : 0);
        
        /* Display results on OLED display, display asterisk if overflow */
        snprintf((char *)fmtstr, 40, "0: 0x%04x%s 2: 0x%04x%s",
                 adc_val[0], overflow[0] ? "*" : " ",
                 adc_val[2], overflow[2] ? "*" : " ");
        printf("%s\n",fmtstr);
        
        snprintf((char *)fmtstr, 40, "1: 0x%04x%s 3: 0x%04x%s",
                 adc_val[1], overflow[1] ? "*" : " ",
                 adc_val[3], overflow[3] ? "*" : " ");
        printf("%s\n",fmtstr);
        /* Determine if programmable limits on AIN1 were exceeded */
        if (ADC_GetFlags() & (MXC_F_ADC_INTR_LO_LIMIT_IF | MXC_F_ADC_INTR_HI_LIMIT_IF)) {
            snprintf((char *)fmtstr, 40, " %s Limit on AIN1 ",
                     (ADC_GetFlags() & MXC_F_ADC_INTR_LO_LIMIT_IF) ? "Low" : "High");
            ADC_ClearFlags(MXC_F_ADC_INTR_LO_LIMIT_IF | MXC_F_ADC_INTR_HI_LIMIT_IF);
        } else {
            snprintf((char *)fmtstr, 40, "                   ");
        }
        printf("%s\n",fmtstr);
        
        /* Delay for 1/4 second before next reading */
        TMR_Delay(MXC_TMR0, MSEC(250), &sys_tmr_cfg);
    }
    
}
