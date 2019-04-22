/**
 * @file    rpu_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the RPU Peripheral Module.
 */

/* ****************************************************************************
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
 *
 *************************************************************************** */

#ifndef _RPU_REGS_H_
#define _RPU_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
 
#if defined (__ICCARM__)
  #pragma system_include
#endif
 
#if defined (__CC_ARM)
  #pragma anon_unions
#endif
/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     rpu
 * @defgroup    rpu_registers RPU_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the RPU Peripheral Module.
 * @details Resource Protection Unit
 */

/**
 * @ingroup rpu_registers
 * Structure type to access the RPU Registers.
 */
typedef struct {
    __IO uint32_t gcr;                  /**< <tt>\b 0x00:</tt> RPU GCR Register */
    __IO uint32_t sir;                  /**< <tt>\b 0x04:</tt> RPU SIR Register */
    __IO uint32_t fcr;                  /**< <tt>\b 0x08:</tt> RPU FCR Register */
    __IO uint32_t crypto;               /**< <tt>\b 0x0C:</tt> RPU CRYPTO Register */
    __R  uint32_t rsv_0x10_0x2f[8];
    __IO uint32_t wdt0;                 /**< <tt>\b 0x30:</tt> RPU WDT0 Register */
    __IO uint32_t wdt1;                 /**< <tt>\b 0x0034:</tt> RPU WDT1 Register */
    __IO uint32_t wdt2;                 /**< <tt>\b 0x0038:</tt> RPU WDT2 Register */
    __R  uint32_t rsv_0x3c;
    __IO uint32_t smon;                 /**< <tt>\b 0x0040:</tt> RPU SMON Register */
    __IO uint32_t simo;                 /**< <tt>\b 0x0044:</tt> RPU SIMO Register */
    __IO uint32_t dvs;                  /**< <tt>\b 0x0048:</tt> RPU DVS Register */
    __R  uint32_t rsv_0x4c_0x53[2];
    __IO uint32_t bbsir;                /**< <tt>\b 0x0054:</tt> RPU BBSIR Register */
    __R  uint32_t rsv_0x58_0x5f[2];
    __IO uint32_t rtc;                  /**< <tt>\b 0x0060:</tt> RPU RTC Register */
    __IO uint32_t wut;                  /**< <tt>\b 0x0064:</tt> RPU WUT Register */
    __IO uint32_t pwrseq;               /**< <tt>\b 0x0068:</tt> RPU PWRSEQ Register */
    __IO uint32_t bbcr;                 /**< <tt>\b 0x006C:</tt> RPU BBCR Register */
    __R  uint32_t rsv_0x70_0x7f[4];
    __IO uint32_t gpio0;                /**< <tt>\b 0x0080:</tt> RPU GPIO0 Register */
    __R  uint32_t rsv_0x84_0x8f[3];
    __IO uint32_t gpio1;                /**< <tt>\b 0x0090:</tt> RPU GPIO1 Register */
    __R  uint32_t rsv_0x94_0xff[27];
    __IO uint32_t tmr0;                 /**< <tt>\b 0x0100:</tt> RPU TMR0 Register */
    __R  uint32_t rsv_0x104_0x10f[3];
    __IO uint32_t tmr1;                 /**< <tt>\b 0x0110:</tt> RPU TMR1 Register */
    __R  uint32_t rsv_0x114_0x11f[3];
    __IO uint32_t tmr2;                 /**< <tt>\b 0x0120:</tt> RPU TMR2 Register */
    __R  uint32_t rsv_0x124_0x12f[3];
    __IO uint32_t tmr3;                 /**< <tt>\b 0x0130:</tt> RPU TMR3 Register */
    __R  uint32_t rsv_0x134_0x13f[3];
    __IO uint32_t tmr4;                 /**< <tt>\b 0x0140:</tt> RPU TMR4 Register */
    __R  uint32_t rsv_0x144_0x14f[3];
    __IO uint32_t tmr5;                 /**< <tt>\b 0x0150:</tt> RPU TMR5 Register */
    __R  uint32_t rsv_0x154_0x1af[23];
    __IO uint32_t htimer0;              /**< <tt>\b 0x01B0:</tt> RPU HTIMER0 Register */
    __R  uint32_t rsv_0x1b4_0x1bf[3];
    __IO uint32_t htimer1;              /**< <tt>\b 0x01C0:</tt> RPU HTIMER1 Register */
    __R  uint32_t rsv_0x1c4_0x1cf[3];
    __IO uint32_t i2c0;                 /**< <tt>\b 0x01D0:</tt> RPU I2C0 Register */
    __R  uint32_t rsv_0x1d4_0x1df[3];
    __IO uint32_t i2c1;                 /**< <tt>\b 0x01E0:</tt> RPU I2C1 Register */
    __R  uint32_t rsv_0x1e4_0x1ef[3];
    __IO uint32_t i2c2;                 /**< <tt>\b 0x01F0:</tt> RPU I2C2 Register */
    __R  uint32_t rsv_0x1f4_0x25f[27];
    __IO uint32_t spixipm;              /**< <tt>\b 0x0260:</tt> RPU SPIXIPM Register */
    __R  uint32_t rsv_0x264_0x26f[3];
    __IO uint32_t spixipmc;             /**< <tt>\b 0x0270:</tt> RPU SPIXIPMC Register */
    __R  uint32_t rsv_0x274_0x27f[3];
    __IO uint32_t dma0;                 /**< <tt>\b 0x0280:</tt> RPU DMA0 Register */
    __R  uint32_t rsv_0x284_0x28f[3];
    __IO uint32_t flc0;                 /**< <tt>\b 0x0290:</tt> RPU FLC0 Register */
    __IO uint32_t flc1;                 /**< <tt>\b 0x0294:</tt> RPU FLC1 Register */
    __R  uint32_t rsv_0x298_0x29f[2];
    __IO uint32_t icache0;              /**< <tt>\b 0x02A0:</tt> RPU ICACHE0 Register */
    __IO uint32_t icache1;              /**< <tt>\b 0x02A4:</tt> RPU ICACHE1 Register */
    __R  uint32_t rsv_0x2a8_0x2ef[18];
    __IO uint32_t icachexip;            /**< <tt>\b 0x02F0:</tt> RPU ICACHEXIP Register */
    __R  uint32_t rsv_0x2f4_0x32f[15];
    __IO uint32_t dcache;               /**< <tt>\b 0x0330:</tt> RPU DCACHE Register */
    __R  uint32_t rsv_0x334_0x33f[3];
    __IO uint32_t adc;                  /**< <tt>\b 0x0340:</tt> RPU ADC Register */
    __R  uint32_t rsv_0x344_0x34f[3];
    __IO uint32_t dma1;                 /**< <tt>\b 0x0350:</tt> RPU DMA1 Register */
    __R  uint32_t rsv_0x354_0x35f[3];
    __IO uint32_t sdma;                 /**< <tt>\b 0x0360:</tt> RPU SDMA Register */
    __R  uint32_t rsv_0x364_0x36f[3];
    __IO uint32_t sdhcctrl;             /**< <tt>\b 0x0370:</tt> RPU SDHCCTRL Register */
    __R  uint32_t rsv_0x374_0x39f[11];
    __IO uint32_t spid;                 /**< <tt>\b 0x03A0:</tt> RPU SPID Register */
    __R  uint32_t rsv_0x3a4_0x3bf[7];
    __IO uint32_t pt;                   /**< <tt>\b 0x03C0:</tt> RPU PT Register */
    __R  uint32_t rsv_0x3c4_0x3cf[3];
    __IO uint32_t owm;                  /**< <tt>\b 0x03D0:</tt> RPU OWM Register */
    __R  uint32_t rsv_0x3d4_0x3df[3];
    __IO uint32_t sema;                 /**< <tt>\b 0x03E0:</tt> RPU SEMA Register */
    __R  uint32_t rsv_0x3e4_0x41f[15];
    __IO uint32_t uart0;                /**< <tt>\b 0x0420:</tt> RPU UART0 Register */
    __R  uint32_t rsv_0x424_0x42f[3];
    __IO uint32_t uart1;                /**< <tt>\b 0x0430:</tt> RPU UART1 Register */
    __R  uint32_t rsv_0x434_0x43f[3];
    __IO uint32_t uart2;                /**< <tt>\b 0x0440:</tt> RPU UART2 Register */
    __R  uint32_t rsv_0x444_0x45f[7];
    __IO uint32_t qspi1;                /**< <tt>\b 0x0460:</tt> RPU QSPI1 Register */
    __R  uint32_t rsv_0x464_0x47f[7];
    __IO uint32_t qspi2;                /**< <tt>\b 0x0480:</tt> RPU QSPI2 Register */
    __R  uint32_t rsv_0x484_0x4bf[15];
    __IO uint32_t audio;                /**< <tt>\b 0x04C0:</tt> RPU AUDIO Register */
    __R  uint32_t rsv_0x4c4_0x4cf[3];
    __IO uint32_t trng;                 /**< <tt>\b 0x04D0:</tt> RPU TRNG Register */
    __R  uint32_t rsv_0x4d4_0x4ff[11];
    __IO uint32_t btle;                 /**< <tt>\b 0x0500:</tt> RPU BTLE Register */
    __R  uint32_t rsv_0x504_0xb0f[387];
    __IO uint32_t usbhs;                /**< <tt>\b 0x0B10:</tt> RPU USBHS Register */
    __R  uint32_t rsv_0xb14_0xb5f[19];
    __IO uint32_t sdio;                 /**< <tt>\b 0x0B60:</tt> RPU SDIO Register */
    __R  uint32_t rsv_0xb64_0xbbf[23];
    __IO uint32_t spixipmfifo;          /**< <tt>\b 0x0BC0:</tt> RPU SPIXIPMFIFO Register */
    __R  uint32_t rsv_0xbc4_0xbdf[7];
    __IO uint32_t qspi0;                /**< <tt>\b 0x0BE0:</tt> RPU QSPI0 Register */
    __R  uint32_t rsv_0xbe4_0xeff[199];
    __IO uint32_t sram0;                /**< <tt>\b 0x0F00:</tt> RPU SRAM0 Register */
    __R  uint32_t rsv_0xf04_0xf0f[3];
    __IO uint32_t sram1;                /**< <tt>\b 0x0F10:</tt> RPU SRAM1 Register */
    __R  uint32_t rsv_0xf14_0xf1f[3];
    __IO uint32_t sram2;                /**< <tt>\b 0x0F20:</tt> RPU SRAM2 Register */
    __R  uint32_t rsv_0xf24_0xf2f[3];
    __IO uint32_t sram3;                /**< <tt>\b 0x0F30:</tt> RPU SRAM3 Register */
    __R  uint32_t rsv_0xf34_0xf3f[3];
    __IO uint32_t sram4;                /**< <tt>\b 0x0F40:</tt> RPU SRAM4 Register */
    __R  uint32_t rsv_0xf44_0xf4f[3];
    __IO uint32_t sram5;                /**< <tt>\b 0x0F50:</tt> RPU SRAM5 Register */
    __R  uint32_t rsv_0xf54_0xf5f[3];
    __IO uint32_t sram6;                /**< <tt>\b 0x0F60:</tt> RPU SRAM6 Register */
} mxc_rpu_regs_t;

/* Register offsets for module RPU */
/**
 * @ingroup    rpu_registers
 * @defgroup   RPU_Register_Offsets Register Offsets
 * @brief      RPU Peripheral Register Offsets from the RPU Base Peripheral Address. 
 * @{
 */
 #define MXC_R_RPU_GCR                      ((uint32_t)0x00000000UL) /**< Offset from RPU Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_RPU_SIR                      ((uint32_t)0x00000004UL) /**< Offset from RPU Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_RPU_FCR                      ((uint32_t)0x00000008UL) /**< Offset from RPU Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_RPU_CRYPTO                   ((uint32_t)0x0000000CUL) /**< Offset from RPU Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_RPU_WDT0                     ((uint32_t)0x00000030UL) /**< Offset from RPU Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_RPU_WDT1                     ((uint32_t)0x00000034UL) /**< Offset from RPU Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_RPU_WDT2                     ((uint32_t)0x00000038UL) /**< Offset from RPU Base Address: <tt> 0x0038</tt> */ 
 #define MXC_R_RPU_SMON                     ((uint32_t)0x00000040UL) /**< Offset from RPU Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_RPU_SIMO                     ((uint32_t)0x00000044UL) /**< Offset from RPU Base Address: <tt> 0x0044</tt> */ 
 #define MXC_R_RPU_DVS                      ((uint32_t)0x00000048UL) /**< Offset from RPU Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_RPU_BBSIR                    ((uint32_t)0x00000054UL) /**< Offset from RPU Base Address: <tt> 0x0054</tt> */ 
 #define MXC_R_RPU_RTC                      ((uint32_t)0x00000060UL) /**< Offset from RPU Base Address: <tt> 0x0060</tt> */ 
 #define MXC_R_RPU_WUT                      ((uint32_t)0x00000064UL) /**< Offset from RPU Base Address: <tt> 0x0064</tt> */ 
 #define MXC_R_RPU_PWRSEQ                   ((uint32_t)0x00000068UL) /**< Offset from RPU Base Address: <tt> 0x0068</tt> */ 
 #define MXC_R_RPU_BBCR                     ((uint32_t)0x0000006CUL) /**< Offset from RPU Base Address: <tt> 0x006C</tt> */ 
 #define MXC_R_RPU_GPIO0                    ((uint32_t)0x00000080UL) /**< Offset from RPU Base Address: <tt> 0x0080</tt> */ 
 #define MXC_R_RPU_GPIO1                    ((uint32_t)0x00000090UL) /**< Offset from RPU Base Address: <tt> 0x0090</tt> */ 
 #define MXC_R_RPU_TMR0                     ((uint32_t)0x00000100UL) /**< Offset from RPU Base Address: <tt> 0x0100</tt> */ 
 #define MXC_R_RPU_TMR1                     ((uint32_t)0x00000110UL) /**< Offset from RPU Base Address: <tt> 0x0110</tt> */ 
 #define MXC_R_RPU_TMR2                     ((uint32_t)0x00000120UL) /**< Offset from RPU Base Address: <tt> 0x0120</tt> */ 
 #define MXC_R_RPU_TMR3                     ((uint32_t)0x00000130UL) /**< Offset from RPU Base Address: <tt> 0x0130</tt> */ 
 #define MXC_R_RPU_TMR4                     ((uint32_t)0x00000140UL) /**< Offset from RPU Base Address: <tt> 0x0140</tt> */ 
 #define MXC_R_RPU_TMR5                     ((uint32_t)0x00000150UL) /**< Offset from RPU Base Address: <tt> 0x0150</tt> */ 
 #define MXC_R_RPU_HTIMER0                  ((uint32_t)0x000001B0UL) /**< Offset from RPU Base Address: <tt> 0x01B0</tt> */ 
 #define MXC_R_RPU_HTIMER1                  ((uint32_t)0x000001C0UL) /**< Offset from RPU Base Address: <tt> 0x01C0</tt> */ 
 #define MXC_R_RPU_I2C0                     ((uint32_t)0x000001D0UL) /**< Offset from RPU Base Address: <tt> 0x01D0</tt> */ 
 #define MXC_R_RPU_I2C1                     ((uint32_t)0x000001E0UL) /**< Offset from RPU Base Address: <tt> 0x01E0</tt> */ 
 #define MXC_R_RPU_I2C2                     ((uint32_t)0x000001F0UL) /**< Offset from RPU Base Address: <tt> 0x01F0</tt> */ 
 #define MXC_R_RPU_SPIXIPM                  ((uint32_t)0x00000260UL) /**< Offset from RPU Base Address: <tt> 0x0260</tt> */ 
 #define MXC_R_RPU_SPIXIPMC                 ((uint32_t)0x00000270UL) /**< Offset from RPU Base Address: <tt> 0x0270</tt> */ 
 #define MXC_R_RPU_DMA0                     ((uint32_t)0x00000280UL) /**< Offset from RPU Base Address: <tt> 0x0280</tt> */ 
 #define MXC_R_RPU_FLC0                     ((uint32_t)0x00000290UL) /**< Offset from RPU Base Address: <tt> 0x0290</tt> */ 
 #define MXC_R_RPU_FLC1                     ((uint32_t)0x00000294UL) /**< Offset from RPU Base Address: <tt> 0x0294</tt> */ 
 #define MXC_R_RPU_ICACHE0                  ((uint32_t)0x000002A0UL) /**< Offset from RPU Base Address: <tt> 0x02A0</tt> */ 
 #define MXC_R_RPU_ICACHE1                  ((uint32_t)0x000002A4UL) /**< Offset from RPU Base Address: <tt> 0x02A4</tt> */ 
 #define MXC_R_RPU_ICACHEXIP                ((uint32_t)0x000002F0UL) /**< Offset from RPU Base Address: <tt> 0x02F0</tt> */ 
 #define MXC_R_RPU_DCACHE                   ((uint32_t)0x00000330UL) /**< Offset from RPU Base Address: <tt> 0x0330</tt> */ 
 #define MXC_R_RPU_ADC                      ((uint32_t)0x00000340UL) /**< Offset from RPU Base Address: <tt> 0x0340</tt> */ 
 #define MXC_R_RPU_DMA1                     ((uint32_t)0x00000350UL) /**< Offset from RPU Base Address: <tt> 0x0350</tt> */ 
 #define MXC_R_RPU_SDMA                     ((uint32_t)0x00000360UL) /**< Offset from RPU Base Address: <tt> 0x0360</tt> */ 
 #define MXC_R_RPU_SDHCCTRL                 ((uint32_t)0x00000370UL) /**< Offset from RPU Base Address: <tt> 0x0370</tt> */ 
 #define MXC_R_RPU_SPID                     ((uint32_t)0x000003A0UL) /**< Offset from RPU Base Address: <tt> 0x03A0</tt> */ 
 #define MXC_R_RPU_PT                       ((uint32_t)0x000003C0UL) /**< Offset from RPU Base Address: <tt> 0x03C0</tt> */ 
 #define MXC_R_RPU_OWM                      ((uint32_t)0x000003D0UL) /**< Offset from RPU Base Address: <tt> 0x03D0</tt> */ 
 #define MXC_R_RPU_SEMA                     ((uint32_t)0x000003E0UL) /**< Offset from RPU Base Address: <tt> 0x03E0</tt> */ 
 #define MXC_R_RPU_UART0                    ((uint32_t)0x00000420UL) /**< Offset from RPU Base Address: <tt> 0x0420</tt> */ 
 #define MXC_R_RPU_UART1                    ((uint32_t)0x00000430UL) /**< Offset from RPU Base Address: <tt> 0x0430</tt> */ 
 #define MXC_R_RPU_UART2                    ((uint32_t)0x00000440UL) /**< Offset from RPU Base Address: <tt> 0x0440</tt> */ 
 #define MXC_R_RPU_QSPI1                    ((uint32_t)0x00000460UL) /**< Offset from RPU Base Address: <tt> 0x0460</tt> */ 
 #define MXC_R_RPU_QSPI2                    ((uint32_t)0x00000480UL) /**< Offset from RPU Base Address: <tt> 0x0480</tt> */ 
 #define MXC_R_RPU_AUDIO                    ((uint32_t)0x000004C0UL) /**< Offset from RPU Base Address: <tt> 0x04C0</tt> */ 
 #define MXC_R_RPU_TRNG                     ((uint32_t)0x000004D0UL) /**< Offset from RPU Base Address: <tt> 0x04D0</tt> */ 
 #define MXC_R_RPU_BTLE                     ((uint32_t)0x00000500UL) /**< Offset from RPU Base Address: <tt> 0x0500</tt> */ 
 #define MXC_R_RPU_USBHS                    ((uint32_t)0x00000B10UL) /**< Offset from RPU Base Address: <tt> 0x0B10</tt> */ 
 #define MXC_R_RPU_SDIO                     ((uint32_t)0x00000B60UL) /**< Offset from RPU Base Address: <tt> 0x0B60</tt> */ 
 #define MXC_R_RPU_SPIXIPMFIFO              ((uint32_t)0x00000BC0UL) /**< Offset from RPU Base Address: <tt> 0x0BC0</tt> */ 
 #define MXC_R_RPU_QSPI0                    ((uint32_t)0x00000BE0UL) /**< Offset from RPU Base Address: <tt> 0x0BE0</tt> */ 
 #define MXC_R_RPU_SRAM0                    ((uint32_t)0x00000F00UL) /**< Offset from RPU Base Address: <tt> 0x0F00</tt> */ 
 #define MXC_R_RPU_SRAM1                    ((uint32_t)0x00000F10UL) /**< Offset from RPU Base Address: <tt> 0x0F10</tt> */ 
 #define MXC_R_RPU_SRAM2                    ((uint32_t)0x00000F20UL) /**< Offset from RPU Base Address: <tt> 0x0F20</tt> */ 
 #define MXC_R_RPU_SRAM3                    ((uint32_t)0x00000F30UL) /**< Offset from RPU Base Address: <tt> 0x0F30</tt> */ 
 #define MXC_R_RPU_SRAM4                    ((uint32_t)0x00000F40UL) /**< Offset from RPU Base Address: <tt> 0x0F40</tt> */ 
 #define MXC_R_RPU_SRAM5                    ((uint32_t)0x00000F50UL) /**< Offset from RPU Base Address: <tt> 0x0F50</tt> */ 
 #define MXC_R_RPU_SRAM6                    ((uint32_t)0x00000F60UL) /**< Offset from RPU Base Address: <tt> 0x0F60</tt> */ 
/**@} end of group rpu_registers */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_GCR RPU_GCR
 * @brief    GCR Protection Register
 * @{
 */
 #define MXC_F_RPU_GCR_DMA0ACN_POS                      0 /**< GCR_DMA0ACN Position */
 #define MXC_F_RPU_GCR_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_GCR_DMA0ACN_POS)) /**< GCR_DMA0ACN Mask */

 #define MXC_F_RPU_GCR_DMA1ACN_POS                      1 /**< GCR_DMA1ACN Position */
 #define MXC_F_RPU_GCR_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_GCR_DMA1ACN_POS)) /**< GCR_DMA1ACN Mask */

 #define MXC_F_RPU_GCR_USBACN_POS                       2 /**< GCR_USBACN Position */
 #define MXC_F_RPU_GCR_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_GCR_USBACN_POS)) /**< GCR_USBACN Mask */

 #define MXC_F_RPU_GCR_SYS0ACN_POS                      3 /**< GCR_SYS0ACN Position */
 #define MXC_F_RPU_GCR_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_GCR_SYS0ACN_POS)) /**< GCR_SYS0ACN Mask */

 #define MXC_F_RPU_GCR_SYS1ACN_POS                      4 /**< GCR_SYS1ACN Position */
 #define MXC_F_RPU_GCR_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_GCR_SYS1ACN_POS)) /**< GCR_SYS1ACN Mask */

 #define MXC_F_RPU_GCR_SDMADACN_POS                     5 /**< GCR_SDMADACN Position */
 #define MXC_F_RPU_GCR_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_GCR_SDMADACN_POS)) /**< GCR_SDMADACN Mask */

 #define MXC_F_RPU_GCR_SDMAIACN_POS                     6 /**< GCR_SDMAIACN Position */
 #define MXC_F_RPU_GCR_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_GCR_SDMAIACN_POS)) /**< GCR_SDMAIACN Mask */

 #define MXC_F_RPU_GCR_CRYPTOACN_POS                    7 /**< GCR_CRYPTOACN Position */
 #define MXC_F_RPU_GCR_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GCR_CRYPTOACN_POS)) /**< GCR_CRYPTOACN Mask */

 #define MXC_F_RPU_GCR_SDIOACN_POS                      8 /**< GCR_SDIOACN Position */
 #define MXC_F_RPU_GCR_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_GCR_SDIOACN_POS)) /**< GCR_SDIOACN Mask */

/**@} end of group RPU_GCR_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SIR RPU_SIR
 * @brief    SIR Protection Register
 * @{
 */
 #define MXC_F_RPU_SIR_DMA0ACN_POS                      0 /**< SIR_DMA0ACN Position */
 #define MXC_F_RPU_SIR_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SIR_DMA0ACN_POS)) /**< SIR_DMA0ACN Mask */

 #define MXC_F_RPU_SIR_DMA1ACN_POS                      1 /**< SIR_DMA1ACN Position */
 #define MXC_F_RPU_SIR_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SIR_DMA1ACN_POS)) /**< SIR_DMA1ACN Mask */

 #define MXC_F_RPU_SIR_USBACN_POS                       2 /**< SIR_USBACN Position */
 #define MXC_F_RPU_SIR_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_SIR_USBACN_POS)) /**< SIR_USBACN Mask */

 #define MXC_F_RPU_SIR_SYS0ACN_POS                      3 /**< SIR_SYS0ACN Position */
 #define MXC_F_RPU_SIR_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SIR_SYS0ACN_POS)) /**< SIR_SYS0ACN Mask */

 #define MXC_F_RPU_SIR_SYS1ACN_POS                      4 /**< SIR_SYS1ACN Position */
 #define MXC_F_RPU_SIR_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SIR_SYS1ACN_POS)) /**< SIR_SYS1ACN Mask */

 #define MXC_F_RPU_SIR_SDMADACN_POS                     5 /**< SIR_SDMADACN Position */
 #define MXC_F_RPU_SIR_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SIR_SDMADACN_POS)) /**< SIR_SDMADACN Mask */

 #define MXC_F_RPU_SIR_SDMAIACN_POS                     6 /**< SIR_SDMAIACN Position */
 #define MXC_F_RPU_SIR_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SIR_SDMAIACN_POS)) /**< SIR_SDMAIACN Mask */

 #define MXC_F_RPU_SIR_CRYPTOACN_POS                    7 /**< SIR_CRYPTOACN Position */
 #define MXC_F_RPU_SIR_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SIR_CRYPTOACN_POS)) /**< SIR_CRYPTOACN Mask */

 #define MXC_F_RPU_SIR_SDIOACN_POS                      8 /**< SIR_SDIOACN Position */
 #define MXC_F_RPU_SIR_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SIR_SDIOACN_POS)) /**< SIR_SDIOACN Mask */

/**@} end of group RPU_SIR_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_FCR RPU_FCR
 * @brief    FCR Protection Register
 * @{
 */
 #define MXC_F_RPU_FCR_DMA0ACN_POS                      0 /**< FCR_DMA0ACN Position */
 #define MXC_F_RPU_FCR_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_FCR_DMA0ACN_POS)) /**< FCR_DMA0ACN Mask */

 #define MXC_F_RPU_FCR_DMA1ACN_POS                      1 /**< FCR_DMA1ACN Position */
 #define MXC_F_RPU_FCR_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_FCR_DMA1ACN_POS)) /**< FCR_DMA1ACN Mask */

 #define MXC_F_RPU_FCR_USBACN_POS                       2 /**< FCR_USBACN Position */
 #define MXC_F_RPU_FCR_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_FCR_USBACN_POS)) /**< FCR_USBACN Mask */

 #define MXC_F_RPU_FCR_SYS0ACN_POS                      3 /**< FCR_SYS0ACN Position */
 #define MXC_F_RPU_FCR_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_FCR_SYS0ACN_POS)) /**< FCR_SYS0ACN Mask */

 #define MXC_F_RPU_FCR_SYS1ACN_POS                      4 /**< FCR_SYS1ACN Position */
 #define MXC_F_RPU_FCR_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_FCR_SYS1ACN_POS)) /**< FCR_SYS1ACN Mask */

 #define MXC_F_RPU_FCR_SDMADACN_POS                     5 /**< FCR_SDMADACN Position */
 #define MXC_F_RPU_FCR_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FCR_SDMADACN_POS)) /**< FCR_SDMADACN Mask */

 #define MXC_F_RPU_FCR_SDMAIACN_POS                     6 /**< FCR_SDMAIACN Position */
 #define MXC_F_RPU_FCR_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FCR_SDMAIACN_POS)) /**< FCR_SDMAIACN Mask */

 #define MXC_F_RPU_FCR_CRYPTOACN_POS                    7 /**< FCR_CRYPTOACN Position */
 #define MXC_F_RPU_FCR_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_FCR_CRYPTOACN_POS)) /**< FCR_CRYPTOACN Mask */

 #define MXC_F_RPU_FCR_SDIOACN_POS                      8 /**< FCR_SDIOACN Position */
 #define MXC_F_RPU_FCR_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_FCR_SDIOACN_POS)) /**< FCR_SDIOACN Mask */

/**@} end of group RPU_FCR_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_CRYPTO RPU_CRYPTO
 * @brief    Crypto Protection Register
 * @{
 */
 #define MXC_F_RPU_CRYPTO_DMA0ACN_POS                   0 /**< CRYPTO_DMA0ACN Position */
 #define MXC_F_RPU_CRYPTO_DMA0ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_DMA0ACN_POS)) /**< CRYPTO_DMA0ACN Mask */

 #define MXC_F_RPU_CRYPTO_DMA1ACN_POS                   1 /**< CRYPTO_DMA1ACN Position */
 #define MXC_F_RPU_CRYPTO_DMA1ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_DMA1ACN_POS)) /**< CRYPTO_DMA1ACN Mask */

 #define MXC_F_RPU_CRYPTO_USBACN_POS                    2 /**< CRYPTO_USBACN Position */
 #define MXC_F_RPU_CRYPTO_USBACN                        ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_USBACN_POS)) /**< CRYPTO_USBACN Mask */

 #define MXC_F_RPU_CRYPTO_SYS0ACN_POS                   3 /**< CRYPTO_SYS0ACN Position */
 #define MXC_F_RPU_CRYPTO_SYS0ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_SYS0ACN_POS)) /**< CRYPTO_SYS0ACN Mask */

 #define MXC_F_RPU_CRYPTO_SYS1ACN_POS                   4 /**< CRYPTO_SYS1ACN Position */
 #define MXC_F_RPU_CRYPTO_SYS1ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_SYS1ACN_POS)) /**< CRYPTO_SYS1ACN Mask */

 #define MXC_F_RPU_CRYPTO_SDMADACN_POS                  5 /**< CRYPTO_SDMADACN Position */
 #define MXC_F_RPU_CRYPTO_SDMADACN                      ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_SDMADACN_POS)) /**< CRYPTO_SDMADACN Mask */

 #define MXC_F_RPU_CRYPTO_SDMAIACN_POS                  6 /**< CRYPTO_SDMAIACN Position */
 #define MXC_F_RPU_CRYPTO_SDMAIACN                      ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_SDMAIACN_POS)) /**< CRYPTO_SDMAIACN Mask */

 #define MXC_F_RPU_CRYPTO_CRYPTOACN_POS                 7 /**< CRYPTO_CRYPTOACN Position */
 #define MXC_F_RPU_CRYPTO_CRYPTOACN                     ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_CRYPTOACN_POS)) /**< CRYPTO_CRYPTOACN Mask */

 #define MXC_F_RPU_CRYPTO_SDIOACN_POS                   8 /**< CRYPTO_SDIOACN Position */
 #define MXC_F_RPU_CRYPTO_SDIOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_CRYPTO_SDIOACN_POS)) /**< CRYPTO_SDIOACN Mask */

/**@} end of group RPU_CRYPTO_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_WDT0 RPU_WDT0
 * @brief    Watchdog 0 Protection Register
 * @{
 */
 #define MXC_F_RPU_WDT0_DMA0ACN_POS                     0 /**< WDT0_DMA0ACN Position */
 #define MXC_F_RPU_WDT0_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_DMA0ACN_POS)) /**< WDT0_DMA0ACN Mask */

 #define MXC_F_RPU_WDT0_DMA1ACN_POS                     1 /**< WDT0_DMA1ACN Position */
 #define MXC_F_RPU_WDT0_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_DMA1ACN_POS)) /**< WDT0_DMA1ACN Mask */

 #define MXC_F_RPU_WDT0_USBACN_POS                      2 /**< WDT0_USBACN Position */
 #define MXC_F_RPU_WDT0_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_USBACN_POS)) /**< WDT0_USBACN Mask */

 #define MXC_F_RPU_WDT0_SYS0ACN_POS                     3 /**< WDT0_SYS0ACN Position */
 #define MXC_F_RPU_WDT0_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_SYS0ACN_POS)) /**< WDT0_SYS0ACN Mask */

 #define MXC_F_RPU_WDT0_SYS1ACN_POS                     4 /**< WDT0_SYS1ACN Position */
 #define MXC_F_RPU_WDT0_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_SYS1ACN_POS)) /**< WDT0_SYS1ACN Mask */

 #define MXC_F_RPU_WDT0_SDMADACN_POS                    5 /**< WDT0_SDMADACN Position */
 #define MXC_F_RPU_WDT0_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_SDMADACN_POS)) /**< WDT0_SDMADACN Mask */

 #define MXC_F_RPU_WDT0_SDMAIACN_POS                    6 /**< WDT0_SDMAIACN Position */
 #define MXC_F_RPU_WDT0_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_SDMAIACN_POS)) /**< WDT0_SDMAIACN Mask */

 #define MXC_F_RPU_WDT0_CRYPTOACN_POS                   7 /**< WDT0_CRYPTOACN Position */
 #define MXC_F_RPU_WDT0_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_CRYPTOACN_POS)) /**< WDT0_CRYPTOACN Mask */

 #define MXC_F_RPU_WDT0_SDIOACN_POS                     8 /**< WDT0_SDIOACN Position */
 #define MXC_F_RPU_WDT0_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT0_SDIOACN_POS)) /**< WDT0_SDIOACN Mask */

/**@} end of group RPU_WDT0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_WDT1 RPU_WDT1
 * @brief    Watchdog 1 Protection Register
 * @{
 */
 #define MXC_F_RPU_WDT1_DMA0ACN_POS                     0 /**< WDT1_DMA0ACN Position */
 #define MXC_F_RPU_WDT1_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_DMA0ACN_POS)) /**< WDT1_DMA0ACN Mask */

 #define MXC_F_RPU_WDT1_DMA1ACN_POS                     1 /**< WDT1_DMA1ACN Position */
 #define MXC_F_RPU_WDT1_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_DMA1ACN_POS)) /**< WDT1_DMA1ACN Mask */

 #define MXC_F_RPU_WDT1_USBACN_POS                      2 /**< WDT1_USBACN Position */
 #define MXC_F_RPU_WDT1_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_USBACN_POS)) /**< WDT1_USBACN Mask */

 #define MXC_F_RPU_WDT1_SYS0ACN_POS                     3 /**< WDT1_SYS0ACN Position */
 #define MXC_F_RPU_WDT1_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_SYS0ACN_POS)) /**< WDT1_SYS0ACN Mask */

 #define MXC_F_RPU_WDT1_SYS1ACN_POS                     4 /**< WDT1_SYS1ACN Position */
 #define MXC_F_RPU_WDT1_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_SYS1ACN_POS)) /**< WDT1_SYS1ACN Mask */

 #define MXC_F_RPU_WDT1_SDMADACN_POS                    5 /**< WDT1_SDMADACN Position */
 #define MXC_F_RPU_WDT1_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_SDMADACN_POS)) /**< WDT1_SDMADACN Mask */

 #define MXC_F_RPU_WDT1_SDMAIACN_POS                    6 /**< WDT1_SDMAIACN Position */
 #define MXC_F_RPU_WDT1_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_SDMAIACN_POS)) /**< WDT1_SDMAIACN Mask */

 #define MXC_F_RPU_WDT1_CRYPTOACN_POS                   7 /**< WDT1_CRYPTOACN Position */
 #define MXC_F_RPU_WDT1_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_CRYPTOACN_POS)) /**< WDT1_CRYPTOACN Mask */

 #define MXC_F_RPU_WDT1_SDIOACN_POS                     8 /**< WDT1_SDIOACN Position */
 #define MXC_F_RPU_WDT1_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT1_SDIOACN_POS)) /**< WDT1_SDIOACN Mask */

/**@} end of group RPU_WDT1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_WDT2 RPU_WDT2
 * @brief    Watchdog 2 Protection Register
 * @{
 */
 #define MXC_F_RPU_WDT2_DMA0ACN_POS                     0 /**< WDT2_DMA0ACN Position */
 #define MXC_F_RPU_WDT2_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_DMA0ACN_POS)) /**< WDT2_DMA0ACN Mask */

 #define MXC_F_RPU_WDT2_DMA1ACN_POS                     1 /**< WDT2_DMA1ACN Position */
 #define MXC_F_RPU_WDT2_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_DMA1ACN_POS)) /**< WDT2_DMA1ACN Mask */

 #define MXC_F_RPU_WDT2_USBACN_POS                      2 /**< WDT2_USBACN Position */
 #define MXC_F_RPU_WDT2_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_USBACN_POS)) /**< WDT2_USBACN Mask */

 #define MXC_F_RPU_WDT2_SYS0ACN_POS                     3 /**< WDT2_SYS0ACN Position */
 #define MXC_F_RPU_WDT2_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_SYS0ACN_POS)) /**< WDT2_SYS0ACN Mask */

 #define MXC_F_RPU_WDT2_SYS1ACN_POS                     4 /**< WDT2_SYS1ACN Position */
 #define MXC_F_RPU_WDT2_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_SYS1ACN_POS)) /**< WDT2_SYS1ACN Mask */

 #define MXC_F_RPU_WDT2_SDMADACN_POS                    5 /**< WDT2_SDMADACN Position */
 #define MXC_F_RPU_WDT2_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_SDMADACN_POS)) /**< WDT2_SDMADACN Mask */

 #define MXC_F_RPU_WDT2_SDMAIACN_POS                    6 /**< WDT2_SDMAIACN Position */
 #define MXC_F_RPU_WDT2_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_SDMAIACN_POS)) /**< WDT2_SDMAIACN Mask */

 #define MXC_F_RPU_WDT2_CRYPTOACN_POS                   7 /**< WDT2_CRYPTOACN Position */
 #define MXC_F_RPU_WDT2_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_CRYPTOACN_POS)) /**< WDT2_CRYPTOACN Mask */

 #define MXC_F_RPU_WDT2_SDIOACN_POS                     8 /**< WDT2_SDIOACN Position */
 #define MXC_F_RPU_WDT2_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WDT2_SDIOACN_POS)) /**< WDT2_SDIOACN Mask */

/**@} end of group RPU_WDT2_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SMON RPU_SMON
 * @brief    SMON Protection Register
 * @{
 */
 #define MXC_F_RPU_SMON_DMA0ACN_POS                     0 /**< SMON_DMA0ACN Position */
 #define MXC_F_RPU_SMON_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SMON_DMA0ACN_POS)) /**< SMON_DMA0ACN Mask */

 #define MXC_F_RPU_SMON_DMA1ACN_POS                     1 /**< SMON_DMA1ACN Position */
 #define MXC_F_RPU_SMON_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SMON_DMA1ACN_POS)) /**< SMON_DMA1ACN Mask */

 #define MXC_F_RPU_SMON_USBACN_POS                      2 /**< SMON_USBACN Position */
 #define MXC_F_RPU_SMON_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SMON_USBACN_POS)) /**< SMON_USBACN Mask */

 #define MXC_F_RPU_SMON_SYS0ACN_POS                     3 /**< SMON_SYS0ACN Position */
 #define MXC_F_RPU_SMON_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SMON_SYS0ACN_POS)) /**< SMON_SYS0ACN Mask */

 #define MXC_F_RPU_SMON_SYS1ACN_POS                     4 /**< SMON_SYS1ACN Position */
 #define MXC_F_RPU_SMON_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SMON_SYS1ACN_POS)) /**< SMON_SYS1ACN Mask */

 #define MXC_F_RPU_SMON_SDMADACN_POS                    5 /**< SMON_SDMADACN Position */
 #define MXC_F_RPU_SMON_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SMON_SDMADACN_POS)) /**< SMON_SDMADACN Mask */

 #define MXC_F_RPU_SMON_SDMAIACN_POS                    6 /**< SMON_SDMAIACN Position */
 #define MXC_F_RPU_SMON_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SMON_SDMAIACN_POS)) /**< SMON_SDMAIACN Mask */

 #define MXC_F_RPU_SMON_CRYPTOACN_POS                   7 /**< SMON_CRYPTOACN Position */
 #define MXC_F_RPU_SMON_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_SMON_CRYPTOACN_POS)) /**< SMON_CRYPTOACN Mask */

 #define MXC_F_RPU_SMON_SDIOACN_POS                     8 /**< SMON_SDIOACN Position */
 #define MXC_F_RPU_SMON_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SMON_SDIOACN_POS)) /**< SMON_SDIOACN Mask */

/**@} end of group RPU_SMON_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SIMO RPU_SIMO
 * @brief    SIMO Protection Register
 * @{
 */
 #define MXC_F_RPU_SIMO_DMA0ACN_POS                     0 /**< SIMO_DMA0ACN Position */
 #define MXC_F_RPU_SIMO_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_DMA0ACN_POS)) /**< SIMO_DMA0ACN Mask */

 #define MXC_F_RPU_SIMO_DMA1ACN_POS                     1 /**< SIMO_DMA1ACN Position */
 #define MXC_F_RPU_SIMO_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_DMA1ACN_POS)) /**< SIMO_DMA1ACN Mask */

 #define MXC_F_RPU_SIMO_USBACN_POS                      2 /**< SIMO_USBACN Position */
 #define MXC_F_RPU_SIMO_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_USBACN_POS)) /**< SIMO_USBACN Mask */

 #define MXC_F_RPU_SIMO_SYS0ACN_POS                     3 /**< SIMO_SYS0ACN Position */
 #define MXC_F_RPU_SIMO_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_SYS0ACN_POS)) /**< SIMO_SYS0ACN Mask */

 #define MXC_F_RPU_SIMO_SYS1ACN_POS                     4 /**< SIMO_SYS1ACN Position */
 #define MXC_F_RPU_SIMO_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_SYS1ACN_POS)) /**< SIMO_SYS1ACN Mask */

 #define MXC_F_RPU_SIMO_SDMADACN_POS                    5 /**< SIMO_SDMADACN Position */
 #define MXC_F_RPU_SIMO_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_SDMADACN_POS)) /**< SIMO_SDMADACN Mask */

 #define MXC_F_RPU_SIMO_SDMAIACN_POS                    6 /**< SIMO_SDMAIACN Position */
 #define MXC_F_RPU_SIMO_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_SDMAIACN_POS)) /**< SIMO_SDMAIACN Mask */

 #define MXC_F_RPU_SIMO_CRYPTOACN_POS                   7 /**< SIMO_CRYPTOACN Position */
 #define MXC_F_RPU_SIMO_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_CRYPTOACN_POS)) /**< SIMO_CRYPTOACN Mask */

 #define MXC_F_RPU_SIMO_SDIOACN_POS                     8 /**< SIMO_SDIOACN Position */
 #define MXC_F_RPU_SIMO_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SIMO_SDIOACN_POS)) /**< SIMO_SDIOACN Mask */

/**@} end of group RPU_SIMO_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_DVS RPU_DVS
 * @brief    DVS Protection Register
 * @{
 */
 #define MXC_F_RPU_DVS_DMA0ACN_POS                      0 /**< DVS_DMA0ACN Position */
 #define MXC_F_RPU_DVS_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_DVS_DMA0ACN_POS)) /**< DVS_DMA0ACN Mask */

 #define MXC_F_RPU_DVS_DMA1ACN_POS                      1 /**< DVS_DMA1ACN Position */
 #define MXC_F_RPU_DVS_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_DVS_DMA1ACN_POS)) /**< DVS_DMA1ACN Mask */

 #define MXC_F_RPU_DVS_USBACN_POS                       2 /**< DVS_USBACN Position */
 #define MXC_F_RPU_DVS_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_DVS_USBACN_POS)) /**< DVS_USBACN Mask */

 #define MXC_F_RPU_DVS_SYS0ACN_POS                      3 /**< DVS_SYS0ACN Position */
 #define MXC_F_RPU_DVS_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_DVS_SYS0ACN_POS)) /**< DVS_SYS0ACN Mask */

 #define MXC_F_RPU_DVS_SYS1ACN_POS                      4 /**< DVS_SYS1ACN Position */
 #define MXC_F_RPU_DVS_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_DVS_SYS1ACN_POS)) /**< DVS_SYS1ACN Mask */

 #define MXC_F_RPU_DVS_SDMADACN_POS                     5 /**< DVS_SDMADACN Position */
 #define MXC_F_RPU_DVS_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DVS_SDMADACN_POS)) /**< DVS_SDMADACN Mask */

 #define MXC_F_RPU_DVS_SDMAIACN_POS                     6 /**< DVS_SDMAIACN Position */
 #define MXC_F_RPU_DVS_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DVS_SDMAIACN_POS)) /**< DVS_SDMAIACN Mask */

 #define MXC_F_RPU_DVS_CRYPTOACN_POS                    7 /**< DVS_CRYPTOACN Position */
 #define MXC_F_RPU_DVS_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_DVS_CRYPTOACN_POS)) /**< DVS_CRYPTOACN Mask */

 #define MXC_F_RPU_DVS_SDIOACN_POS                      8 /**< DVS_SDIOACN Position */
 #define MXC_F_RPU_DVS_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_DVS_SDIOACN_POS)) /**< DVS_SDIOACN Mask */

/**@} end of group RPU_DVS_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_BBSIR RPU_BBSIR
 * @brief    BBSIR Protection Register
 * @{
 */
 #define MXC_F_RPU_BBSIR_DMA0ACN_POS                    0 /**< BBSIR_DMA0ACN Position */
 #define MXC_F_RPU_BBSIR_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_DMA0ACN_POS)) /**< BBSIR_DMA0ACN Mask */

 #define MXC_F_RPU_BBSIR_DMA1ACN_POS                    1 /**< BBSIR_DMA1ACN Position */
 #define MXC_F_RPU_BBSIR_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_DMA1ACN_POS)) /**< BBSIR_DMA1ACN Mask */

 #define MXC_F_RPU_BBSIR_USBACN_POS                     2 /**< BBSIR_USBACN Position */
 #define MXC_F_RPU_BBSIR_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_USBACN_POS)) /**< BBSIR_USBACN Mask */

 #define MXC_F_RPU_BBSIR_SYS0ACN_POS                    3 /**< BBSIR_SYS0ACN Position */
 #define MXC_F_RPU_BBSIR_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_SYS0ACN_POS)) /**< BBSIR_SYS0ACN Mask */

 #define MXC_F_RPU_BBSIR_SYS1ACN_POS                    4 /**< BBSIR_SYS1ACN Position */
 #define MXC_F_RPU_BBSIR_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_SYS1ACN_POS)) /**< BBSIR_SYS1ACN Mask */

 #define MXC_F_RPU_BBSIR_SDMADACN_POS                   5 /**< BBSIR_SDMADACN Position */
 #define MXC_F_RPU_BBSIR_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_SDMADACN_POS)) /**< BBSIR_SDMADACN Mask */

 #define MXC_F_RPU_BBSIR_SDMAIACN_POS                   6 /**< BBSIR_SDMAIACN Position */
 #define MXC_F_RPU_BBSIR_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_SDMAIACN_POS)) /**< BBSIR_SDMAIACN Mask */

 #define MXC_F_RPU_BBSIR_CRYPTOACN_POS                  7 /**< BBSIR_CRYPTOACN Position */
 #define MXC_F_RPU_BBSIR_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_CRYPTOACN_POS)) /**< BBSIR_CRYPTOACN Mask */

 #define MXC_F_RPU_BBSIR_SDIOACN_POS                    8 /**< BBSIR_SDIOACN Position */
 #define MXC_F_RPU_BBSIR_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BBSIR_SDIOACN_POS)) /**< BBSIR_SDIOACN Mask */

/**@} end of group RPU_BBSIR_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_RTC RPU_RTC
 * @brief    RTC Protection Register
 * @{
 */
 #define MXC_F_RPU_RTC_DMA0ACN_POS                      0 /**< RTC_DMA0ACN Position */
 #define MXC_F_RPU_RTC_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_RTC_DMA0ACN_POS)) /**< RTC_DMA0ACN Mask */

 #define MXC_F_RPU_RTC_DMA1ACN_POS                      1 /**< RTC_DMA1ACN Position */
 #define MXC_F_RPU_RTC_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_RTC_DMA1ACN_POS)) /**< RTC_DMA1ACN Mask */

 #define MXC_F_RPU_RTC_USBACN_POS                       2 /**< RTC_USBACN Position */
 #define MXC_F_RPU_RTC_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_RTC_USBACN_POS)) /**< RTC_USBACN Mask */

 #define MXC_F_RPU_RTC_SYS0ACN_POS                      3 /**< RTC_SYS0ACN Position */
 #define MXC_F_RPU_RTC_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_RTC_SYS0ACN_POS)) /**< RTC_SYS0ACN Mask */

 #define MXC_F_RPU_RTC_SYS1ACN_POS                      4 /**< RTC_SYS1ACN Position */
 #define MXC_F_RPU_RTC_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_RTC_SYS1ACN_POS)) /**< RTC_SYS1ACN Mask */

 #define MXC_F_RPU_RTC_SDMADACN_POS                     5 /**< RTC_SDMADACN Position */
 #define MXC_F_RPU_RTC_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_RTC_SDMADACN_POS)) /**< RTC_SDMADACN Mask */

 #define MXC_F_RPU_RTC_SDMAIACN_POS                     6 /**< RTC_SDMAIACN Position */
 #define MXC_F_RPU_RTC_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_RTC_SDMAIACN_POS)) /**< RTC_SDMAIACN Mask */

 #define MXC_F_RPU_RTC_CRYPTOACN_POS                    7 /**< RTC_CRYPTOACN Position */
 #define MXC_F_RPU_RTC_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_RTC_CRYPTOACN_POS)) /**< RTC_CRYPTOACN Mask */

 #define MXC_F_RPU_RTC_SDIOACN_POS                      8 /**< RTC_SDIOACN Position */
 #define MXC_F_RPU_RTC_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_RTC_SDIOACN_POS)) /**< RTC_SDIOACN Mask */

/**@} end of group RPU_RTC_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_WUT RPU_WUT
 * @brief    Wakeup Timer Protection Register
 * @{
 */
 #define MXC_F_RPU_WUT_DMA0ACN_POS                      0 /**< WUT_DMA0ACN Position */
 #define MXC_F_RPU_WUT_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WUT_DMA0ACN_POS)) /**< WUT_DMA0ACN Mask */

 #define MXC_F_RPU_WUT_DMA1ACN_POS                      1 /**< WUT_DMA1ACN Position */
 #define MXC_F_RPU_WUT_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WUT_DMA1ACN_POS)) /**< WUT_DMA1ACN Mask */

 #define MXC_F_RPU_WUT_USBACN_POS                       2 /**< WUT_USBACN Position */
 #define MXC_F_RPU_WUT_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_WUT_USBACN_POS)) /**< WUT_USBACN Mask */

 #define MXC_F_RPU_WUT_SYS0ACN_POS                      3 /**< WUT_SYS0ACN Position */
 #define MXC_F_RPU_WUT_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WUT_SYS0ACN_POS)) /**< WUT_SYS0ACN Mask */

 #define MXC_F_RPU_WUT_SYS1ACN_POS                      4 /**< WUT_SYS1ACN Position */
 #define MXC_F_RPU_WUT_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WUT_SYS1ACN_POS)) /**< WUT_SYS1ACN Mask */

 #define MXC_F_RPU_WUT_SDMADACN_POS                     5 /**< WUT_SDMADACN Position */
 #define MXC_F_RPU_WUT_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WUT_SDMADACN_POS)) /**< WUT_SDMADACN Mask */

 #define MXC_F_RPU_WUT_SDMAIACN_POS                     6 /**< WUT_SDMAIACN Position */
 #define MXC_F_RPU_WUT_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_WUT_SDMAIACN_POS)) /**< WUT_SDMAIACN Mask */

 #define MXC_F_RPU_WUT_CRYPTOACN_POS                    7 /**< WUT_CRYPTOACN Position */
 #define MXC_F_RPU_WUT_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_WUT_CRYPTOACN_POS)) /**< WUT_CRYPTOACN Mask */

 #define MXC_F_RPU_WUT_SDIOACN_POS                      8 /**< WUT_SDIOACN Position */
 #define MXC_F_RPU_WUT_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_WUT_SDIOACN_POS)) /**< WUT_SDIOACN Mask */

/**@} end of group RPU_WUT_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_PWRSEQ RPU_PWRSEQ
 * @brief    Power Sequencer Protection Register
 * @{
 */
 #define MXC_F_RPU_PWRSEQ_DMA0ACN_POS                   0 /**< PWRSEQ_DMA0ACN Position */
 #define MXC_F_RPU_PWRSEQ_DMA0ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_DMA0ACN_POS)) /**< PWRSEQ_DMA0ACN Mask */

 #define MXC_F_RPU_PWRSEQ_DMA1ACN_POS                   1 /**< PWRSEQ_DMA1ACN Position */
 #define MXC_F_RPU_PWRSEQ_DMA1ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_DMA1ACN_POS)) /**< PWRSEQ_DMA1ACN Mask */

 #define MXC_F_RPU_PWRSEQ_USBACN_POS                    2 /**< PWRSEQ_USBACN Position */
 #define MXC_F_RPU_PWRSEQ_USBACN                        ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_USBACN_POS)) /**< PWRSEQ_USBACN Mask */

 #define MXC_F_RPU_PWRSEQ_SYS0ACN_POS                   3 /**< PWRSEQ_SYS0ACN Position */
 #define MXC_F_RPU_PWRSEQ_SYS0ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_SYS0ACN_POS)) /**< PWRSEQ_SYS0ACN Mask */

 #define MXC_F_RPU_PWRSEQ_SYS1ACN_POS                   4 /**< PWRSEQ_SYS1ACN Position */
 #define MXC_F_RPU_PWRSEQ_SYS1ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_SYS1ACN_POS)) /**< PWRSEQ_SYS1ACN Mask */

 #define MXC_F_RPU_PWRSEQ_SDMADACN_POS                  5 /**< PWRSEQ_SDMADACN Position */
 #define MXC_F_RPU_PWRSEQ_SDMADACN                      ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_SDMADACN_POS)) /**< PWRSEQ_SDMADACN Mask */

 #define MXC_F_RPU_PWRSEQ_SDMAIACN_POS                  6 /**< PWRSEQ_SDMAIACN Position */
 #define MXC_F_RPU_PWRSEQ_SDMAIACN                      ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_SDMAIACN_POS)) /**< PWRSEQ_SDMAIACN Mask */

 #define MXC_F_RPU_PWRSEQ_CRYPTOACN_POS                 7 /**< PWRSEQ_CRYPTOACN Position */
 #define MXC_F_RPU_PWRSEQ_CRYPTOACN                     ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_CRYPTOACN_POS)) /**< PWRSEQ_CRYPTOACN Mask */

 #define MXC_F_RPU_PWRSEQ_SDIOACN_POS                   8 /**< PWRSEQ_SDIOACN Position */
 #define MXC_F_RPU_PWRSEQ_SDIOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_PWRSEQ_SDIOACN_POS)) /**< PWRSEQ_SDIOACN Mask */

/**@} end of group RPU_PWRSEQ_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_BBCR RPU_BBCR
 * @brief    BBCR Protection Register
 * @{
 */
 #define MXC_F_RPU_BBCR_DMA0ACN_POS                     0 /**< BBCR_DMA0ACN Position */
 #define MXC_F_RPU_BBCR_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_DMA0ACN_POS)) /**< BBCR_DMA0ACN Mask */

 #define MXC_F_RPU_BBCR_DMA1ACN_POS                     1 /**< BBCR_DMA1ACN Position */
 #define MXC_F_RPU_BBCR_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_DMA1ACN_POS)) /**< BBCR_DMA1ACN Mask */

 #define MXC_F_RPU_BBCR_USBACN_POS                      2 /**< BBCR_USBACN Position */
 #define MXC_F_RPU_BBCR_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_USBACN_POS)) /**< BBCR_USBACN Mask */

 #define MXC_F_RPU_BBCR_SYS0ACN_POS                     3 /**< BBCR_SYS0ACN Position */
 #define MXC_F_RPU_BBCR_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_SYS0ACN_POS)) /**< BBCR_SYS0ACN Mask */

 #define MXC_F_RPU_BBCR_SYS1ACN_POS                     4 /**< BBCR_SYS1ACN Position */
 #define MXC_F_RPU_BBCR_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_SYS1ACN_POS)) /**< BBCR_SYS1ACN Mask */

 #define MXC_F_RPU_BBCR_SDMADACN_POS                    5 /**< BBCR_SDMADACN Position */
 #define MXC_F_RPU_BBCR_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_SDMADACN_POS)) /**< BBCR_SDMADACN Mask */

 #define MXC_F_RPU_BBCR_SDMAIACN_POS                    6 /**< BBCR_SDMAIACN Position */
 #define MXC_F_RPU_BBCR_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_SDMAIACN_POS)) /**< BBCR_SDMAIACN Mask */

 #define MXC_F_RPU_BBCR_CRYPTOACN_POS                   7 /**< BBCR_CRYPTOACN Position */
 #define MXC_F_RPU_BBCR_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_CRYPTOACN_POS)) /**< BBCR_CRYPTOACN Mask */

 #define MXC_F_RPU_BBCR_SDIOACN_POS                     8 /**< BBCR_SDIOACN Position */
 #define MXC_F_RPU_BBCR_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BBCR_SDIOACN_POS)) /**< BBCR_SDIOACN Mask */

/**@} end of group RPU_BBCR_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_GPIO0 RPU_GPIO0
 * @brief    GPIO0 Protection Register
 * @{
 */
 #define MXC_F_RPU_GPIO0_DMA0ACN_POS                    0 /**< GPIO0_DMA0ACN Position */
 #define MXC_F_RPU_GPIO0_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_DMA0ACN_POS)) /**< GPIO0_DMA0ACN Mask */

 #define MXC_F_RPU_GPIO0_DMA1ACN_POS                    1 /**< GPIO0_DMA1ACN Position */
 #define MXC_F_RPU_GPIO0_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_DMA1ACN_POS)) /**< GPIO0_DMA1ACN Mask */

 #define MXC_F_RPU_GPIO0_USBACN_POS                     2 /**< GPIO0_USBACN Position */
 #define MXC_F_RPU_GPIO0_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_USBACN_POS)) /**< GPIO0_USBACN Mask */

 #define MXC_F_RPU_GPIO0_SYS0ACN_POS                    3 /**< GPIO0_SYS0ACN Position */
 #define MXC_F_RPU_GPIO0_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_SYS0ACN_POS)) /**< GPIO0_SYS0ACN Mask */

 #define MXC_F_RPU_GPIO0_SYS1ACN_POS                    4 /**< GPIO0_SYS1ACN Position */
 #define MXC_F_RPU_GPIO0_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_SYS1ACN_POS)) /**< GPIO0_SYS1ACN Mask */

 #define MXC_F_RPU_GPIO0_SDMADACN_POS                   5 /**< GPIO0_SDMADACN Position */
 #define MXC_F_RPU_GPIO0_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_SDMADACN_POS)) /**< GPIO0_SDMADACN Mask */

 #define MXC_F_RPU_GPIO0_SDMAIACN_POS                   6 /**< GPIO0_SDMAIACN Position */
 #define MXC_F_RPU_GPIO0_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_SDMAIACN_POS)) /**< GPIO0_SDMAIACN Mask */

 #define MXC_F_RPU_GPIO0_CRYPTOACN_POS                  7 /**< GPIO0_CRYPTOACN Position */
 #define MXC_F_RPU_GPIO0_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_CRYPTOACN_POS)) /**< GPIO0_CRYPTOACN Mask */

 #define MXC_F_RPU_GPIO0_SDIOACN_POS                    8 /**< GPIO0_SDIOACN Position */
 #define MXC_F_RPU_GPIO0_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO0_SDIOACN_POS)) /**< GPIO0_SDIOACN Mask */

/**@} end of group RPU_GPIO0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_GPIO1 RPU_GPIO1
 * @brief    GPIO1 Protection Register
 * @{
 */
 #define MXC_F_RPU_GPIO1_DMA0ACN_POS                    0 /**< GPIO1_DMA0ACN Position */
 #define MXC_F_RPU_GPIO1_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_DMA0ACN_POS)) /**< GPIO1_DMA0ACN Mask */

 #define MXC_F_RPU_GPIO1_DMA1ACN_POS                    1 /**< GPIO1_DMA1ACN Position */
 #define MXC_F_RPU_GPIO1_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_DMA1ACN_POS)) /**< GPIO1_DMA1ACN Mask */

 #define MXC_F_RPU_GPIO1_USBACN_POS                     2 /**< GPIO1_USBACN Position */
 #define MXC_F_RPU_GPIO1_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_USBACN_POS)) /**< GPIO1_USBACN Mask */

 #define MXC_F_RPU_GPIO1_SYS0ACN_POS                    3 /**< GPIO1_SYS0ACN Position */
 #define MXC_F_RPU_GPIO1_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_SYS0ACN_POS)) /**< GPIO1_SYS0ACN Mask */

 #define MXC_F_RPU_GPIO1_SYS1ACN_POS                    4 /**< GPIO1_SYS1ACN Position */
 #define MXC_F_RPU_GPIO1_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_SYS1ACN_POS)) /**< GPIO1_SYS1ACN Mask */

 #define MXC_F_RPU_GPIO1_SDMADACN_POS                   5 /**< GPIO1_SDMADACN Position */
 #define MXC_F_RPU_GPIO1_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_SDMADACN_POS)) /**< GPIO1_SDMADACN Mask */

 #define MXC_F_RPU_GPIO1_SDMAIACN_POS                   6 /**< GPIO1_SDMAIACN Position */
 #define MXC_F_RPU_GPIO1_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_SDMAIACN_POS)) /**< GPIO1_SDMAIACN Mask */

 #define MXC_F_RPU_GPIO1_CRYPTOACN_POS                  7 /**< GPIO1_CRYPTOACN Position */
 #define MXC_F_RPU_GPIO1_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_CRYPTOACN_POS)) /**< GPIO1_CRYPTOACN Mask */

 #define MXC_F_RPU_GPIO1_SDIOACN_POS                    8 /**< GPIO1_SDIOACN Position */
 #define MXC_F_RPU_GPIO1_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_GPIO1_SDIOACN_POS)) /**< GPIO1_SDIOACN Mask */

/**@} end of group RPU_GPIO1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_TMR0 RPU_TMR0
 * @brief    TMR0 Protection Register
 * @{
 */
 #define MXC_F_RPU_TMR0_DMA0ACN_POS                     0 /**< TMR0_DMA0ACN Position */
 #define MXC_F_RPU_TMR0_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_DMA0ACN_POS)) /**< TMR0_DMA0ACN Mask */

 #define MXC_F_RPU_TMR0_DMA1ACN_POS                     1 /**< TMR0_DMA1ACN Position */
 #define MXC_F_RPU_TMR0_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_DMA1ACN_POS)) /**< TMR0_DMA1ACN Mask */

 #define MXC_F_RPU_TMR0_USBACN_POS                      2 /**< TMR0_USBACN Position */
 #define MXC_F_RPU_TMR0_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_USBACN_POS)) /**< TMR0_USBACN Mask */

 #define MXC_F_RPU_TMR0_SYS0ACN_POS                     3 /**< TMR0_SYS0ACN Position */
 #define MXC_F_RPU_TMR0_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_SYS0ACN_POS)) /**< TMR0_SYS0ACN Mask */

 #define MXC_F_RPU_TMR0_SYS1ACN_POS                     4 /**< TMR0_SYS1ACN Position */
 #define MXC_F_RPU_TMR0_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_SYS1ACN_POS)) /**< TMR0_SYS1ACN Mask */

 #define MXC_F_RPU_TMR0_SDMADACN_POS                    5 /**< TMR0_SDMADACN Position */
 #define MXC_F_RPU_TMR0_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_SDMADACN_POS)) /**< TMR0_SDMADACN Mask */

 #define MXC_F_RPU_TMR0_SDMAIACN_POS                    6 /**< TMR0_SDMAIACN Position */
 #define MXC_F_RPU_TMR0_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_SDMAIACN_POS)) /**< TMR0_SDMAIACN Mask */

 #define MXC_F_RPU_TMR0_CRYPTOACN_POS                   7 /**< TMR0_CRYPTOACN Position */
 #define MXC_F_RPU_TMR0_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_CRYPTOACN_POS)) /**< TMR0_CRYPTOACN Mask */

 #define MXC_F_RPU_TMR0_SDIOACN_POS                     8 /**< TMR0_SDIOACN Position */
 #define MXC_F_RPU_TMR0_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR0_SDIOACN_POS)) /**< TMR0_SDIOACN Mask */

/**@} end of group RPU_TMR0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_TMR1 RPU_TMR1
 * @brief    TMR1 Protection Register
 * @{
 */
 #define MXC_F_RPU_TMR1_DMA0ACN_POS                     0 /**< TMR1_DMA0ACN Position */
 #define MXC_F_RPU_TMR1_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_DMA0ACN_POS)) /**< TMR1_DMA0ACN Mask */

 #define MXC_F_RPU_TMR1_DMA1ACN_POS                     1 /**< TMR1_DMA1ACN Position */
 #define MXC_F_RPU_TMR1_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_DMA1ACN_POS)) /**< TMR1_DMA1ACN Mask */

 #define MXC_F_RPU_TMR1_USBACN_POS                      2 /**< TMR1_USBACN Position */
 #define MXC_F_RPU_TMR1_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_USBACN_POS)) /**< TMR1_USBACN Mask */

 #define MXC_F_RPU_TMR1_SYS0ACN_POS                     3 /**< TMR1_SYS0ACN Position */
 #define MXC_F_RPU_TMR1_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_SYS0ACN_POS)) /**< TMR1_SYS0ACN Mask */

 #define MXC_F_RPU_TMR1_SYS1ACN_POS                     4 /**< TMR1_SYS1ACN Position */
 #define MXC_F_RPU_TMR1_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_SYS1ACN_POS)) /**< TMR1_SYS1ACN Mask */

 #define MXC_F_RPU_TMR1_SDMADACN_POS                    5 /**< TMR1_SDMADACN Position */
 #define MXC_F_RPU_TMR1_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_SDMADACN_POS)) /**< TMR1_SDMADACN Mask */

 #define MXC_F_RPU_TMR1_SDMAIACN_POS                    6 /**< TMR1_SDMAIACN Position */
 #define MXC_F_RPU_TMR1_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_SDMAIACN_POS)) /**< TMR1_SDMAIACN Mask */

 #define MXC_F_RPU_TMR1_CRYPTOACN_POS                   7 /**< TMR1_CRYPTOACN Position */
 #define MXC_F_RPU_TMR1_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_CRYPTOACN_POS)) /**< TMR1_CRYPTOACN Mask */

 #define MXC_F_RPU_TMR1_SDIOACN_POS                     8 /**< TMR1_SDIOACN Position */
 #define MXC_F_RPU_TMR1_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR1_SDIOACN_POS)) /**< TMR1_SDIOACN Mask */

/**@} end of group RPU_TMR1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_TMR2 RPU_TMR2
 * @brief    TMR2 Protection Register
 * @{
 */
 #define MXC_F_RPU_TMR2_DMA0ACN_POS                     0 /**< TMR2_DMA0ACN Position */
 #define MXC_F_RPU_TMR2_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_DMA0ACN_POS)) /**< TMR2_DMA0ACN Mask */

 #define MXC_F_RPU_TMR2_DMA1ACN_POS                     1 /**< TMR2_DMA1ACN Position */
 #define MXC_F_RPU_TMR2_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_DMA1ACN_POS)) /**< TMR2_DMA1ACN Mask */

 #define MXC_F_RPU_TMR2_USBACN_POS                      2 /**< TMR2_USBACN Position */
 #define MXC_F_RPU_TMR2_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_USBACN_POS)) /**< TMR2_USBACN Mask */

 #define MXC_F_RPU_TMR2_SYS0ACN_POS                     3 /**< TMR2_SYS0ACN Position */
 #define MXC_F_RPU_TMR2_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_SYS0ACN_POS)) /**< TMR2_SYS0ACN Mask */

 #define MXC_F_RPU_TMR2_SYS1ACN_POS                     4 /**< TMR2_SYS1ACN Position */
 #define MXC_F_RPU_TMR2_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_SYS1ACN_POS)) /**< TMR2_SYS1ACN Mask */

 #define MXC_F_RPU_TMR2_SDMADACN_POS                    5 /**< TMR2_SDMADACN Position */
 #define MXC_F_RPU_TMR2_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_SDMADACN_POS)) /**< TMR2_SDMADACN Mask */

 #define MXC_F_RPU_TMR2_SDMAIACN_POS                    6 /**< TMR2_SDMAIACN Position */
 #define MXC_F_RPU_TMR2_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_SDMAIACN_POS)) /**< TMR2_SDMAIACN Mask */

 #define MXC_F_RPU_TMR2_CRYPTOACN_POS                   7 /**< TMR2_CRYPTOACN Position */
 #define MXC_F_RPU_TMR2_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_CRYPTOACN_POS)) /**< TMR2_CRYPTOACN Mask */

 #define MXC_F_RPU_TMR2_SDIOACN_POS                     8 /**< TMR2_SDIOACN Position */
 #define MXC_F_RPU_TMR2_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR2_SDIOACN_POS)) /**< TMR2_SDIOACN Mask */

/**@} end of group RPU_TMR2_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_TMR3 RPU_TMR3
 * @brief    TMR3 Protection Register
 * @{
 */
 #define MXC_F_RPU_TMR3_DMA0ACN_POS                     0 /**< TMR3_DMA0ACN Position */
 #define MXC_F_RPU_TMR3_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_DMA0ACN_POS)) /**< TMR3_DMA0ACN Mask */

 #define MXC_F_RPU_TMR3_DMA1ACN_POS                     1 /**< TMR3_DMA1ACN Position */
 #define MXC_F_RPU_TMR3_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_DMA1ACN_POS)) /**< TMR3_DMA1ACN Mask */

 #define MXC_F_RPU_TMR3_USBACN_POS                      2 /**< TMR3_USBACN Position */
 #define MXC_F_RPU_TMR3_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_USBACN_POS)) /**< TMR3_USBACN Mask */

 #define MXC_F_RPU_TMR3_SYS0ACN_POS                     3 /**< TMR3_SYS0ACN Position */
 #define MXC_F_RPU_TMR3_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_SYS0ACN_POS)) /**< TMR3_SYS0ACN Mask */

 #define MXC_F_RPU_TMR3_SYS1ACN_POS                     4 /**< TMR3_SYS1ACN Position */
 #define MXC_F_RPU_TMR3_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_SYS1ACN_POS)) /**< TMR3_SYS1ACN Mask */

 #define MXC_F_RPU_TMR3_SDMADACN_POS                    5 /**< TMR3_SDMADACN Position */
 #define MXC_F_RPU_TMR3_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_SDMADACN_POS)) /**< TMR3_SDMADACN Mask */

 #define MXC_F_RPU_TMR3_SDMAIACN_POS                    6 /**< TMR3_SDMAIACN Position */
 #define MXC_F_RPU_TMR3_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_SDMAIACN_POS)) /**< TMR3_SDMAIACN Mask */

 #define MXC_F_RPU_TMR3_CRYPTOACN_POS                   7 /**< TMR3_CRYPTOACN Position */
 #define MXC_F_RPU_TMR3_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_CRYPTOACN_POS)) /**< TMR3_CRYPTOACN Mask */

 #define MXC_F_RPU_TMR3_SDIOACN_POS                     8 /**< TMR3_SDIOACN Position */
 #define MXC_F_RPU_TMR3_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR3_SDIOACN_POS)) /**< TMR3_SDIOACN Mask */

/**@} end of group RPU_TMR3_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_TMR4 RPU_TMR4
 * @brief    TMR4 Protection Register
 * @{
 */
 #define MXC_F_RPU_TMR4_DMA0ACN_POS                     0 /**< TMR4_DMA0ACN Position */
 #define MXC_F_RPU_TMR4_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_DMA0ACN_POS)) /**< TMR4_DMA0ACN Mask */

 #define MXC_F_RPU_TMR4_DMA1ACN_POS                     1 /**< TMR4_DMA1ACN Position */
 #define MXC_F_RPU_TMR4_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_DMA1ACN_POS)) /**< TMR4_DMA1ACN Mask */

 #define MXC_F_RPU_TMR4_USBACN_POS                      2 /**< TMR4_USBACN Position */
 #define MXC_F_RPU_TMR4_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_USBACN_POS)) /**< TMR4_USBACN Mask */

 #define MXC_F_RPU_TMR4_SYS0ACN_POS                     3 /**< TMR4_SYS0ACN Position */
 #define MXC_F_RPU_TMR4_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_SYS0ACN_POS)) /**< TMR4_SYS0ACN Mask */

 #define MXC_F_RPU_TMR4_SYS1ACN_POS                     4 /**< TMR4_SYS1ACN Position */
 #define MXC_F_RPU_TMR4_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_SYS1ACN_POS)) /**< TMR4_SYS1ACN Mask */

 #define MXC_F_RPU_TMR4_SDMADACN_POS                    5 /**< TMR4_SDMADACN Position */
 #define MXC_F_RPU_TMR4_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_SDMADACN_POS)) /**< TMR4_SDMADACN Mask */

 #define MXC_F_RPU_TMR4_SDMAIACN_POS                    6 /**< TMR4_SDMAIACN Position */
 #define MXC_F_RPU_TMR4_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_SDMAIACN_POS)) /**< TMR4_SDMAIACN Mask */

 #define MXC_F_RPU_TMR4_CRYPTOACN_POS                   7 /**< TMR4_CRYPTOACN Position */
 #define MXC_F_RPU_TMR4_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_CRYPTOACN_POS)) /**< TMR4_CRYPTOACN Mask */

 #define MXC_F_RPU_TMR4_SDIOACN_POS                     8 /**< TMR4_SDIOACN Position */
 #define MXC_F_RPU_TMR4_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR4_SDIOACN_POS)) /**< TMR4_SDIOACN Mask */

/**@} end of group RPU_TMR4_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_TMR5 RPU_TMR5
 * @brief    TMR5 Protection Register
 * @{
 */
 #define MXC_F_RPU_TMR5_DMA0ACN_POS                     0 /**< TMR5_DMA0ACN Position */
 #define MXC_F_RPU_TMR5_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_DMA0ACN_POS)) /**< TMR5_DMA0ACN Mask */

 #define MXC_F_RPU_TMR5_DMA1ACN_POS                     1 /**< TMR5_DMA1ACN Position */
 #define MXC_F_RPU_TMR5_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_DMA1ACN_POS)) /**< TMR5_DMA1ACN Mask */

 #define MXC_F_RPU_TMR5_USBACN_POS                      2 /**< TMR5_USBACN Position */
 #define MXC_F_RPU_TMR5_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_USBACN_POS)) /**< TMR5_USBACN Mask */

 #define MXC_F_RPU_TMR5_SYS0ACN_POS                     3 /**< TMR5_SYS0ACN Position */
 #define MXC_F_RPU_TMR5_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_SYS0ACN_POS)) /**< TMR5_SYS0ACN Mask */

 #define MXC_F_RPU_TMR5_SYS1ACN_POS                     4 /**< TMR5_SYS1ACN Position */
 #define MXC_F_RPU_TMR5_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_SYS1ACN_POS)) /**< TMR5_SYS1ACN Mask */

 #define MXC_F_RPU_TMR5_SDMADACN_POS                    5 /**< TMR5_SDMADACN Position */
 #define MXC_F_RPU_TMR5_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_SDMADACN_POS)) /**< TMR5_SDMADACN Mask */

 #define MXC_F_RPU_TMR5_SDMAIACN_POS                    6 /**< TMR5_SDMAIACN Position */
 #define MXC_F_RPU_TMR5_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_SDMAIACN_POS)) /**< TMR5_SDMAIACN Mask */

 #define MXC_F_RPU_TMR5_CRYPTOACN_POS                   7 /**< TMR5_CRYPTOACN Position */
 #define MXC_F_RPU_TMR5_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_CRYPTOACN_POS)) /**< TMR5_CRYPTOACN Mask */

 #define MXC_F_RPU_TMR5_SDIOACN_POS                     8 /**< TMR5_SDIOACN Position */
 #define MXC_F_RPU_TMR5_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TMR5_SDIOACN_POS)) /**< TMR5_SDIOACN Mask */

/**@} end of group RPU_TMR5_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_HTIMER0 RPU_HTIMER0
 * @brief    HTimer0 Protection Register
 * @{
 */
 #define MXC_F_RPU_HTIMER0_DMA0ACN_POS                  0 /**< HTIMER0_DMA0ACN Position */
 #define MXC_F_RPU_HTIMER0_DMA0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_DMA0ACN_POS)) /**< HTIMER0_DMA0ACN Mask */

 #define MXC_F_RPU_HTIMER0_DMA1ACN_POS                  1 /**< HTIMER0_DMA1ACN Position */
 #define MXC_F_RPU_HTIMER0_DMA1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_DMA1ACN_POS)) /**< HTIMER0_DMA1ACN Mask */

 #define MXC_F_RPU_HTIMER0_USBACN_POS                   2 /**< HTIMER0_USBACN Position */
 #define MXC_F_RPU_HTIMER0_USBACN                       ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_USBACN_POS)) /**< HTIMER0_USBACN Mask */

 #define MXC_F_RPU_HTIMER0_SYS0ACN_POS                  3 /**< HTIMER0_SYS0ACN Position */
 #define MXC_F_RPU_HTIMER0_SYS0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_SYS0ACN_POS)) /**< HTIMER0_SYS0ACN Mask */

 #define MXC_F_RPU_HTIMER0_SYS1ACN_POS                  4 /**< HTIMER0_SYS1ACN Position */
 #define MXC_F_RPU_HTIMER0_SYS1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_SYS1ACN_POS)) /**< HTIMER0_SYS1ACN Mask */

 #define MXC_F_RPU_HTIMER0_SDMADACN_POS                 5 /**< HTIMER0_SDMADACN Position */
 #define MXC_F_RPU_HTIMER0_SDMADACN                     ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_SDMADACN_POS)) /**< HTIMER0_SDMADACN Mask */

 #define MXC_F_RPU_HTIMER0_SDMAIACN_POS                 6 /**< HTIMER0_SDMAIACN Position */
 #define MXC_F_RPU_HTIMER0_SDMAIACN                     ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_SDMAIACN_POS)) /**< HTIMER0_SDMAIACN Mask */

 #define MXC_F_RPU_HTIMER0_CRYPTOACN_POS                7 /**< HTIMER0_CRYPTOACN Position */
 #define MXC_F_RPU_HTIMER0_CRYPTOACN                    ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_CRYPTOACN_POS)) /**< HTIMER0_CRYPTOACN Mask */

 #define MXC_F_RPU_HTIMER0_SDIOACN_POS                  8 /**< HTIMER0_SDIOACN Position */
 #define MXC_F_RPU_HTIMER0_SDIOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER0_SDIOACN_POS)) /**< HTIMER0_SDIOACN Mask */

/**@} end of group RPU_HTIMER0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_HTIMER1 RPU_HTIMER1
 * @brief    HTimer1 Protection Register
 * @{
 */
 #define MXC_F_RPU_HTIMER1_DMA0ACN_POS                  0 /**< HTIMER1_DMA0ACN Position */
 #define MXC_F_RPU_HTIMER1_DMA0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_DMA0ACN_POS)) /**< HTIMER1_DMA0ACN Mask */

 #define MXC_F_RPU_HTIMER1_DMA1ACN_POS                  1 /**< HTIMER1_DMA1ACN Position */
 #define MXC_F_RPU_HTIMER1_DMA1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_DMA1ACN_POS)) /**< HTIMER1_DMA1ACN Mask */

 #define MXC_F_RPU_HTIMER1_USBACN_POS                   2 /**< HTIMER1_USBACN Position */
 #define MXC_F_RPU_HTIMER1_USBACN                       ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_USBACN_POS)) /**< HTIMER1_USBACN Mask */

 #define MXC_F_RPU_HTIMER1_SYS0ACN_POS                  3 /**< HTIMER1_SYS0ACN Position */
 #define MXC_F_RPU_HTIMER1_SYS0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_SYS0ACN_POS)) /**< HTIMER1_SYS0ACN Mask */

 #define MXC_F_RPU_HTIMER1_SYS1ACN_POS                  4 /**< HTIMER1_SYS1ACN Position */
 #define MXC_F_RPU_HTIMER1_SYS1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_SYS1ACN_POS)) /**< HTIMER1_SYS1ACN Mask */

 #define MXC_F_RPU_HTIMER1_SDMADACN_POS                 5 /**< HTIMER1_SDMADACN Position */
 #define MXC_F_RPU_HTIMER1_SDMADACN                     ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_SDMADACN_POS)) /**< HTIMER1_SDMADACN Mask */

 #define MXC_F_RPU_HTIMER1_SDMAIACN_POS                 6 /**< HTIMER1_SDMAIACN Position */
 #define MXC_F_RPU_HTIMER1_SDMAIACN                     ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_SDMAIACN_POS)) /**< HTIMER1_SDMAIACN Mask */

 #define MXC_F_RPU_HTIMER1_CRYPTOACN_POS                7 /**< HTIMER1_CRYPTOACN Position */
 #define MXC_F_RPU_HTIMER1_CRYPTOACN                    ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_CRYPTOACN_POS)) /**< HTIMER1_CRYPTOACN Mask */

 #define MXC_F_RPU_HTIMER1_SDIOACN_POS                  8 /**< HTIMER1_SDIOACN Position */
 #define MXC_F_RPU_HTIMER1_SDIOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_HTIMER1_SDIOACN_POS)) /**< HTIMER1_SDIOACN Mask */

/**@} end of group RPU_HTIMER1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_I2C0 RPU_I2C0
 * @brief    I2C0 Protection Register
 * @{
 */
 #define MXC_F_RPU_I2C0_DMA0ACN_POS                     0 /**< I2C0_DMA0ACN Position */
 #define MXC_F_RPU_I2C0_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_DMA0ACN_POS)) /**< I2C0_DMA0ACN Mask */

 #define MXC_F_RPU_I2C0_DMA1ACN_POS                     1 /**< I2C0_DMA1ACN Position */
 #define MXC_F_RPU_I2C0_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_DMA1ACN_POS)) /**< I2C0_DMA1ACN Mask */

 #define MXC_F_RPU_I2C0_USBACN_POS                      2 /**< I2C0_USBACN Position */
 #define MXC_F_RPU_I2C0_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_USBACN_POS)) /**< I2C0_USBACN Mask */

 #define MXC_F_RPU_I2C0_SYS0ACN_POS                     3 /**< I2C0_SYS0ACN Position */
 #define MXC_F_RPU_I2C0_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_SYS0ACN_POS)) /**< I2C0_SYS0ACN Mask */

 #define MXC_F_RPU_I2C0_SYS1ACN_POS                     4 /**< I2C0_SYS1ACN Position */
 #define MXC_F_RPU_I2C0_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_SYS1ACN_POS)) /**< I2C0_SYS1ACN Mask */

 #define MXC_F_RPU_I2C0_SDMADACN_POS                    5 /**< I2C0_SDMADACN Position */
 #define MXC_F_RPU_I2C0_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_SDMADACN_POS)) /**< I2C0_SDMADACN Mask */

 #define MXC_F_RPU_I2C0_SDMAIACN_POS                    6 /**< I2C0_SDMAIACN Position */
 #define MXC_F_RPU_I2C0_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_SDMAIACN_POS)) /**< I2C0_SDMAIACN Mask */

 #define MXC_F_RPU_I2C0_CRYPTOACN_POS                   7 /**< I2C0_CRYPTOACN Position */
 #define MXC_F_RPU_I2C0_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_CRYPTOACN_POS)) /**< I2C0_CRYPTOACN Mask */

 #define MXC_F_RPU_I2C0_SDIOACN_POS                     8 /**< I2C0_SDIOACN Position */
 #define MXC_F_RPU_I2C0_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C0_SDIOACN_POS)) /**< I2C0_SDIOACN Mask */

/**@} end of group RPU_I2C0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_I2C1 RPU_I2C1
 * @brief    I2C1 Protection Register
 * @{
 */
 #define MXC_F_RPU_I2C1_DMA0ACN_POS                     0 /**< I2C1_DMA0ACN Position */
 #define MXC_F_RPU_I2C1_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_DMA0ACN_POS)) /**< I2C1_DMA0ACN Mask */

 #define MXC_F_RPU_I2C1_DMA1ACN_POS                     1 /**< I2C1_DMA1ACN Position */
 #define MXC_F_RPU_I2C1_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_DMA1ACN_POS)) /**< I2C1_DMA1ACN Mask */

 #define MXC_F_RPU_I2C1_USBACN_POS                      2 /**< I2C1_USBACN Position */
 #define MXC_F_RPU_I2C1_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_USBACN_POS)) /**< I2C1_USBACN Mask */

 #define MXC_F_RPU_I2C1_SYS0ACN_POS                     3 /**< I2C1_SYS0ACN Position */
 #define MXC_F_RPU_I2C1_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_SYS0ACN_POS)) /**< I2C1_SYS0ACN Mask */

 #define MXC_F_RPU_I2C1_SYS1ACN_POS                     4 /**< I2C1_SYS1ACN Position */
 #define MXC_F_RPU_I2C1_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_SYS1ACN_POS)) /**< I2C1_SYS1ACN Mask */

 #define MXC_F_RPU_I2C1_SDMADACN_POS                    5 /**< I2C1_SDMADACN Position */
 #define MXC_F_RPU_I2C1_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_SDMADACN_POS)) /**< I2C1_SDMADACN Mask */

 #define MXC_F_RPU_I2C1_SDMAIACN_POS                    6 /**< I2C1_SDMAIACN Position */
 #define MXC_F_RPU_I2C1_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_SDMAIACN_POS)) /**< I2C1_SDMAIACN Mask */

 #define MXC_F_RPU_I2C1_CRYPTOACN_POS                   7 /**< I2C1_CRYPTOACN Position */
 #define MXC_F_RPU_I2C1_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_CRYPTOACN_POS)) /**< I2C1_CRYPTOACN Mask */

 #define MXC_F_RPU_I2C1_SDIOACN_POS                     8 /**< I2C1_SDIOACN Position */
 #define MXC_F_RPU_I2C1_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C1_SDIOACN_POS)) /**< I2C1_SDIOACN Mask */

/**@} end of group RPU_I2C1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_I2C2 RPU_I2C2
 * @brief    I2C2 Protection Register
 * @{
 */
 #define MXC_F_RPU_I2C2_DMA0ACN_POS                     0 /**< I2C2_DMA0ACN Position */
 #define MXC_F_RPU_I2C2_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_DMA0ACN_POS)) /**< I2C2_DMA0ACN Mask */

 #define MXC_F_RPU_I2C2_DMA1ACN_POS                     1 /**< I2C2_DMA1ACN Position */
 #define MXC_F_RPU_I2C2_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_DMA1ACN_POS)) /**< I2C2_DMA1ACN Mask */

 #define MXC_F_RPU_I2C2_USBACN_POS                      2 /**< I2C2_USBACN Position */
 #define MXC_F_RPU_I2C2_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_USBACN_POS)) /**< I2C2_USBACN Mask */

 #define MXC_F_RPU_I2C2_SYS0ACN_POS                     3 /**< I2C2_SYS0ACN Position */
 #define MXC_F_RPU_I2C2_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_SYS0ACN_POS)) /**< I2C2_SYS0ACN Mask */

 #define MXC_F_RPU_I2C2_SYS1ACN_POS                     4 /**< I2C2_SYS1ACN Position */
 #define MXC_F_RPU_I2C2_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_SYS1ACN_POS)) /**< I2C2_SYS1ACN Mask */

 #define MXC_F_RPU_I2C2_SDMADACN_POS                    5 /**< I2C2_SDMADACN Position */
 #define MXC_F_RPU_I2C2_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_SDMADACN_POS)) /**< I2C2_SDMADACN Mask */

 #define MXC_F_RPU_I2C2_SDMAIACN_POS                    6 /**< I2C2_SDMAIACN Position */
 #define MXC_F_RPU_I2C2_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_SDMAIACN_POS)) /**< I2C2_SDMAIACN Mask */

 #define MXC_F_RPU_I2C2_CRYPTOACN_POS                   7 /**< I2C2_CRYPTOACN Position */
 #define MXC_F_RPU_I2C2_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_CRYPTOACN_POS)) /**< I2C2_CRYPTOACN Mask */

 #define MXC_F_RPU_I2C2_SDIOACN_POS                     8 /**< I2C2_SDIOACN Position */
 #define MXC_F_RPU_I2C2_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_I2C2_SDIOACN_POS)) /**< I2C2_SDIOACN Mask */

/**@} end of group RPU_I2C2_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SPIXIPM RPU_SPIXIPM
 * @brief    SPI-XIP Master Protection Register
 * @{
 */
 #define MXC_F_RPU_SPIXIPM_DMA0ACN_POS                  0 /**< SPIXIPM_DMA0ACN Position */
 #define MXC_F_RPU_SPIXIPM_DMA0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_DMA0ACN_POS)) /**< SPIXIPM_DMA0ACN Mask */

 #define MXC_F_RPU_SPIXIPM_DMA1ACN_POS                  1 /**< SPIXIPM_DMA1ACN Position */
 #define MXC_F_RPU_SPIXIPM_DMA1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_DMA1ACN_POS)) /**< SPIXIPM_DMA1ACN Mask */

 #define MXC_F_RPU_SPIXIPM_USBACN_POS                   2 /**< SPIXIPM_USBACN Position */
 #define MXC_F_RPU_SPIXIPM_USBACN                       ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_USBACN_POS)) /**< SPIXIPM_USBACN Mask */

 #define MXC_F_RPU_SPIXIPM_SYS0ACN_POS                  3 /**< SPIXIPM_SYS0ACN Position */
 #define MXC_F_RPU_SPIXIPM_SYS0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_SYS0ACN_POS)) /**< SPIXIPM_SYS0ACN Mask */

 #define MXC_F_RPU_SPIXIPM_SYS1ACN_POS                  4 /**< SPIXIPM_SYS1ACN Position */
 #define MXC_F_RPU_SPIXIPM_SYS1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_SYS1ACN_POS)) /**< SPIXIPM_SYS1ACN Mask */

 #define MXC_F_RPU_SPIXIPM_SDMADACN_POS                 5 /**< SPIXIPM_SDMADACN Position */
 #define MXC_F_RPU_SPIXIPM_SDMADACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_SDMADACN_POS)) /**< SPIXIPM_SDMADACN Mask */

 #define MXC_F_RPU_SPIXIPM_SDMAIACN_POS                 6 /**< SPIXIPM_SDMAIACN Position */
 #define MXC_F_RPU_SPIXIPM_SDMAIACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_SDMAIACN_POS)) /**< SPIXIPM_SDMAIACN Mask */

 #define MXC_F_RPU_SPIXIPM_CRYPTOACN_POS                7 /**< SPIXIPM_CRYPTOACN Position */
 #define MXC_F_RPU_SPIXIPM_CRYPTOACN                    ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_CRYPTOACN_POS)) /**< SPIXIPM_CRYPTOACN Mask */

 #define MXC_F_RPU_SPIXIPM_SDIOACN_POS                  8 /**< SPIXIPM_SDIOACN Position */
 #define MXC_F_RPU_SPIXIPM_SDIOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPM_SDIOACN_POS)) /**< SPIXIPM_SDIOACN Mask */

/**@} end of group RPU_SPIXIPM_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SPIXIPMC RPU_SPIXIPMC
 * @brief    SPI-XIP Master Controller Protection Register
 * @{
 */
 #define MXC_F_RPU_SPIXIPMC_DMA0ACN_POS                 0 /**< SPIXIPMC_DMA0ACN Position */
 #define MXC_F_RPU_SPIXIPMC_DMA0ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_DMA0ACN_POS)) /**< SPIXIPMC_DMA0ACN Mask */

 #define MXC_F_RPU_SPIXIPMC_DMA1ACN_POS                 1 /**< SPIXIPMC_DMA1ACN Position */
 #define MXC_F_RPU_SPIXIPMC_DMA1ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_DMA1ACN_POS)) /**< SPIXIPMC_DMA1ACN Mask */

 #define MXC_F_RPU_SPIXIPMC_USBACN_POS                  2 /**< SPIXIPMC_USBACN Position */
 #define MXC_F_RPU_SPIXIPMC_USBACN                      ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_USBACN_POS)) /**< SPIXIPMC_USBACN Mask */

 #define MXC_F_RPU_SPIXIPMC_SYS0ACN_POS                 3 /**< SPIXIPMC_SYS0ACN Position */
 #define MXC_F_RPU_SPIXIPMC_SYS0ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_SYS0ACN_POS)) /**< SPIXIPMC_SYS0ACN Mask */

 #define MXC_F_RPU_SPIXIPMC_SYS1ACN_POS                 4 /**< SPIXIPMC_SYS1ACN Position */
 #define MXC_F_RPU_SPIXIPMC_SYS1ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_SYS1ACN_POS)) /**< SPIXIPMC_SYS1ACN Mask */

 #define MXC_F_RPU_SPIXIPMC_SDMADACN_POS                5 /**< SPIXIPMC_SDMADACN Position */
 #define MXC_F_RPU_SPIXIPMC_SDMADACN                    ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_SDMADACN_POS)) /**< SPIXIPMC_SDMADACN Mask */

 #define MXC_F_RPU_SPIXIPMC_SDMAIACN_POS                6 /**< SPIXIPMC_SDMAIACN Position */
 #define MXC_F_RPU_SPIXIPMC_SDMAIACN                    ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_SDMAIACN_POS)) /**< SPIXIPMC_SDMAIACN Mask */

 #define MXC_F_RPU_SPIXIPMC_CRYPTOACN_POS               7 /**< SPIXIPMC_CRYPTOACN Position */
 #define MXC_F_RPU_SPIXIPMC_CRYPTOACN                   ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_CRYPTOACN_POS)) /**< SPIXIPMC_CRYPTOACN Mask */

 #define MXC_F_RPU_SPIXIPMC_SDIOACN_POS                 8 /**< SPIXIPMC_SDIOACN Position */
 #define MXC_F_RPU_SPIXIPMC_SDIOACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMC_SDIOACN_POS)) /**< SPIXIPMC_SDIOACN Mask */

/**@} end of group RPU_SPIXIPMC_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_DMA0 RPU_DMA0
 * @brief    DMA0 Protection Register
 * @{
 */
 #define MXC_F_RPU_DMA0_DMA0ACN_POS                     0 /**< DMA0_DMA0ACN Position */
 #define MXC_F_RPU_DMA0_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_DMA0ACN_POS)) /**< DMA0_DMA0ACN Mask */

 #define MXC_F_RPU_DMA0_DMA1ACN_POS                     1 /**< DMA0_DMA1ACN Position */
 #define MXC_F_RPU_DMA0_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_DMA1ACN_POS)) /**< DMA0_DMA1ACN Mask */

 #define MXC_F_RPU_DMA0_USBACN_POS                      2 /**< DMA0_USBACN Position */
 #define MXC_F_RPU_DMA0_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_USBACN_POS)) /**< DMA0_USBACN Mask */

 #define MXC_F_RPU_DMA0_SYS0ACN_POS                     3 /**< DMA0_SYS0ACN Position */
 #define MXC_F_RPU_DMA0_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_SYS0ACN_POS)) /**< DMA0_SYS0ACN Mask */

 #define MXC_F_RPU_DMA0_SYS1ACN_POS                     4 /**< DMA0_SYS1ACN Position */
 #define MXC_F_RPU_DMA0_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_SYS1ACN_POS)) /**< DMA0_SYS1ACN Mask */

 #define MXC_F_RPU_DMA0_SDMADACN_POS                    5 /**< DMA0_SDMADACN Position */
 #define MXC_F_RPU_DMA0_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_SDMADACN_POS)) /**< DMA0_SDMADACN Mask */

 #define MXC_F_RPU_DMA0_SDMAIACN_POS                    6 /**< DMA0_SDMAIACN Position */
 #define MXC_F_RPU_DMA0_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_SDMAIACN_POS)) /**< DMA0_SDMAIACN Mask */

 #define MXC_F_RPU_DMA0_CRYPTOACN_POS                   7 /**< DMA0_CRYPTOACN Position */
 #define MXC_F_RPU_DMA0_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_CRYPTOACN_POS)) /**< DMA0_CRYPTOACN Mask */

 #define MXC_F_RPU_DMA0_SDIOACN_POS                     8 /**< DMA0_SDIOACN Position */
 #define MXC_F_RPU_DMA0_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA0_SDIOACN_POS)) /**< DMA0_SDIOACN Mask */

/**@} end of group RPU_DMA0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_FLC0 RPU_FLC0
 * @brief    Flash 0 Protection Register
 * @{
 */
 #define MXC_F_RPU_FLC0_DMA0ACN_POS                     0 /**< FLC0_DMA0ACN Position */
 #define MXC_F_RPU_FLC0_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_DMA0ACN_POS)) /**< FLC0_DMA0ACN Mask */

 #define MXC_F_RPU_FLC0_DMA1ACN_POS                     1 /**< FLC0_DMA1ACN Position */
 #define MXC_F_RPU_FLC0_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_DMA1ACN_POS)) /**< FLC0_DMA1ACN Mask */

 #define MXC_F_RPU_FLC0_USBACN_POS                      2 /**< FLC0_USBACN Position */
 #define MXC_F_RPU_FLC0_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_USBACN_POS)) /**< FLC0_USBACN Mask */

 #define MXC_F_RPU_FLC0_SYS0ACN_POS                     3 /**< FLC0_SYS0ACN Position */
 #define MXC_F_RPU_FLC0_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_SYS0ACN_POS)) /**< FLC0_SYS0ACN Mask */

 #define MXC_F_RPU_FLC0_SYS1ACN_POS                     4 /**< FLC0_SYS1ACN Position */
 #define MXC_F_RPU_FLC0_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_SYS1ACN_POS)) /**< FLC0_SYS1ACN Mask */

 #define MXC_F_RPU_FLC0_SDMADACN_POS                    5 /**< FLC0_SDMADACN Position */
 #define MXC_F_RPU_FLC0_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_SDMADACN_POS)) /**< FLC0_SDMADACN Mask */

 #define MXC_F_RPU_FLC0_SDMAIACN_POS                    6 /**< FLC0_SDMAIACN Position */
 #define MXC_F_RPU_FLC0_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_SDMAIACN_POS)) /**< FLC0_SDMAIACN Mask */

 #define MXC_F_RPU_FLC0_CRYPTOACN_POS                   7 /**< FLC0_CRYPTOACN Position */
 #define MXC_F_RPU_FLC0_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_CRYPTOACN_POS)) /**< FLC0_CRYPTOACN Mask */

 #define MXC_F_RPU_FLC0_SDIOACN_POS                     8 /**< FLC0_SDIOACN Position */
 #define MXC_F_RPU_FLC0_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC0_SDIOACN_POS)) /**< FLC0_SDIOACN Mask */

/**@} end of group RPU_FLC0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_FLC1 RPU_FLC1
 * @brief    Flash 1 Protection Register
 * @{
 */
 #define MXC_F_RPU_FLC1_DMA0ACN_POS                     0 /**< FLC1_DMA0ACN Position */
 #define MXC_F_RPU_FLC1_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_DMA0ACN_POS)) /**< FLC1_DMA0ACN Mask */

 #define MXC_F_RPU_FLC1_DMA1ACN_POS                     1 /**< FLC1_DMA1ACN Position */
 #define MXC_F_RPU_FLC1_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_DMA1ACN_POS)) /**< FLC1_DMA1ACN Mask */

 #define MXC_F_RPU_FLC1_USBACN_POS                      2 /**< FLC1_USBACN Position */
 #define MXC_F_RPU_FLC1_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_USBACN_POS)) /**< FLC1_USBACN Mask */

 #define MXC_F_RPU_FLC1_SYS0ACN_POS                     3 /**< FLC1_SYS0ACN Position */
 #define MXC_F_RPU_FLC1_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_SYS0ACN_POS)) /**< FLC1_SYS0ACN Mask */

 #define MXC_F_RPU_FLC1_SYS1ACN_POS                     4 /**< FLC1_SYS1ACN Position */
 #define MXC_F_RPU_FLC1_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_SYS1ACN_POS)) /**< FLC1_SYS1ACN Mask */

 #define MXC_F_RPU_FLC1_SDMADACN_POS                    5 /**< FLC1_SDMADACN Position */
 #define MXC_F_RPU_FLC1_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_SDMADACN_POS)) /**< FLC1_SDMADACN Mask */

 #define MXC_F_RPU_FLC1_SDMAIACN_POS                    6 /**< FLC1_SDMAIACN Position */
 #define MXC_F_RPU_FLC1_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_SDMAIACN_POS)) /**< FLC1_SDMAIACN Mask */

 #define MXC_F_RPU_FLC1_CRYPTOACN_POS                   7 /**< FLC1_CRYPTOACN Position */
 #define MXC_F_RPU_FLC1_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_CRYPTOACN_POS)) /**< FLC1_CRYPTOACN Mask */

 #define MXC_F_RPU_FLC1_SDIOACN_POS                     8 /**< FLC1_SDIOACN Position */
 #define MXC_F_RPU_FLC1_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_FLC1_SDIOACN_POS)) /**< FLC1_SDIOACN Mask */

/**@} end of group RPU_FLC1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_ICACHE0 RPU_ICACHE0
 * @brief    Instruction Cache 0 Protection Register
 * @{
 */
 #define MXC_F_RPU_ICACHE0_DMA0ACN_POS                  0 /**< ICACHE0_DMA0ACN Position */
 #define MXC_F_RPU_ICACHE0_DMA0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_DMA0ACN_POS)) /**< ICACHE0_DMA0ACN Mask */

 #define MXC_F_RPU_ICACHE0_DMA1ACN_POS                  1 /**< ICACHE0_DMA1ACN Position */
 #define MXC_F_RPU_ICACHE0_DMA1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_DMA1ACN_POS)) /**< ICACHE0_DMA1ACN Mask */

 #define MXC_F_RPU_ICACHE0_USBACN_POS                   2 /**< ICACHE0_USBACN Position */
 #define MXC_F_RPU_ICACHE0_USBACN                       ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_USBACN_POS)) /**< ICACHE0_USBACN Mask */

 #define MXC_F_RPU_ICACHE0_SYS0ACN_POS                  3 /**< ICACHE0_SYS0ACN Position */
 #define MXC_F_RPU_ICACHE0_SYS0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_SYS0ACN_POS)) /**< ICACHE0_SYS0ACN Mask */

 #define MXC_F_RPU_ICACHE0_SYS1ACN_POS                  4 /**< ICACHE0_SYS1ACN Position */
 #define MXC_F_RPU_ICACHE0_SYS1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_SYS1ACN_POS)) /**< ICACHE0_SYS1ACN Mask */

 #define MXC_F_RPU_ICACHE0_SDMADACN_POS                 5 /**< ICACHE0_SDMADACN Position */
 #define MXC_F_RPU_ICACHE0_SDMADACN                     ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_SDMADACN_POS)) /**< ICACHE0_SDMADACN Mask */

 #define MXC_F_RPU_ICACHE0_SDMAIACN_POS                 6 /**< ICACHE0_SDMAIACN Position */
 #define MXC_F_RPU_ICACHE0_SDMAIACN                     ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_SDMAIACN_POS)) /**< ICACHE0_SDMAIACN Mask */

 #define MXC_F_RPU_ICACHE0_CRYPTOACN_POS                7 /**< ICACHE0_CRYPTOACN Position */
 #define MXC_F_RPU_ICACHE0_CRYPTOACN                    ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_CRYPTOACN_POS)) /**< ICACHE0_CRYPTOACN Mask */

 #define MXC_F_RPU_ICACHE0_SDIOACN_POS                  8 /**< ICACHE0_SDIOACN Position */
 #define MXC_F_RPU_ICACHE0_SDIOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE0_SDIOACN_POS)) /**< ICACHE0_SDIOACN Mask */

/**@} end of group RPU_ICACHE0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_ICACHE1 RPU_ICACHE1
 * @brief    Instruction Cache 1 Protection Register
 * @{
 */
 #define MXC_F_RPU_ICACHE1_DMA0ACN_POS                  0 /**< ICACHE1_DMA0ACN Position */
 #define MXC_F_RPU_ICACHE1_DMA0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_DMA0ACN_POS)) /**< ICACHE1_DMA0ACN Mask */

 #define MXC_F_RPU_ICACHE1_DMA1ACN_POS                  1 /**< ICACHE1_DMA1ACN Position */
 #define MXC_F_RPU_ICACHE1_DMA1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_DMA1ACN_POS)) /**< ICACHE1_DMA1ACN Mask */

 #define MXC_F_RPU_ICACHE1_USBACN_POS                   2 /**< ICACHE1_USBACN Position */
 #define MXC_F_RPU_ICACHE1_USBACN                       ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_USBACN_POS)) /**< ICACHE1_USBACN Mask */

 #define MXC_F_RPU_ICACHE1_SYS0ACN_POS                  3 /**< ICACHE1_SYS0ACN Position */
 #define MXC_F_RPU_ICACHE1_SYS0ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_SYS0ACN_POS)) /**< ICACHE1_SYS0ACN Mask */

 #define MXC_F_RPU_ICACHE1_SYS1ACN_POS                  4 /**< ICACHE1_SYS1ACN Position */
 #define MXC_F_RPU_ICACHE1_SYS1ACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_SYS1ACN_POS)) /**< ICACHE1_SYS1ACN Mask */

 #define MXC_F_RPU_ICACHE1_SDMADACN_POS                 5 /**< ICACHE1_SDMADACN Position */
 #define MXC_F_RPU_ICACHE1_SDMADACN                     ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_SDMADACN_POS)) /**< ICACHE1_SDMADACN Mask */

 #define MXC_F_RPU_ICACHE1_SDMAIACN_POS                 6 /**< ICACHE1_SDMAIACN Position */
 #define MXC_F_RPU_ICACHE1_SDMAIACN                     ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_SDMAIACN_POS)) /**< ICACHE1_SDMAIACN Mask */

 #define MXC_F_RPU_ICACHE1_CRYPTOACN_POS                7 /**< ICACHE1_CRYPTOACN Position */
 #define MXC_F_RPU_ICACHE1_CRYPTOACN                    ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_CRYPTOACN_POS)) /**< ICACHE1_CRYPTOACN Mask */

 #define MXC_F_RPU_ICACHE1_SDIOACN_POS                  8 /**< ICACHE1_SDIOACN Position */
 #define MXC_F_RPU_ICACHE1_SDIOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_ICACHE1_SDIOACN_POS)) /**< ICACHE1_SDIOACN Mask */

/**@} end of group RPU_ICACHE1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_ICACHEXIP RPU_ICACHEXIP
 * @brief    Instruction Cache XIP Protection Register
 * @{
 */
 #define MXC_F_RPU_ICACHEXIP_DMA0ACN_POS                0 /**< ICACHEXIP_DMA0ACN Position */
 #define MXC_F_RPU_ICACHEXIP_DMA0ACN                    ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_DMA0ACN_POS)) /**< ICACHEXIP_DMA0ACN Mask */

 #define MXC_F_RPU_ICACHEXIP_DMA1ACN_POS                1 /**< ICACHEXIP_DMA1ACN Position */
 #define MXC_F_RPU_ICACHEXIP_DMA1ACN                    ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_DMA1ACN_POS)) /**< ICACHEXIP_DMA1ACN Mask */

 #define MXC_F_RPU_ICACHEXIP_USBACN_POS                 2 /**< ICACHEXIP_USBACN Position */
 #define MXC_F_RPU_ICACHEXIP_USBACN                     ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_USBACN_POS)) /**< ICACHEXIP_USBACN Mask */

 #define MXC_F_RPU_ICACHEXIP_SYS0ACN_POS                3 /**< ICACHEXIP_SYS0ACN Position */
 #define MXC_F_RPU_ICACHEXIP_SYS0ACN                    ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_SYS0ACN_POS)) /**< ICACHEXIP_SYS0ACN Mask */

 #define MXC_F_RPU_ICACHEXIP_SYS1ACN_POS                4 /**< ICACHEXIP_SYS1ACN Position */
 #define MXC_F_RPU_ICACHEXIP_SYS1ACN                    ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_SYS1ACN_POS)) /**< ICACHEXIP_SYS1ACN Mask */

 #define MXC_F_RPU_ICACHEXIP_SDMADACN_POS               5 /**< ICACHEXIP_SDMADACN Position */
 #define MXC_F_RPU_ICACHEXIP_SDMADACN                   ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_SDMADACN_POS)) /**< ICACHEXIP_SDMADACN Mask */

 #define MXC_F_RPU_ICACHEXIP_SDMAIACN_POS               6 /**< ICACHEXIP_SDMAIACN Position */
 #define MXC_F_RPU_ICACHEXIP_SDMAIACN                   ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_SDMAIACN_POS)) /**< ICACHEXIP_SDMAIACN Mask */

 #define MXC_F_RPU_ICACHEXIP_CRYPTOACN_POS              7 /**< ICACHEXIP_CRYPTOACN Position */
 #define MXC_F_RPU_ICACHEXIP_CRYPTOACN                  ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_CRYPTOACN_POS)) /**< ICACHEXIP_CRYPTOACN Mask */

 #define MXC_F_RPU_ICACHEXIP_SDIOACN_POS                8 /**< ICACHEXIP_SDIOACN Position */
 #define MXC_F_RPU_ICACHEXIP_SDIOACN                    ((uint32_t)(0x1UL << MXC_F_RPU_ICACHEXIP_SDIOACN_POS)) /**< ICACHEXIP_SDIOACN Mask */

/**@} end of group RPU_ICACHEXIP_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_DCACHE RPU_DCACHE
 * @brief    Data Cache Controller Protection Register
 * @{
 */
 #define MXC_F_RPU_DCACHE_DMA0ACN_POS                   0 /**< DCACHE_DMA0ACN Position */
 #define MXC_F_RPU_DCACHE_DMA0ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_DMA0ACN_POS)) /**< DCACHE_DMA0ACN Mask */

 #define MXC_F_RPU_DCACHE_DMA1ACN_POS                   1 /**< DCACHE_DMA1ACN Position */
 #define MXC_F_RPU_DCACHE_DMA1ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_DMA1ACN_POS)) /**< DCACHE_DMA1ACN Mask */

 #define MXC_F_RPU_DCACHE_USBACN_POS                    2 /**< DCACHE_USBACN Position */
 #define MXC_F_RPU_DCACHE_USBACN                        ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_USBACN_POS)) /**< DCACHE_USBACN Mask */

 #define MXC_F_RPU_DCACHE_SYS0ACN_POS                   3 /**< DCACHE_SYS0ACN Position */
 #define MXC_F_RPU_DCACHE_SYS0ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_SYS0ACN_POS)) /**< DCACHE_SYS0ACN Mask */

 #define MXC_F_RPU_DCACHE_SYS1ACN_POS                   4 /**< DCACHE_SYS1ACN Position */
 #define MXC_F_RPU_DCACHE_SYS1ACN                       ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_SYS1ACN_POS)) /**< DCACHE_SYS1ACN Mask */

 #define MXC_F_RPU_DCACHE_SDMADACN_POS                  5 /**< DCACHE_SDMADACN Position */
 #define MXC_F_RPU_DCACHE_SDMADACN                      ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_SDMADACN_POS)) /**< DCACHE_SDMADACN Mask */

 #define MXC_F_RPU_DCACHE_SDMAIACN_POS                  6 /**< DCACHE_SDMAIACN Position */
 #define MXC_F_RPU_DCACHE_SDMAIACN                      ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_SDMAIACN_POS)) /**< DCACHE_SDMAIACN Mask */

 #define MXC_F_RPU_DCACHE_CRYPTOACN_POS                 7 /**< DCACHE_CRYPTOACN Position */
 #define MXC_F_RPU_DCACHE_CRYPTOACN                     ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_CRYPTOACN_POS)) /**< DCACHE_CRYPTOACN Mask */

 #define MXC_F_RPU_DCACHE_SDIOACN_POS                   8 /**< DCACHE_SDIOACN Position */
 #define MXC_F_RPU_DCACHE_SDIOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_DCACHE_SDIOACN_POS)) /**< DCACHE_SDIOACN Mask */

/**@} end of group RPU_DCACHE_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_ADC RPU_ADC
 * @brief    ADC Protection Register
 * @{
 */
 #define MXC_F_RPU_ADC_DMA0ACN_POS                      0 /**< ADC_DMA0ACN Position */
 #define MXC_F_RPU_ADC_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_ADC_DMA0ACN_POS)) /**< ADC_DMA0ACN Mask */

 #define MXC_F_RPU_ADC_DMA1ACN_POS                      1 /**< ADC_DMA1ACN Position */
 #define MXC_F_RPU_ADC_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_ADC_DMA1ACN_POS)) /**< ADC_DMA1ACN Mask */

 #define MXC_F_RPU_ADC_USBACN_POS                       2 /**< ADC_USBACN Position */
 #define MXC_F_RPU_ADC_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_ADC_USBACN_POS)) /**< ADC_USBACN Mask */

 #define MXC_F_RPU_ADC_SYS0ACN_POS                      3 /**< ADC_SYS0ACN Position */
 #define MXC_F_RPU_ADC_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_ADC_SYS0ACN_POS)) /**< ADC_SYS0ACN Mask */

 #define MXC_F_RPU_ADC_SYS1ACN_POS                      4 /**< ADC_SYS1ACN Position */
 #define MXC_F_RPU_ADC_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_ADC_SYS1ACN_POS)) /**< ADC_SYS1ACN Mask */

 #define MXC_F_RPU_ADC_SDMADACN_POS                     5 /**< ADC_SDMADACN Position */
 #define MXC_F_RPU_ADC_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_ADC_SDMADACN_POS)) /**< ADC_SDMADACN Mask */

 #define MXC_F_RPU_ADC_SDMAIACN_POS                     6 /**< ADC_SDMAIACN Position */
 #define MXC_F_RPU_ADC_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_ADC_SDMAIACN_POS)) /**< ADC_SDMAIACN Mask */

 #define MXC_F_RPU_ADC_CRYPTOACN_POS                    7 /**< ADC_CRYPTOACN Position */
 #define MXC_F_RPU_ADC_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_ADC_CRYPTOACN_POS)) /**< ADC_CRYPTOACN Mask */

 #define MXC_F_RPU_ADC_SDIOACN_POS                      8 /**< ADC_SDIOACN Position */
 #define MXC_F_RPU_ADC_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_ADC_SDIOACN_POS)) /**< ADC_SDIOACN Mask */

/**@} end of group RPU_ADC_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_DMA1 RPU_DMA1
 * @brief    DMA1 Protection Register
 * @{
 */
 #define MXC_F_RPU_DMA1_DMA0ACN_POS                     0 /**< DMA1_DMA0ACN Position */
 #define MXC_F_RPU_DMA1_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_DMA0ACN_POS)) /**< DMA1_DMA0ACN Mask */

 #define MXC_F_RPU_DMA1_DMA1ACN_POS                     1 /**< DMA1_DMA1ACN Position */
 #define MXC_F_RPU_DMA1_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_DMA1ACN_POS)) /**< DMA1_DMA1ACN Mask */

 #define MXC_F_RPU_DMA1_USBACN_POS                      2 /**< DMA1_USBACN Position */
 #define MXC_F_RPU_DMA1_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_USBACN_POS)) /**< DMA1_USBACN Mask */

 #define MXC_F_RPU_DMA1_SYS0ACN_POS                     3 /**< DMA1_SYS0ACN Position */
 #define MXC_F_RPU_DMA1_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_SYS0ACN_POS)) /**< DMA1_SYS0ACN Mask */

 #define MXC_F_RPU_DMA1_SYS1ACN_POS                     4 /**< DMA1_SYS1ACN Position */
 #define MXC_F_RPU_DMA1_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_SYS1ACN_POS)) /**< DMA1_SYS1ACN Mask */

 #define MXC_F_RPU_DMA1_SDMADACN_POS                    5 /**< DMA1_SDMADACN Position */
 #define MXC_F_RPU_DMA1_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_SDMADACN_POS)) /**< DMA1_SDMADACN Mask */

 #define MXC_F_RPU_DMA1_SDMAIACN_POS                    6 /**< DMA1_SDMAIACN Position */
 #define MXC_F_RPU_DMA1_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_SDMAIACN_POS)) /**< DMA1_SDMAIACN Mask */

 #define MXC_F_RPU_DMA1_CRYPTOACN_POS                   7 /**< DMA1_CRYPTOACN Position */
 #define MXC_F_RPU_DMA1_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_CRYPTOACN_POS)) /**< DMA1_CRYPTOACN Mask */

 #define MXC_F_RPU_DMA1_SDIOACN_POS                     8 /**< DMA1_SDIOACN Position */
 #define MXC_F_RPU_DMA1_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_DMA1_SDIOACN_POS)) /**< DMA1_SDIOACN Mask */

/**@} end of group RPU_DMA1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SDMA RPU_SDMA
 * @brief    SDMA Protection Register
 * @{
 */
 #define MXC_F_RPU_SDMA_DMA0ACN_POS                     0 /**< SDMA_DMA0ACN Position */
 #define MXC_F_RPU_SDMA_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_DMA0ACN_POS)) /**< SDMA_DMA0ACN Mask */

 #define MXC_F_RPU_SDMA_DMA1ACN_POS                     1 /**< SDMA_DMA1ACN Position */
 #define MXC_F_RPU_SDMA_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_DMA1ACN_POS)) /**< SDMA_DMA1ACN Mask */

 #define MXC_F_RPU_SDMA_USBACN_POS                      2 /**< SDMA_USBACN Position */
 #define MXC_F_RPU_SDMA_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_USBACN_POS)) /**< SDMA_USBACN Mask */

 #define MXC_F_RPU_SDMA_SYS0ACN_POS                     3 /**< SDMA_SYS0ACN Position */
 #define MXC_F_RPU_SDMA_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_SYS0ACN_POS)) /**< SDMA_SYS0ACN Mask */

 #define MXC_F_RPU_SDMA_SYS1ACN_POS                     4 /**< SDMA_SYS1ACN Position */
 #define MXC_F_RPU_SDMA_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_SYS1ACN_POS)) /**< SDMA_SYS1ACN Mask */

 #define MXC_F_RPU_SDMA_SDMADACN_POS                    5 /**< SDMA_SDMADACN Position */
 #define MXC_F_RPU_SDMA_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_SDMADACN_POS)) /**< SDMA_SDMADACN Mask */

 #define MXC_F_RPU_SDMA_SDMAIACN_POS                    6 /**< SDMA_SDMAIACN Position */
 #define MXC_F_RPU_SDMA_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_SDMAIACN_POS)) /**< SDMA_SDMAIACN Mask */

 #define MXC_F_RPU_SDMA_CRYPTOACN_POS                   7 /**< SDMA_CRYPTOACN Position */
 #define MXC_F_RPU_SDMA_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_CRYPTOACN_POS)) /**< SDMA_CRYPTOACN Mask */

 #define MXC_F_RPU_SDMA_SDIOACN_POS                     8 /**< SDMA_SDIOACN Position */
 #define MXC_F_RPU_SDMA_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SDMA_SDIOACN_POS)) /**< SDMA_SDIOACN Mask */

/**@} end of group RPU_SDMA_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SDHCCTRL RPU_SDHCCTRL
 * @brief    SDHC Controller Protection Register
 * @{
 */
 #define MXC_F_RPU_SDHCCTRL_DMA0ACN_POS                 0 /**< SDHCCTRL_DMA0ACN Position */
 #define MXC_F_RPU_SDHCCTRL_DMA0ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_DMA0ACN_POS)) /**< SDHCCTRL_DMA0ACN Mask */

 #define MXC_F_RPU_SDHCCTRL_DMA1ACN_POS                 1 /**< SDHCCTRL_DMA1ACN Position */
 #define MXC_F_RPU_SDHCCTRL_DMA1ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_DMA1ACN_POS)) /**< SDHCCTRL_DMA1ACN Mask */

 #define MXC_F_RPU_SDHCCTRL_USBACN_POS                  2 /**< SDHCCTRL_USBACN Position */
 #define MXC_F_RPU_SDHCCTRL_USBACN                      ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_USBACN_POS)) /**< SDHCCTRL_USBACN Mask */

 #define MXC_F_RPU_SDHCCTRL_SYS0ACN_POS                 3 /**< SDHCCTRL_SYS0ACN Position */
 #define MXC_F_RPU_SDHCCTRL_SYS0ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_SYS0ACN_POS)) /**< SDHCCTRL_SYS0ACN Mask */

 #define MXC_F_RPU_SDHCCTRL_SYS1ACN_POS                 4 /**< SDHCCTRL_SYS1ACN Position */
 #define MXC_F_RPU_SDHCCTRL_SYS1ACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_SYS1ACN_POS)) /**< SDHCCTRL_SYS1ACN Mask */

 #define MXC_F_RPU_SDHCCTRL_SDMADACN_POS                5 /**< SDHCCTRL_SDMADACN Position */
 #define MXC_F_RPU_SDHCCTRL_SDMADACN                    ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_SDMADACN_POS)) /**< SDHCCTRL_SDMADACN Mask */

 #define MXC_F_RPU_SDHCCTRL_SDMAIACN_POS                6 /**< SDHCCTRL_SDMAIACN Position */
 #define MXC_F_RPU_SDHCCTRL_SDMAIACN                    ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_SDMAIACN_POS)) /**< SDHCCTRL_SDMAIACN Mask */

 #define MXC_F_RPU_SDHCCTRL_CRYPTOACN_POS               7 /**< SDHCCTRL_CRYPTOACN Position */
 #define MXC_F_RPU_SDHCCTRL_CRYPTOACN                   ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_CRYPTOACN_POS)) /**< SDHCCTRL_CRYPTOACN Mask */

 #define MXC_F_RPU_SDHCCTRL_SDIOACN_POS                 8 /**< SDHCCTRL_SDIOACN Position */
 #define MXC_F_RPU_SDHCCTRL_SDIOACN                     ((uint32_t)(0x1UL << MXC_F_RPU_SDHCCTRL_SDIOACN_POS)) /**< SDHCCTRL_SDIOACN Mask */

/**@} end of group RPU_SDHCCTRL_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SPID RPU_SPID
 * @brief    SPI Data Controller Protection Register
 * @{
 */
 #define MXC_F_RPU_SPID_DMA0ACN_POS                     0 /**< SPID_DMA0ACN Position */
 #define MXC_F_RPU_SPID_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SPID_DMA0ACN_POS)) /**< SPID_DMA0ACN Mask */

 #define MXC_F_RPU_SPID_DMA1ACN_POS                     1 /**< SPID_DMA1ACN Position */
 #define MXC_F_RPU_SPID_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SPID_DMA1ACN_POS)) /**< SPID_DMA1ACN Mask */

 #define MXC_F_RPU_SPID_USBACN_POS                      2 /**< SPID_USBACN Position */
 #define MXC_F_RPU_SPID_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SPID_USBACN_POS)) /**< SPID_USBACN Mask */

 #define MXC_F_RPU_SPID_SYS0ACN_POS                     3 /**< SPID_SYS0ACN Position */
 #define MXC_F_RPU_SPID_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SPID_SYS0ACN_POS)) /**< SPID_SYS0ACN Mask */

 #define MXC_F_RPU_SPID_SYS1ACN_POS                     4 /**< SPID_SYS1ACN Position */
 #define MXC_F_RPU_SPID_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SPID_SYS1ACN_POS)) /**< SPID_SYS1ACN Mask */

 #define MXC_F_RPU_SPID_SDMADACN_POS                    5 /**< SPID_SDMADACN Position */
 #define MXC_F_RPU_SPID_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SPID_SDMADACN_POS)) /**< SPID_SDMADACN Mask */

 #define MXC_F_RPU_SPID_SDMAIACN_POS                    6 /**< SPID_SDMAIACN Position */
 #define MXC_F_RPU_SPID_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SPID_SDMAIACN_POS)) /**< SPID_SDMAIACN Mask */

 #define MXC_F_RPU_SPID_CRYPTOACN_POS                   7 /**< SPID_CRYPTOACN Position */
 #define MXC_F_RPU_SPID_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_SPID_CRYPTOACN_POS)) /**< SPID_CRYPTOACN Mask */

 #define MXC_F_RPU_SPID_SDIOACN_POS                     8 /**< SPID_SDIOACN Position */
 #define MXC_F_RPU_SPID_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SPID_SDIOACN_POS)) /**< SPID_SDIOACN Mask */

/**@} end of group RPU_SPID_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_PT RPU_PT
 * @brief    Pulse Train Protection Register
 * @{
 */
 #define MXC_F_RPU_PT_DMA0ACN_POS                       0 /**< PT_DMA0ACN Position */
 #define MXC_F_RPU_PT_DMA0ACN                           ((uint32_t)(0x1UL << MXC_F_RPU_PT_DMA0ACN_POS)) /**< PT_DMA0ACN Mask */

 #define MXC_F_RPU_PT_DMA1ACN_POS                       1 /**< PT_DMA1ACN Position */
 #define MXC_F_RPU_PT_DMA1ACN                           ((uint32_t)(0x1UL << MXC_F_RPU_PT_DMA1ACN_POS)) /**< PT_DMA1ACN Mask */

 #define MXC_F_RPU_PT_USBACN_POS                        2 /**< PT_USBACN Position */
 #define MXC_F_RPU_PT_USBACN                            ((uint32_t)(0x1UL << MXC_F_RPU_PT_USBACN_POS)) /**< PT_USBACN Mask */

 #define MXC_F_RPU_PT_SYS0ACN_POS                       3 /**< PT_SYS0ACN Position */
 #define MXC_F_RPU_PT_SYS0ACN                           ((uint32_t)(0x1UL << MXC_F_RPU_PT_SYS0ACN_POS)) /**< PT_SYS0ACN Mask */

 #define MXC_F_RPU_PT_SYS1ACN_POS                       4 /**< PT_SYS1ACN Position */
 #define MXC_F_RPU_PT_SYS1ACN                           ((uint32_t)(0x1UL << MXC_F_RPU_PT_SYS1ACN_POS)) /**< PT_SYS1ACN Mask */

 #define MXC_F_RPU_PT_SDMADACN_POS                      5 /**< PT_SDMADACN Position */
 #define MXC_F_RPU_PT_SDMADACN                          ((uint32_t)(0x1UL << MXC_F_RPU_PT_SDMADACN_POS)) /**< PT_SDMADACN Mask */

 #define MXC_F_RPU_PT_SDMAIACN_POS                      6 /**< PT_SDMAIACN Position */
 #define MXC_F_RPU_PT_SDMAIACN                          ((uint32_t)(0x1UL << MXC_F_RPU_PT_SDMAIACN_POS)) /**< PT_SDMAIACN Mask */

 #define MXC_F_RPU_PT_CRYPTOACN_POS                     7 /**< PT_CRYPTOACN Position */
 #define MXC_F_RPU_PT_CRYPTOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_PT_CRYPTOACN_POS)) /**< PT_CRYPTOACN Mask */

 #define MXC_F_RPU_PT_SDIOACN_POS                       8 /**< PT_SDIOACN Position */
 #define MXC_F_RPU_PT_SDIOACN                           ((uint32_t)(0x1UL << MXC_F_RPU_PT_SDIOACN_POS)) /**< PT_SDIOACN Mask */

/**@} end of group RPU_PT_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_OWM RPU_OWM
 * @brief    One Wire Master Protection Register
 * @{
 */
 #define MXC_F_RPU_OWM_DMA0ACN_POS                      0 /**< OWM_DMA0ACN Position */
 #define MXC_F_RPU_OWM_DMA0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_OWM_DMA0ACN_POS)) /**< OWM_DMA0ACN Mask */

 #define MXC_F_RPU_OWM_DMA1ACN_POS                      1 /**< OWM_DMA1ACN Position */
 #define MXC_F_RPU_OWM_DMA1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_OWM_DMA1ACN_POS)) /**< OWM_DMA1ACN Mask */

 #define MXC_F_RPU_OWM_USBACN_POS                       2 /**< OWM_USBACN Position */
 #define MXC_F_RPU_OWM_USBACN                           ((uint32_t)(0x1UL << MXC_F_RPU_OWM_USBACN_POS)) /**< OWM_USBACN Mask */

 #define MXC_F_RPU_OWM_SYS0ACN_POS                      3 /**< OWM_SYS0ACN Position */
 #define MXC_F_RPU_OWM_SYS0ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_OWM_SYS0ACN_POS)) /**< OWM_SYS0ACN Mask */

 #define MXC_F_RPU_OWM_SYS1ACN_POS                      4 /**< OWM_SYS1ACN Position */
 #define MXC_F_RPU_OWM_SYS1ACN                          ((uint32_t)(0x1UL << MXC_F_RPU_OWM_SYS1ACN_POS)) /**< OWM_SYS1ACN Mask */

 #define MXC_F_RPU_OWM_SDMADACN_POS                     5 /**< OWM_SDMADACN Position */
 #define MXC_F_RPU_OWM_SDMADACN                         ((uint32_t)(0x1UL << MXC_F_RPU_OWM_SDMADACN_POS)) /**< OWM_SDMADACN Mask */

 #define MXC_F_RPU_OWM_SDMAIACN_POS                     6 /**< OWM_SDMAIACN Position */
 #define MXC_F_RPU_OWM_SDMAIACN                         ((uint32_t)(0x1UL << MXC_F_RPU_OWM_SDMAIACN_POS)) /**< OWM_SDMAIACN Mask */

 #define MXC_F_RPU_OWM_CRYPTOACN_POS                    7 /**< OWM_CRYPTOACN Position */
 #define MXC_F_RPU_OWM_CRYPTOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_OWM_CRYPTOACN_POS)) /**< OWM_CRYPTOACN Mask */

 #define MXC_F_RPU_OWM_SDIOACN_POS                      8 /**< OWM_SDIOACN Position */
 #define MXC_F_RPU_OWM_SDIOACN                          ((uint32_t)(0x1UL << MXC_F_RPU_OWM_SDIOACN_POS)) /**< OWM_SDIOACN Mask */

/**@} end of group RPU_OWM_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SEMA RPU_SEMA
 * @brief    Semaphores Protection Register
 * @{
 */
 #define MXC_F_RPU_SEMA_DMA0ACN_POS                     0 /**< SEMA_DMA0ACN Position */
 #define MXC_F_RPU_SEMA_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_DMA0ACN_POS)) /**< SEMA_DMA0ACN Mask */

 #define MXC_F_RPU_SEMA_DMA1ACN_POS                     1 /**< SEMA_DMA1ACN Position */
 #define MXC_F_RPU_SEMA_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_DMA1ACN_POS)) /**< SEMA_DMA1ACN Mask */

 #define MXC_F_RPU_SEMA_USBACN_POS                      2 /**< SEMA_USBACN Position */
 #define MXC_F_RPU_SEMA_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_USBACN_POS)) /**< SEMA_USBACN Mask */

 #define MXC_F_RPU_SEMA_SYS0ACN_POS                     3 /**< SEMA_SYS0ACN Position */
 #define MXC_F_RPU_SEMA_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_SYS0ACN_POS)) /**< SEMA_SYS0ACN Mask */

 #define MXC_F_RPU_SEMA_SYS1ACN_POS                     4 /**< SEMA_SYS1ACN Position */
 #define MXC_F_RPU_SEMA_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_SYS1ACN_POS)) /**< SEMA_SYS1ACN Mask */

 #define MXC_F_RPU_SEMA_SDMADACN_POS                    5 /**< SEMA_SDMADACN Position */
 #define MXC_F_RPU_SEMA_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_SDMADACN_POS)) /**< SEMA_SDMADACN Mask */

 #define MXC_F_RPU_SEMA_SDMAIACN_POS                    6 /**< SEMA_SDMAIACN Position */
 #define MXC_F_RPU_SEMA_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_SDMAIACN_POS)) /**< SEMA_SDMAIACN Mask */

 #define MXC_F_RPU_SEMA_CRYPTOACN_POS                   7 /**< SEMA_CRYPTOACN Position */
 #define MXC_F_RPU_SEMA_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_CRYPTOACN_POS)) /**< SEMA_CRYPTOACN Mask */

 #define MXC_F_RPU_SEMA_SDIOACN_POS                     8 /**< SEMA_SDIOACN Position */
 #define MXC_F_RPU_SEMA_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_SEMA_SDIOACN_POS)) /**< SEMA_SDIOACN Mask */

/**@} end of group RPU_SEMA_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_UART0 RPU_UART0
 * @brief    UART0 Protection Register
 * @{
 */
 #define MXC_F_RPU_UART0_DMA0ACN_POS                    0 /**< UART0_DMA0ACN Position */
 #define MXC_F_RPU_UART0_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART0_DMA0ACN_POS)) /**< UART0_DMA0ACN Mask */

 #define MXC_F_RPU_UART0_DMA1ACN_POS                    1 /**< UART0_DMA1ACN Position */
 #define MXC_F_RPU_UART0_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART0_DMA1ACN_POS)) /**< UART0_DMA1ACN Mask */

 #define MXC_F_RPU_UART0_USBACN_POS                     2 /**< UART0_USBACN Position */
 #define MXC_F_RPU_UART0_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_UART0_USBACN_POS)) /**< UART0_USBACN Mask */

 #define MXC_F_RPU_UART0_SYS0ACN_POS                    3 /**< UART0_SYS0ACN Position */
 #define MXC_F_RPU_UART0_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART0_SYS0ACN_POS)) /**< UART0_SYS0ACN Mask */

 #define MXC_F_RPU_UART0_SYS1ACN_POS                    4 /**< UART0_SYS1ACN Position */
 #define MXC_F_RPU_UART0_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART0_SYS1ACN_POS)) /**< UART0_SYS1ACN Mask */

 #define MXC_F_RPU_UART0_SDMADACN_POS                   5 /**< UART0_SDMADACN Position */
 #define MXC_F_RPU_UART0_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_UART0_SDMADACN_POS)) /**< UART0_SDMADACN Mask */

 #define MXC_F_RPU_UART0_SDMAIACN_POS                   6 /**< UART0_SDMAIACN Position */
 #define MXC_F_RPU_UART0_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_UART0_SDMAIACN_POS)) /**< UART0_SDMAIACN Mask */

 #define MXC_F_RPU_UART0_CRYPTOACN_POS                  7 /**< UART0_CRYPTOACN Position */
 #define MXC_F_RPU_UART0_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_UART0_CRYPTOACN_POS)) /**< UART0_CRYPTOACN Mask */

 #define MXC_F_RPU_UART0_SDIOACN_POS                    8 /**< UART0_SDIOACN Position */
 #define MXC_F_RPU_UART0_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART0_SDIOACN_POS)) /**< UART0_SDIOACN Mask */

/**@} end of group RPU_UART0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_UART1 RPU_UART1
 * @brief    UART1 Protection Register
 * @{
 */
 #define MXC_F_RPU_UART1_DMA0ACN_POS                    0 /**< UART1_DMA0ACN Position */
 #define MXC_F_RPU_UART1_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART1_DMA0ACN_POS)) /**< UART1_DMA0ACN Mask */

 #define MXC_F_RPU_UART1_DMA1ACN_POS                    1 /**< UART1_DMA1ACN Position */
 #define MXC_F_RPU_UART1_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART1_DMA1ACN_POS)) /**< UART1_DMA1ACN Mask */

 #define MXC_F_RPU_UART1_USBACN_POS                     2 /**< UART1_USBACN Position */
 #define MXC_F_RPU_UART1_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_UART1_USBACN_POS)) /**< UART1_USBACN Mask */

 #define MXC_F_RPU_UART1_SYS0ACN_POS                    3 /**< UART1_SYS0ACN Position */
 #define MXC_F_RPU_UART1_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART1_SYS0ACN_POS)) /**< UART1_SYS0ACN Mask */

 #define MXC_F_RPU_UART1_SYS1ACN_POS                    4 /**< UART1_SYS1ACN Position */
 #define MXC_F_RPU_UART1_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART1_SYS1ACN_POS)) /**< UART1_SYS1ACN Mask */

 #define MXC_F_RPU_UART1_SDMADACN_POS                   5 /**< UART1_SDMADACN Position */
 #define MXC_F_RPU_UART1_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_UART1_SDMADACN_POS)) /**< UART1_SDMADACN Mask */

 #define MXC_F_RPU_UART1_SDMAIACN_POS                   6 /**< UART1_SDMAIACN Position */
 #define MXC_F_RPU_UART1_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_UART1_SDMAIACN_POS)) /**< UART1_SDMAIACN Mask */

 #define MXC_F_RPU_UART1_CRYPTOACN_POS                  7 /**< UART1_CRYPTOACN Position */
 #define MXC_F_RPU_UART1_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_UART1_CRYPTOACN_POS)) /**< UART1_CRYPTOACN Mask */

 #define MXC_F_RPU_UART1_SDIOACN_POS                    8 /**< UART1_SDIOACN Position */
 #define MXC_F_RPU_UART1_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART1_SDIOACN_POS)) /**< UART1_SDIOACN Mask */

/**@} end of group RPU_UART1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_UART2 RPU_UART2
 * @brief    UART2 Protection Register
 * @{
 */
 #define MXC_F_RPU_UART2_DMA0ACN_POS                    0 /**< UART2_DMA0ACN Position */
 #define MXC_F_RPU_UART2_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART2_DMA0ACN_POS)) /**< UART2_DMA0ACN Mask */

 #define MXC_F_RPU_UART2_DMA1ACN_POS                    1 /**< UART2_DMA1ACN Position */
 #define MXC_F_RPU_UART2_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART2_DMA1ACN_POS)) /**< UART2_DMA1ACN Mask */

 #define MXC_F_RPU_UART2_USBACN_POS                     2 /**< UART2_USBACN Position */
 #define MXC_F_RPU_UART2_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_UART2_USBACN_POS)) /**< UART2_USBACN Mask */

 #define MXC_F_RPU_UART2_SYS0ACN_POS                    3 /**< UART2_SYS0ACN Position */
 #define MXC_F_RPU_UART2_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART2_SYS0ACN_POS)) /**< UART2_SYS0ACN Mask */

 #define MXC_F_RPU_UART2_SYS1ACN_POS                    4 /**< UART2_SYS1ACN Position */
 #define MXC_F_RPU_UART2_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART2_SYS1ACN_POS)) /**< UART2_SYS1ACN Mask */

 #define MXC_F_RPU_UART2_SDMADACN_POS                   5 /**< UART2_SDMADACN Position */
 #define MXC_F_RPU_UART2_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_UART2_SDMADACN_POS)) /**< UART2_SDMADACN Mask */

 #define MXC_F_RPU_UART2_SDMAIACN_POS                   6 /**< UART2_SDMAIACN Position */
 #define MXC_F_RPU_UART2_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_UART2_SDMAIACN_POS)) /**< UART2_SDMAIACN Mask */

 #define MXC_F_RPU_UART2_CRYPTOACN_POS                  7 /**< UART2_CRYPTOACN Position */
 #define MXC_F_RPU_UART2_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_UART2_CRYPTOACN_POS)) /**< UART2_CRYPTOACN Mask */

 #define MXC_F_RPU_UART2_SDIOACN_POS                    8 /**< UART2_SDIOACN Position */
 #define MXC_F_RPU_UART2_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_UART2_SDIOACN_POS)) /**< UART2_SDIOACN Mask */

/**@} end of group RPU_UART2_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_QSPI1 RPU_QSPI1
 * @brief    QSPI1 Protection Register
 * @{
 */
 #define MXC_F_RPU_QSPI1_DMA0ACN_POS                    0 /**< QSPI1_DMA0ACN Position */
 #define MXC_F_RPU_QSPI1_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_DMA0ACN_POS)) /**< QSPI1_DMA0ACN Mask */

 #define MXC_F_RPU_QSPI1_DMA1ACN_POS                    1 /**< QSPI1_DMA1ACN Position */
 #define MXC_F_RPU_QSPI1_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_DMA1ACN_POS)) /**< QSPI1_DMA1ACN Mask */

 #define MXC_F_RPU_QSPI1_USBACN_POS                     2 /**< QSPI1_USBACN Position */
 #define MXC_F_RPU_QSPI1_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_USBACN_POS)) /**< QSPI1_USBACN Mask */

 #define MXC_F_RPU_QSPI1_SYS0ACN_POS                    3 /**< QSPI1_SYS0ACN Position */
 #define MXC_F_RPU_QSPI1_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_SYS0ACN_POS)) /**< QSPI1_SYS0ACN Mask */

 #define MXC_F_RPU_QSPI1_SYS1ACN_POS                    4 /**< QSPI1_SYS1ACN Position */
 #define MXC_F_RPU_QSPI1_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_SYS1ACN_POS)) /**< QSPI1_SYS1ACN Mask */

 #define MXC_F_RPU_QSPI1_SDMADACN_POS                   5 /**< QSPI1_SDMADACN Position */
 #define MXC_F_RPU_QSPI1_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_SDMADACN_POS)) /**< QSPI1_SDMADACN Mask */

 #define MXC_F_RPU_QSPI1_SDMAIACN_POS                   6 /**< QSPI1_SDMAIACN Position */
 #define MXC_F_RPU_QSPI1_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_SDMAIACN_POS)) /**< QSPI1_SDMAIACN Mask */

 #define MXC_F_RPU_QSPI1_CRYPTOACN_POS                  7 /**< QSPI1_CRYPTOACN Position */
 #define MXC_F_RPU_QSPI1_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_CRYPTOACN_POS)) /**< QSPI1_CRYPTOACN Mask */

 #define MXC_F_RPU_QSPI1_SDIOACN_POS                    8 /**< QSPI1_SDIOACN Position */
 #define MXC_F_RPU_QSPI1_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI1_SDIOACN_POS)) /**< QSPI1_SDIOACN Mask */

/**@} end of group RPU_QSPI1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_QSPI2 RPU_QSPI2
 * @brief    QSPI2 Protection Register
 * @{
 */
 #define MXC_F_RPU_QSPI2_DMA0ACN_POS                    0 /**< QSPI2_DMA0ACN Position */
 #define MXC_F_RPU_QSPI2_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_DMA0ACN_POS)) /**< QSPI2_DMA0ACN Mask */

 #define MXC_F_RPU_QSPI2_DMA1ACN_POS                    1 /**< QSPI2_DMA1ACN Position */
 #define MXC_F_RPU_QSPI2_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_DMA1ACN_POS)) /**< QSPI2_DMA1ACN Mask */

 #define MXC_F_RPU_QSPI2_USBACN_POS                     2 /**< QSPI2_USBACN Position */
 #define MXC_F_RPU_QSPI2_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_USBACN_POS)) /**< QSPI2_USBACN Mask */

 #define MXC_F_RPU_QSPI2_SYS0ACN_POS                    3 /**< QSPI2_SYS0ACN Position */
 #define MXC_F_RPU_QSPI2_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_SYS0ACN_POS)) /**< QSPI2_SYS0ACN Mask */

 #define MXC_F_RPU_QSPI2_SYS1ACN_POS                    4 /**< QSPI2_SYS1ACN Position */
 #define MXC_F_RPU_QSPI2_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_SYS1ACN_POS)) /**< QSPI2_SYS1ACN Mask */

 #define MXC_F_RPU_QSPI2_SDMADACN_POS                   5 /**< QSPI2_SDMADACN Position */
 #define MXC_F_RPU_QSPI2_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_SDMADACN_POS)) /**< QSPI2_SDMADACN Mask */

 #define MXC_F_RPU_QSPI2_SDMAIACN_POS                   6 /**< QSPI2_SDMAIACN Position */
 #define MXC_F_RPU_QSPI2_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_SDMAIACN_POS)) /**< QSPI2_SDMAIACN Mask */

 #define MXC_F_RPU_QSPI2_CRYPTOACN_POS                  7 /**< QSPI2_CRYPTOACN Position */
 #define MXC_F_RPU_QSPI2_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_CRYPTOACN_POS)) /**< QSPI2_CRYPTOACN Mask */

 #define MXC_F_RPU_QSPI2_SDIOACN_POS                    8 /**< QSPI2_SDIOACN Position */
 #define MXC_F_RPU_QSPI2_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI2_SDIOACN_POS)) /**< QSPI2_SDIOACN Mask */

/**@} end of group RPU_QSPI2_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_AUDIO RPU_AUDIO
 * @brief    Audio Subsystem Protection Register
 * @{
 */
 #define MXC_F_RPU_AUDIO_DMA0ACN_POS                    0 /**< AUDIO_DMA0ACN Position */
 #define MXC_F_RPU_AUDIO_DMA0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_DMA0ACN_POS)) /**< AUDIO_DMA0ACN Mask */

 #define MXC_F_RPU_AUDIO_DMA1ACN_POS                    1 /**< AUDIO_DMA1ACN Position */
 #define MXC_F_RPU_AUDIO_DMA1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_DMA1ACN_POS)) /**< AUDIO_DMA1ACN Mask */

 #define MXC_F_RPU_AUDIO_USBACN_POS                     2 /**< AUDIO_USBACN Position */
 #define MXC_F_RPU_AUDIO_USBACN                         ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_USBACN_POS)) /**< AUDIO_USBACN Mask */

 #define MXC_F_RPU_AUDIO_SYS0ACN_POS                    3 /**< AUDIO_SYS0ACN Position */
 #define MXC_F_RPU_AUDIO_SYS0ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_SYS0ACN_POS)) /**< AUDIO_SYS0ACN Mask */

 #define MXC_F_RPU_AUDIO_SYS1ACN_POS                    4 /**< AUDIO_SYS1ACN Position */
 #define MXC_F_RPU_AUDIO_SYS1ACN                        ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_SYS1ACN_POS)) /**< AUDIO_SYS1ACN Mask */

 #define MXC_F_RPU_AUDIO_SDMADACN_POS                   5 /**< AUDIO_SDMADACN Position */
 #define MXC_F_RPU_AUDIO_SDMADACN                       ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_SDMADACN_POS)) /**< AUDIO_SDMADACN Mask */

 #define MXC_F_RPU_AUDIO_SDMAIACN_POS                   6 /**< AUDIO_SDMAIACN Position */
 #define MXC_F_RPU_AUDIO_SDMAIACN                       ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_SDMAIACN_POS)) /**< AUDIO_SDMAIACN Mask */

 #define MXC_F_RPU_AUDIO_CRYPTOACN_POS                  7 /**< AUDIO_CRYPTOACN Position */
 #define MXC_F_RPU_AUDIO_CRYPTOACN                      ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_CRYPTOACN_POS)) /**< AUDIO_CRYPTOACN Mask */

 #define MXC_F_RPU_AUDIO_SDIOACN_POS                    8 /**< AUDIO_SDIOACN Position */
 #define MXC_F_RPU_AUDIO_SDIOACN                        ((uint32_t)(0x1UL << MXC_F_RPU_AUDIO_SDIOACN_POS)) /**< AUDIO_SDIOACN Mask */

/**@} end of group RPU_AUDIO_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_TRNG RPU_TRNG
 * @brief    TRNG Protection Register
 * @{
 */
 #define MXC_F_RPU_TRNG_DMA0ACN_POS                     0 /**< TRNG_DMA0ACN Position */
 #define MXC_F_RPU_TRNG_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_DMA0ACN_POS)) /**< TRNG_DMA0ACN Mask */

 #define MXC_F_RPU_TRNG_DMA1ACN_POS                     1 /**< TRNG_DMA1ACN Position */
 #define MXC_F_RPU_TRNG_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_DMA1ACN_POS)) /**< TRNG_DMA1ACN Mask */

 #define MXC_F_RPU_TRNG_USBACN_POS                      2 /**< TRNG_USBACN Position */
 #define MXC_F_RPU_TRNG_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_USBACN_POS)) /**< TRNG_USBACN Mask */

 #define MXC_F_RPU_TRNG_SYS0ACN_POS                     3 /**< TRNG_SYS0ACN Position */
 #define MXC_F_RPU_TRNG_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_SYS0ACN_POS)) /**< TRNG_SYS0ACN Mask */

 #define MXC_F_RPU_TRNG_SYS1ACN_POS                     4 /**< TRNG_SYS1ACN Position */
 #define MXC_F_RPU_TRNG_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_SYS1ACN_POS)) /**< TRNG_SYS1ACN Mask */

 #define MXC_F_RPU_TRNG_SDMADACN_POS                    5 /**< TRNG_SDMADACN Position */
 #define MXC_F_RPU_TRNG_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_SDMADACN_POS)) /**< TRNG_SDMADACN Mask */

 #define MXC_F_RPU_TRNG_SDMAIACN_POS                    6 /**< TRNG_SDMAIACN Position */
 #define MXC_F_RPU_TRNG_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_SDMAIACN_POS)) /**< TRNG_SDMAIACN Mask */

 #define MXC_F_RPU_TRNG_CRYPTOACN_POS                   7 /**< TRNG_CRYPTOACN Position */
 #define MXC_F_RPU_TRNG_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_CRYPTOACN_POS)) /**< TRNG_CRYPTOACN Mask */

 #define MXC_F_RPU_TRNG_SDIOACN_POS                     8 /**< TRNG_SDIOACN Position */
 #define MXC_F_RPU_TRNG_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_TRNG_SDIOACN_POS)) /**< TRNG_SDIOACN Mask */

/**@} end of group RPU_TRNG_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_BTLE RPU_BTLE
 * @brief    BTLE Registers Protection Register
 * @{
 */
 #define MXC_F_RPU_BTLE_DMA0ACN_POS                     0 /**< BTLE_DMA0ACN Position */
 #define MXC_F_RPU_BTLE_DMA0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_DMA0ACN_POS)) /**< BTLE_DMA0ACN Mask */

 #define MXC_F_RPU_BTLE_DMA1ACN_POS                     1 /**< BTLE_DMA1ACN Position */
 #define MXC_F_RPU_BTLE_DMA1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_DMA1ACN_POS)) /**< BTLE_DMA1ACN Mask */

 #define MXC_F_RPU_BTLE_USBACN_POS                      2 /**< BTLE_USBACN Position */
 #define MXC_F_RPU_BTLE_USBACN                          ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_USBACN_POS)) /**< BTLE_USBACN Mask */

 #define MXC_F_RPU_BTLE_SYS0ACN_POS                     3 /**< BTLE_SYS0ACN Position */
 #define MXC_F_RPU_BTLE_SYS0ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_SYS0ACN_POS)) /**< BTLE_SYS0ACN Mask */

 #define MXC_F_RPU_BTLE_SYS1ACN_POS                     4 /**< BTLE_SYS1ACN Position */
 #define MXC_F_RPU_BTLE_SYS1ACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_SYS1ACN_POS)) /**< BTLE_SYS1ACN Mask */

 #define MXC_F_RPU_BTLE_SDMADACN_POS                    5 /**< BTLE_SDMADACN Position */
 #define MXC_F_RPU_BTLE_SDMADACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_SDMADACN_POS)) /**< BTLE_SDMADACN Mask */

 #define MXC_F_RPU_BTLE_SDMAIACN_POS                    6 /**< BTLE_SDMAIACN Position */
 #define MXC_F_RPU_BTLE_SDMAIACN                        ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_SDMAIACN_POS)) /**< BTLE_SDMAIACN Mask */

 #define MXC_F_RPU_BTLE_CRYPTOACN_POS                   7 /**< BTLE_CRYPTOACN Position */
 #define MXC_F_RPU_BTLE_CRYPTOACN                       ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_CRYPTOACN_POS)) /**< BTLE_CRYPTOACN Mask */

 #define MXC_F_RPU_BTLE_SDIOACN_POS                     8 /**< BTLE_SDIOACN Position */
 #define MXC_F_RPU_BTLE_SDIOACN                         ((uint32_t)(0x1UL << MXC_F_RPU_BTLE_SDIOACN_POS)) /**< BTLE_SDIOACN Mask */

/**@} end of group RPU_BTLE_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_USBHS RPU_USBHS
 * @brief    USBHS Protection Register
 * @{
 */
 #define MXC_F_RPU_USBHS_DMA0ACNR_POS                   0 /**< USBHS_DMA0ACNR Position */
 #define MXC_F_RPU_USBHS_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_DMA0ACNR_POS)) /**< USBHS_DMA0ACNR Mask */

 #define MXC_F_RPU_USBHS_DMA0ACNW_POS                   1 /**< USBHS_DMA0ACNW Position */
 #define MXC_F_RPU_USBHS_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_DMA0ACNW_POS)) /**< USBHS_DMA0ACNW Mask */

 #define MXC_F_RPU_USBHS_DMA1ACNR_POS                   2 /**< USBHS_DMA1ACNR Position */
 #define MXC_F_RPU_USBHS_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_DMA1ACNR_POS)) /**< USBHS_DMA1ACNR Mask */

 #define MXC_F_RPU_USBHS_DMA1ACNW_POS                   3 /**< USBHS_DMA1ACNW Position */
 #define MXC_F_RPU_USBHS_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_DMA1ACNW_POS)) /**< USBHS_DMA1ACNW Mask */

 #define MXC_F_RPU_USBHS_USBACNR_POS                    4 /**< USBHS_USBACNR Position */
 #define MXC_F_RPU_USBHS_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_USBACNR_POS)) /**< USBHS_USBACNR Mask */

 #define MXC_F_RPU_USBHS_USBACNW_POS                    5 /**< USBHS_USBACNW Position */
 #define MXC_F_RPU_USBHS_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_USBACNW_POS)) /**< USBHS_USBACNW Mask */

 #define MXC_F_RPU_USBHS_SYS0ACNR_POS                   6 /**< USBHS_SYS0ACNR Position */
 #define MXC_F_RPU_USBHS_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SYS0ACNR_POS)) /**< USBHS_SYS0ACNR Mask */

 #define MXC_F_RPU_USBHS_SYS0ACNW_POS                   7 /**< USBHS_SYS0ACNW Position */
 #define MXC_F_RPU_USBHS_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SYS0ACNW_POS)) /**< USBHS_SYS0ACNW Mask */

 #define MXC_F_RPU_USBHS_SYS1ACNR_POS                   8 /**< USBHS_SYS1ACNR Position */
 #define MXC_F_RPU_USBHS_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SYS1ACNR_POS)) /**< USBHS_SYS1ACNR Mask */

 #define MXC_F_RPU_USBHS_SYS1ACNW_POS                   9 /**< USBHS_SYS1ACNW Position */
 #define MXC_F_RPU_USBHS_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SYS1ACNW_POS)) /**< USBHS_SYS1ACNW Mask */

 #define MXC_F_RPU_USBHS_SDMADACNR_POS                  10 /**< USBHS_SDMADACNR Position */
 #define MXC_F_RPU_USBHS_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SDMADACNR_POS)) /**< USBHS_SDMADACNR Mask */

 #define MXC_F_RPU_USBHS_SDMADACNW_POS                  11 /**< USBHS_SDMADACNW Position */
 #define MXC_F_RPU_USBHS_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SDMADACNW_POS)) /**< USBHS_SDMADACNW Mask */

 #define MXC_F_RPU_USBHS_SDMAIACNR_POS                  12 /**< USBHS_SDMAIACNR Position */
 #define MXC_F_RPU_USBHS_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SDMAIACNR_POS)) /**< USBHS_SDMAIACNR Mask */

 #define MXC_F_RPU_USBHS_SDMAIACNW_POS                  13 /**< USBHS_SDMAIACNW Position */
 #define MXC_F_RPU_USBHS_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SDMAIACNW_POS)) /**< USBHS_SDMAIACNW Mask */

 #define MXC_F_RPU_USBHS_CRYPTOACNR_POS                 14 /**< USBHS_CRYPTOACNR Position */
 #define MXC_F_RPU_USBHS_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_CRYPTOACNR_POS)) /**< USBHS_CRYPTOACNR Mask */

 #define MXC_F_RPU_USBHS_CRYPTOACNW_POS                 15 /**< USBHS_CRYPTOACNW Position */
 #define MXC_F_RPU_USBHS_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_CRYPTOACNW_POS)) /**< USBHS_CRYPTOACNW Mask */

 #define MXC_F_RPU_USBHS_SDIOACNR_POS                   16 /**< USBHS_SDIOACNR Position */
 #define MXC_F_RPU_USBHS_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SDIOACNR_POS)) /**< USBHS_SDIOACNR Mask */

 #define MXC_F_RPU_USBHS_SDIOACNW_POS                   17 /**< USBHS_SDIOACNW Position */
 #define MXC_F_RPU_USBHS_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_USBHS_SDIOACNW_POS)) /**< USBHS_SDIOACNW Mask */

/**@} end of group RPU_USBHS_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SDIO RPU_SDIO
 * @brief    SDIO Protection Register
 * @{
 */
 #define MXC_F_RPU_SDIO_DMA0ACNR_POS                    0 /**< SDIO_DMA0ACNR Position */
 #define MXC_F_RPU_SDIO_DMA0ACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_DMA0ACNR_POS)) /**< SDIO_DMA0ACNR Mask */

 #define MXC_F_RPU_SDIO_DMA0ACNW_POS                    1 /**< SDIO_DMA0ACNW Position */
 #define MXC_F_RPU_SDIO_DMA0ACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_DMA0ACNW_POS)) /**< SDIO_DMA0ACNW Mask */

 #define MXC_F_RPU_SDIO_DMA1ACNR_POS                    2 /**< SDIO_DMA1ACNR Position */
 #define MXC_F_RPU_SDIO_DMA1ACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_DMA1ACNR_POS)) /**< SDIO_DMA1ACNR Mask */

 #define MXC_F_RPU_SDIO_DMA1ACNW_POS                    3 /**< SDIO_DMA1ACNW Position */
 #define MXC_F_RPU_SDIO_DMA1ACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_DMA1ACNW_POS)) /**< SDIO_DMA1ACNW Mask */

 #define MXC_F_RPU_SDIO_USBACNR_POS                     4 /**< SDIO_USBACNR Position */
 #define MXC_F_RPU_SDIO_USBACNR                         ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_USBACNR_POS)) /**< SDIO_USBACNR Mask */

 #define MXC_F_RPU_SDIO_USBACNW_POS                     5 /**< SDIO_USBACNW Position */
 #define MXC_F_RPU_SDIO_USBACNW                         ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_USBACNW_POS)) /**< SDIO_USBACNW Mask */

 #define MXC_F_RPU_SDIO_SYS0ACNR_POS                    6 /**< SDIO_SYS0ACNR Position */
 #define MXC_F_RPU_SDIO_SYS0ACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SYS0ACNR_POS)) /**< SDIO_SYS0ACNR Mask */

 #define MXC_F_RPU_SDIO_SYS0ACNW_POS                    7 /**< SDIO_SYS0ACNW Position */
 #define MXC_F_RPU_SDIO_SYS0ACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SYS0ACNW_POS)) /**< SDIO_SYS0ACNW Mask */

 #define MXC_F_RPU_SDIO_SYS1ACNR_POS                    8 /**< SDIO_SYS1ACNR Position */
 #define MXC_F_RPU_SDIO_SYS1ACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SYS1ACNR_POS)) /**< SDIO_SYS1ACNR Mask */

 #define MXC_F_RPU_SDIO_SYS1ACNW_POS                    9 /**< SDIO_SYS1ACNW Position */
 #define MXC_F_RPU_SDIO_SYS1ACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SYS1ACNW_POS)) /**< SDIO_SYS1ACNW Mask */

 #define MXC_F_RPU_SDIO_SDMADACNR_POS                   10 /**< SDIO_SDMADACNR Position */
 #define MXC_F_RPU_SDIO_SDMADACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SDMADACNR_POS)) /**< SDIO_SDMADACNR Mask */

 #define MXC_F_RPU_SDIO_SDMADACNW_POS                   11 /**< SDIO_SDMADACNW Position */
 #define MXC_F_RPU_SDIO_SDMADACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SDMADACNW_POS)) /**< SDIO_SDMADACNW Mask */

 #define MXC_F_RPU_SDIO_SDMAIACNR_POS                   12 /**< SDIO_SDMAIACNR Position */
 #define MXC_F_RPU_SDIO_SDMAIACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SDMAIACNR_POS)) /**< SDIO_SDMAIACNR Mask */

 #define MXC_F_RPU_SDIO_SDMAIACNW_POS                   13 /**< SDIO_SDMAIACNW Position */
 #define MXC_F_RPU_SDIO_SDMAIACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SDMAIACNW_POS)) /**< SDIO_SDMAIACNW Mask */

 #define MXC_F_RPU_SDIO_CRYPTOACNR_POS                  14 /**< SDIO_CRYPTOACNR Position */
 #define MXC_F_RPU_SDIO_CRYPTOACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_CRYPTOACNR_POS)) /**< SDIO_CRYPTOACNR Mask */

 #define MXC_F_RPU_SDIO_CRYPTOACNW_POS                  15 /**< SDIO_CRYPTOACNW Position */
 #define MXC_F_RPU_SDIO_CRYPTOACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_CRYPTOACNW_POS)) /**< SDIO_CRYPTOACNW Mask */

 #define MXC_F_RPU_SDIO_SDIOACNR_POS                    16 /**< SDIO_SDIOACNR Position */
 #define MXC_F_RPU_SDIO_SDIOACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SDIOACNR_POS)) /**< SDIO_SDIOACNR Mask */

 #define MXC_F_RPU_SDIO_SDIOACNW_POS                    17 /**< SDIO_SDIOACNW Position */
 #define MXC_F_RPU_SDIO_SDIOACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SDIO_SDIOACNW_POS)) /**< SDIO_SDIOACNW Mask */

/**@} end of group RPU_SDIO_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SPIXIPMFIFO RPU_SPIXIPMFIFO
 * @brief    SPI XIP Master FIFO Protection Register
 * @{
 */
 #define MXC_F_RPU_SPIXIPMFIFO_DMA0ACNR_POS             0 /**< SPIXIPMFIFO_DMA0ACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_DMA0ACNR                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_DMA0ACNR_POS)) /**< SPIXIPMFIFO_DMA0ACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_DMA0ACNW_POS             1 /**< SPIXIPMFIFO_DMA0ACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_DMA0ACNW                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_DMA0ACNW_POS)) /**< SPIXIPMFIFO_DMA0ACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_DMA1ACNR_POS             2 /**< SPIXIPMFIFO_DMA1ACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_DMA1ACNR                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_DMA1ACNR_POS)) /**< SPIXIPMFIFO_DMA1ACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_DMA1ACNW_POS             3 /**< SPIXIPMFIFO_DMA1ACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_DMA1ACNW                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_DMA1ACNW_POS)) /**< SPIXIPMFIFO_DMA1ACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_USBACNR_POS              4 /**< SPIXIPMFIFO_USBACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_USBACNR                  ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_USBACNR_POS)) /**< SPIXIPMFIFO_USBACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_USBACNW_POS              5 /**< SPIXIPMFIFO_USBACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_USBACNW                  ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_USBACNW_POS)) /**< SPIXIPMFIFO_USBACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SYS0ACNR_POS             6 /**< SPIXIPMFIFO_SYS0ACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SYS0ACNR                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SYS0ACNR_POS)) /**< SPIXIPMFIFO_SYS0ACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SYS0ACNW_POS             7 /**< SPIXIPMFIFO_SYS0ACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SYS0ACNW                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SYS0ACNW_POS)) /**< SPIXIPMFIFO_SYS0ACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SYS1ACNR_POS             8 /**< SPIXIPMFIFO_SYS1ACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SYS1ACNR                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SYS1ACNR_POS)) /**< SPIXIPMFIFO_SYS1ACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SYS1ACNW_POS             9 /**< SPIXIPMFIFO_SYS1ACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SYS1ACNW                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SYS1ACNW_POS)) /**< SPIXIPMFIFO_SYS1ACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SDMADACNR_POS            10 /**< SPIXIPMFIFO_SDMADACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SDMADACNR                ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SDMADACNR_POS)) /**< SPIXIPMFIFO_SDMADACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SDMADACNW_POS            11 /**< SPIXIPMFIFO_SDMADACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SDMADACNW                ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SDMADACNW_POS)) /**< SPIXIPMFIFO_SDMADACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SDMAIACNR_POS            12 /**< SPIXIPMFIFO_SDMAIACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SDMAIACNR                ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SDMAIACNR_POS)) /**< SPIXIPMFIFO_SDMAIACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SDMAIACNW_POS            13 /**< SPIXIPMFIFO_SDMAIACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SDMAIACNW                ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SDMAIACNW_POS)) /**< SPIXIPMFIFO_SDMAIACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_CRYPTOACNR_POS           14 /**< SPIXIPMFIFO_CRYPTOACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_CRYPTOACNR               ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_CRYPTOACNR_POS)) /**< SPIXIPMFIFO_CRYPTOACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_CRYPTOACNW_POS           15 /**< SPIXIPMFIFO_CRYPTOACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_CRYPTOACNW               ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_CRYPTOACNW_POS)) /**< SPIXIPMFIFO_CRYPTOACNW Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SDIOACNR_POS             16 /**< SPIXIPMFIFO_SDIOACNR Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SDIOACNR                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SDIOACNR_POS)) /**< SPIXIPMFIFO_SDIOACNR Mask */

 #define MXC_F_RPU_SPIXIPMFIFO_SDIOACNW_POS             17 /**< SPIXIPMFIFO_SDIOACNW Position */
 #define MXC_F_RPU_SPIXIPMFIFO_SDIOACNW                 ((uint32_t)(0x1UL << MXC_F_RPU_SPIXIPMFIFO_SDIOACNW_POS)) /**< SPIXIPMFIFO_SDIOACNW Mask */

/**@} end of group RPU_SPIXIPMFIFO_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_QSPI0 RPU_QSPI0
 * @brief    QSPI0 Protection Register
 * @{
 */
 #define MXC_F_RPU_QSPI0_DMA0ACNR_POS                   0 /**< QSPI0_DMA0ACNR Position */
 #define MXC_F_RPU_QSPI0_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_DMA0ACNR_POS)) /**< QSPI0_DMA0ACNR Mask */

 #define MXC_F_RPU_QSPI0_DMA0ACNW_POS                   1 /**< QSPI0_DMA0ACNW Position */
 #define MXC_F_RPU_QSPI0_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_DMA0ACNW_POS)) /**< QSPI0_DMA0ACNW Mask */

 #define MXC_F_RPU_QSPI0_DMA1ACNR_POS                   2 /**< QSPI0_DMA1ACNR Position */
 #define MXC_F_RPU_QSPI0_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_DMA1ACNR_POS)) /**< QSPI0_DMA1ACNR Mask */

 #define MXC_F_RPU_QSPI0_DMA1ACNW_POS                   3 /**< QSPI0_DMA1ACNW Position */
 #define MXC_F_RPU_QSPI0_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_DMA1ACNW_POS)) /**< QSPI0_DMA1ACNW Mask */

 #define MXC_F_RPU_QSPI0_USBACNR_POS                    4 /**< QSPI0_USBACNR Position */
 #define MXC_F_RPU_QSPI0_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_USBACNR_POS)) /**< QSPI0_USBACNR Mask */

 #define MXC_F_RPU_QSPI0_USBACNW_POS                    5 /**< QSPI0_USBACNW Position */
 #define MXC_F_RPU_QSPI0_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_USBACNW_POS)) /**< QSPI0_USBACNW Mask */

 #define MXC_F_RPU_QSPI0_SYS0ACNR_POS                   6 /**< QSPI0_SYS0ACNR Position */
 #define MXC_F_RPU_QSPI0_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SYS0ACNR_POS)) /**< QSPI0_SYS0ACNR Mask */

 #define MXC_F_RPU_QSPI0_SYS0ACNW_POS                   7 /**< QSPI0_SYS0ACNW Position */
 #define MXC_F_RPU_QSPI0_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SYS0ACNW_POS)) /**< QSPI0_SYS0ACNW Mask */

 #define MXC_F_RPU_QSPI0_SYS1ACNR_POS                   8 /**< QSPI0_SYS1ACNR Position */
 #define MXC_F_RPU_QSPI0_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SYS1ACNR_POS)) /**< QSPI0_SYS1ACNR Mask */

 #define MXC_F_RPU_QSPI0_SYS1ACNW_POS                   9 /**< QSPI0_SYS1ACNW Position */
 #define MXC_F_RPU_QSPI0_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SYS1ACNW_POS)) /**< QSPI0_SYS1ACNW Mask */

 #define MXC_F_RPU_QSPI0_SDMADACNR_POS                  10 /**< QSPI0_SDMADACNR Position */
 #define MXC_F_RPU_QSPI0_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SDMADACNR_POS)) /**< QSPI0_SDMADACNR Mask */

 #define MXC_F_RPU_QSPI0_SDMADACNW_POS                  11 /**< QSPI0_SDMADACNW Position */
 #define MXC_F_RPU_QSPI0_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SDMADACNW_POS)) /**< QSPI0_SDMADACNW Mask */

 #define MXC_F_RPU_QSPI0_SDMAIACNR_POS                  12 /**< QSPI0_SDMAIACNR Position */
 #define MXC_F_RPU_QSPI0_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SDMAIACNR_POS)) /**< QSPI0_SDMAIACNR Mask */

 #define MXC_F_RPU_QSPI0_SDMAIACNW_POS                  13 /**< QSPI0_SDMAIACNW Position */
 #define MXC_F_RPU_QSPI0_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SDMAIACNW_POS)) /**< QSPI0_SDMAIACNW Mask */

 #define MXC_F_RPU_QSPI0_CRYPTOACNR_POS                 14 /**< QSPI0_CRYPTOACNR Position */
 #define MXC_F_RPU_QSPI0_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_CRYPTOACNR_POS)) /**< QSPI0_CRYPTOACNR Mask */

 #define MXC_F_RPU_QSPI0_CRYPTOACNW_POS                 15 /**< QSPI0_CRYPTOACNW Position */
 #define MXC_F_RPU_QSPI0_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_CRYPTOACNW_POS)) /**< QSPI0_CRYPTOACNW Mask */

 #define MXC_F_RPU_QSPI0_SDIOACNR_POS                   16 /**< QSPI0_SDIOACNR Position */
 #define MXC_F_RPU_QSPI0_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SDIOACNR_POS)) /**< QSPI0_SDIOACNR Mask */

 #define MXC_F_RPU_QSPI0_SDIOACNW_POS                   17 /**< QSPI0_SDIOACNW Position */
 #define MXC_F_RPU_QSPI0_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_QSPI0_SDIOACNW_POS)) /**< QSPI0_SDIOACNW Mask */

/**@} end of group RPU_QSPI0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SRAM0 RPU_SRAM0
 * @brief    SRAM0 Protection Register
 * @{
 */
 #define MXC_F_RPU_SRAM0_DMA0ACNR_POS                   0 /**< SRAM0_DMA0ACNR Position */
 #define MXC_F_RPU_SRAM0_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_DMA0ACNR_POS)) /**< SRAM0_DMA0ACNR Mask */

 #define MXC_F_RPU_SRAM0_DMA0ACNW_POS                   1 /**< SRAM0_DMA0ACNW Position */
 #define MXC_F_RPU_SRAM0_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_DMA0ACNW_POS)) /**< SRAM0_DMA0ACNW Mask */

 #define MXC_F_RPU_SRAM0_DMA1ACNR_POS                   2 /**< SRAM0_DMA1ACNR Position */
 #define MXC_F_RPU_SRAM0_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_DMA1ACNR_POS)) /**< SRAM0_DMA1ACNR Mask */

 #define MXC_F_RPU_SRAM0_DMA1ACNW_POS                   3 /**< SRAM0_DMA1ACNW Position */
 #define MXC_F_RPU_SRAM0_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_DMA1ACNW_POS)) /**< SRAM0_DMA1ACNW Mask */

 #define MXC_F_RPU_SRAM0_USBACNR_POS                    4 /**< SRAM0_USBACNR Position */
 #define MXC_F_RPU_SRAM0_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_USBACNR_POS)) /**< SRAM0_USBACNR Mask */

 #define MXC_F_RPU_SRAM0_USBACNW_POS                    5 /**< SRAM0_USBACNW Position */
 #define MXC_F_RPU_SRAM0_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_USBACNW_POS)) /**< SRAM0_USBACNW Mask */

 #define MXC_F_RPU_SRAM0_SYS0ACNR_POS                   6 /**< SRAM0_SYS0ACNR Position */
 #define MXC_F_RPU_SRAM0_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SYS0ACNR_POS)) /**< SRAM0_SYS0ACNR Mask */

 #define MXC_F_RPU_SRAM0_SYS0ACNW_POS                   7 /**< SRAM0_SYS0ACNW Position */
 #define MXC_F_RPU_SRAM0_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SYS0ACNW_POS)) /**< SRAM0_SYS0ACNW Mask */

 #define MXC_F_RPU_SRAM0_SYS1ACNR_POS                   8 /**< SRAM0_SYS1ACNR Position */
 #define MXC_F_RPU_SRAM0_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SYS1ACNR_POS)) /**< SRAM0_SYS1ACNR Mask */

 #define MXC_F_RPU_SRAM0_SYS1ACNW_POS                   9 /**< SRAM0_SYS1ACNW Position */
 #define MXC_F_RPU_SRAM0_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SYS1ACNW_POS)) /**< SRAM0_SYS1ACNW Mask */

 #define MXC_F_RPU_SRAM0_SDMADACNR_POS                  10 /**< SRAM0_SDMADACNR Position */
 #define MXC_F_RPU_SRAM0_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SDMADACNR_POS)) /**< SRAM0_SDMADACNR Mask */

 #define MXC_F_RPU_SRAM0_SDMADACNW_POS                  11 /**< SRAM0_SDMADACNW Position */
 #define MXC_F_RPU_SRAM0_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SDMADACNW_POS)) /**< SRAM0_SDMADACNW Mask */

 #define MXC_F_RPU_SRAM0_SDMAIACNR_POS                  12 /**< SRAM0_SDMAIACNR Position */
 #define MXC_F_RPU_SRAM0_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SDMAIACNR_POS)) /**< SRAM0_SDMAIACNR Mask */

 #define MXC_F_RPU_SRAM0_SDMAIACNW_POS                  13 /**< SRAM0_SDMAIACNW Position */
 #define MXC_F_RPU_SRAM0_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SDMAIACNW_POS)) /**< SRAM0_SDMAIACNW Mask */

 #define MXC_F_RPU_SRAM0_CRYPTOACNR_POS                 14 /**< SRAM0_CRYPTOACNR Position */
 #define MXC_F_RPU_SRAM0_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_CRYPTOACNR_POS)) /**< SRAM0_CRYPTOACNR Mask */

 #define MXC_F_RPU_SRAM0_CRYPTOACNW_POS                 15 /**< SRAM0_CRYPTOACNW Position */
 #define MXC_F_RPU_SRAM0_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_CRYPTOACNW_POS)) /**< SRAM0_CRYPTOACNW Mask */

 #define MXC_F_RPU_SRAM0_SDIOACNR_POS                   16 /**< SRAM0_SDIOACNR Position */
 #define MXC_F_RPU_SRAM0_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SDIOACNR_POS)) /**< SRAM0_SDIOACNR Mask */

 #define MXC_F_RPU_SRAM0_SDIOACNW_POS                   17 /**< SRAM0_SDIOACNW Position */
 #define MXC_F_RPU_SRAM0_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM0_SDIOACNW_POS)) /**< SRAM0_SDIOACNW Mask */

/**@} end of group RPU_SRAM0_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SRAM1 RPU_SRAM1
 * @brief    SRAM1 Protection Register
 * @{
 */
 #define MXC_F_RPU_SRAM1_DMA0ACNR_POS                   0 /**< SRAM1_DMA0ACNR Position */
 #define MXC_F_RPU_SRAM1_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_DMA0ACNR_POS)) /**< SRAM1_DMA0ACNR Mask */

 #define MXC_F_RPU_SRAM1_DMA0ACNW_POS                   1 /**< SRAM1_DMA0ACNW Position */
 #define MXC_F_RPU_SRAM1_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_DMA0ACNW_POS)) /**< SRAM1_DMA0ACNW Mask */

 #define MXC_F_RPU_SRAM1_DMA1ACNR_POS                   2 /**< SRAM1_DMA1ACNR Position */
 #define MXC_F_RPU_SRAM1_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_DMA1ACNR_POS)) /**< SRAM1_DMA1ACNR Mask */

 #define MXC_F_RPU_SRAM1_DMA1ACNW_POS                   3 /**< SRAM1_DMA1ACNW Position */
 #define MXC_F_RPU_SRAM1_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_DMA1ACNW_POS)) /**< SRAM1_DMA1ACNW Mask */

 #define MXC_F_RPU_SRAM1_USBACNR_POS                    4 /**< SRAM1_USBACNR Position */
 #define MXC_F_RPU_SRAM1_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_USBACNR_POS)) /**< SRAM1_USBACNR Mask */

 #define MXC_F_RPU_SRAM1_USBACNW_POS                    5 /**< SRAM1_USBACNW Position */
 #define MXC_F_RPU_SRAM1_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_USBACNW_POS)) /**< SRAM1_USBACNW Mask */

 #define MXC_F_RPU_SRAM1_SYS0ACNR_POS                   6 /**< SRAM1_SYS0ACNR Position */
 #define MXC_F_RPU_SRAM1_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SYS0ACNR_POS)) /**< SRAM1_SYS0ACNR Mask */

 #define MXC_F_RPU_SRAM1_SYS0ACNW_POS                   7 /**< SRAM1_SYS0ACNW Position */
 #define MXC_F_RPU_SRAM1_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SYS0ACNW_POS)) /**< SRAM1_SYS0ACNW Mask */

 #define MXC_F_RPU_SRAM1_SYS1ACNR_POS                   8 /**< SRAM1_SYS1ACNR Position */
 #define MXC_F_RPU_SRAM1_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SYS1ACNR_POS)) /**< SRAM1_SYS1ACNR Mask */

 #define MXC_F_RPU_SRAM1_SYS1ACNW_POS                   9 /**< SRAM1_SYS1ACNW Position */
 #define MXC_F_RPU_SRAM1_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SYS1ACNW_POS)) /**< SRAM1_SYS1ACNW Mask */

 #define MXC_F_RPU_SRAM1_SDMADACNR_POS                  10 /**< SRAM1_SDMADACNR Position */
 #define MXC_F_RPU_SRAM1_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SDMADACNR_POS)) /**< SRAM1_SDMADACNR Mask */

 #define MXC_F_RPU_SRAM1_SDMADACNW_POS                  11 /**< SRAM1_SDMADACNW Position */
 #define MXC_F_RPU_SRAM1_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SDMADACNW_POS)) /**< SRAM1_SDMADACNW Mask */

 #define MXC_F_RPU_SRAM1_SDMAIACNR_POS                  12 /**< SRAM1_SDMAIACNR Position */
 #define MXC_F_RPU_SRAM1_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SDMAIACNR_POS)) /**< SRAM1_SDMAIACNR Mask */

 #define MXC_F_RPU_SRAM1_SDMAIACNW_POS                  13 /**< SRAM1_SDMAIACNW Position */
 #define MXC_F_RPU_SRAM1_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SDMAIACNW_POS)) /**< SRAM1_SDMAIACNW Mask */

 #define MXC_F_RPU_SRAM1_CRYPTOACNR_POS                 14 /**< SRAM1_CRYPTOACNR Position */
 #define MXC_F_RPU_SRAM1_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_CRYPTOACNR_POS)) /**< SRAM1_CRYPTOACNR Mask */

 #define MXC_F_RPU_SRAM1_CRYPTOACNW_POS                 15 /**< SRAM1_CRYPTOACNW Position */
 #define MXC_F_RPU_SRAM1_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_CRYPTOACNW_POS)) /**< SRAM1_CRYPTOACNW Mask */

 #define MXC_F_RPU_SRAM1_SDIOACNR_POS                   16 /**< SRAM1_SDIOACNR Position */
 #define MXC_F_RPU_SRAM1_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SDIOACNR_POS)) /**< SRAM1_SDIOACNR Mask */

 #define MXC_F_RPU_SRAM1_SDIOACNW_POS                   17 /**< SRAM1_SDIOACNW Position */
 #define MXC_F_RPU_SRAM1_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM1_SDIOACNW_POS)) /**< SRAM1_SDIOACNW Mask */

/**@} end of group RPU_SRAM1_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SRAM2 RPU_SRAM2
 * @brief    SRAM2 Protection Register
 * @{
 */
 #define MXC_F_RPU_SRAM2_DMA0ACNR_POS                   0 /**< SRAM2_DMA0ACNR Position */
 #define MXC_F_RPU_SRAM2_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_DMA0ACNR_POS)) /**< SRAM2_DMA0ACNR Mask */

 #define MXC_F_RPU_SRAM2_DMA0ACNW_POS                   1 /**< SRAM2_DMA0ACNW Position */
 #define MXC_F_RPU_SRAM2_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_DMA0ACNW_POS)) /**< SRAM2_DMA0ACNW Mask */

 #define MXC_F_RPU_SRAM2_DMA1ACNR_POS                   2 /**< SRAM2_DMA1ACNR Position */
 #define MXC_F_RPU_SRAM2_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_DMA1ACNR_POS)) /**< SRAM2_DMA1ACNR Mask */

 #define MXC_F_RPU_SRAM2_DMA1ACNW_POS                   3 /**< SRAM2_DMA1ACNW Position */
 #define MXC_F_RPU_SRAM2_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_DMA1ACNW_POS)) /**< SRAM2_DMA1ACNW Mask */

 #define MXC_F_RPU_SRAM2_USBACNR_POS                    4 /**< SRAM2_USBACNR Position */
 #define MXC_F_RPU_SRAM2_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_USBACNR_POS)) /**< SRAM2_USBACNR Mask */

 #define MXC_F_RPU_SRAM2_USBACNW_POS                    5 /**< SRAM2_USBACNW Position */
 #define MXC_F_RPU_SRAM2_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_USBACNW_POS)) /**< SRAM2_USBACNW Mask */

 #define MXC_F_RPU_SRAM2_SYS0ACNR_POS                   6 /**< SRAM2_SYS0ACNR Position */
 #define MXC_F_RPU_SRAM2_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SYS0ACNR_POS)) /**< SRAM2_SYS0ACNR Mask */

 #define MXC_F_RPU_SRAM2_SYS0ACNW_POS                   7 /**< SRAM2_SYS0ACNW Position */
 #define MXC_F_RPU_SRAM2_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SYS0ACNW_POS)) /**< SRAM2_SYS0ACNW Mask */

 #define MXC_F_RPU_SRAM2_SYS1ACNR_POS                   8 /**< SRAM2_SYS1ACNR Position */
 #define MXC_F_RPU_SRAM2_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SYS1ACNR_POS)) /**< SRAM2_SYS1ACNR Mask */

 #define MXC_F_RPU_SRAM2_SYS1ACNW_POS                   9 /**< SRAM2_SYS1ACNW Position */
 #define MXC_F_RPU_SRAM2_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SYS1ACNW_POS)) /**< SRAM2_SYS1ACNW Mask */

 #define MXC_F_RPU_SRAM2_SDMADACNR_POS                  10 /**< SRAM2_SDMADACNR Position */
 #define MXC_F_RPU_SRAM2_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SDMADACNR_POS)) /**< SRAM2_SDMADACNR Mask */

 #define MXC_F_RPU_SRAM2_SDMADACNW_POS                  11 /**< SRAM2_SDMADACNW Position */
 #define MXC_F_RPU_SRAM2_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SDMADACNW_POS)) /**< SRAM2_SDMADACNW Mask */

 #define MXC_F_RPU_SRAM2_SDMAIACNR_POS                  12 /**< SRAM2_SDMAIACNR Position */
 #define MXC_F_RPU_SRAM2_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SDMAIACNR_POS)) /**< SRAM2_SDMAIACNR Mask */

 #define MXC_F_RPU_SRAM2_SDMAIACNW_POS                  13 /**< SRAM2_SDMAIACNW Position */
 #define MXC_F_RPU_SRAM2_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SDMAIACNW_POS)) /**< SRAM2_SDMAIACNW Mask */

 #define MXC_F_RPU_SRAM2_CRYPTOACNR_POS                 14 /**< SRAM2_CRYPTOACNR Position */
 #define MXC_F_RPU_SRAM2_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_CRYPTOACNR_POS)) /**< SRAM2_CRYPTOACNR Mask */

 #define MXC_F_RPU_SRAM2_CRYPTOACNW_POS                 15 /**< SRAM2_CRYPTOACNW Position */
 #define MXC_F_RPU_SRAM2_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_CRYPTOACNW_POS)) /**< SRAM2_CRYPTOACNW Mask */

 #define MXC_F_RPU_SRAM2_SDIOACNR_POS                   16 /**< SRAM2_SDIOACNR Position */
 #define MXC_F_RPU_SRAM2_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SDIOACNR_POS)) /**< SRAM2_SDIOACNR Mask */

 #define MXC_F_RPU_SRAM2_SDIOACNW_POS                   17 /**< SRAM2_SDIOACNW Position */
 #define MXC_F_RPU_SRAM2_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM2_SDIOACNW_POS)) /**< SRAM2_SDIOACNW Mask */

/**@} end of group RPU_SRAM2_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SRAM3 RPU_SRAM3
 * @brief    SRAM3 Protection Register
 * @{
 */
 #define MXC_F_RPU_SRAM3_DMA0ACNR_POS                   0 /**< SRAM3_DMA0ACNR Position */
 #define MXC_F_RPU_SRAM3_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_DMA0ACNR_POS)) /**< SRAM3_DMA0ACNR Mask */

 #define MXC_F_RPU_SRAM3_DMA0ACNW_POS                   1 /**< SRAM3_DMA0ACNW Position */
 #define MXC_F_RPU_SRAM3_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_DMA0ACNW_POS)) /**< SRAM3_DMA0ACNW Mask */

 #define MXC_F_RPU_SRAM3_DMA1ACNR_POS                   2 /**< SRAM3_DMA1ACNR Position */
 #define MXC_F_RPU_SRAM3_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_DMA1ACNR_POS)) /**< SRAM3_DMA1ACNR Mask */

 #define MXC_F_RPU_SRAM3_DMA1ACNW_POS                   3 /**< SRAM3_DMA1ACNW Position */
 #define MXC_F_RPU_SRAM3_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_DMA1ACNW_POS)) /**< SRAM3_DMA1ACNW Mask */

 #define MXC_F_RPU_SRAM3_USBACNR_POS                    4 /**< SRAM3_USBACNR Position */
 #define MXC_F_RPU_SRAM3_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_USBACNR_POS)) /**< SRAM3_USBACNR Mask */

 #define MXC_F_RPU_SRAM3_USBACNW_POS                    5 /**< SRAM3_USBACNW Position */
 #define MXC_F_RPU_SRAM3_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_USBACNW_POS)) /**< SRAM3_USBACNW Mask */

 #define MXC_F_RPU_SRAM3_SYS0ACNR_POS                   6 /**< SRAM3_SYS0ACNR Position */
 #define MXC_F_RPU_SRAM3_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SYS0ACNR_POS)) /**< SRAM3_SYS0ACNR Mask */

 #define MXC_F_RPU_SRAM3_SYS0ACNW_POS                   7 /**< SRAM3_SYS0ACNW Position */
 #define MXC_F_RPU_SRAM3_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SYS0ACNW_POS)) /**< SRAM3_SYS0ACNW Mask */

 #define MXC_F_RPU_SRAM3_SYS1ACNR_POS                   8 /**< SRAM3_SYS1ACNR Position */
 #define MXC_F_RPU_SRAM3_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SYS1ACNR_POS)) /**< SRAM3_SYS1ACNR Mask */

 #define MXC_F_RPU_SRAM3_SYS1ACNW_POS                   9 /**< SRAM3_SYS1ACNW Position */
 #define MXC_F_RPU_SRAM3_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SYS1ACNW_POS)) /**< SRAM3_SYS1ACNW Mask */

 #define MXC_F_RPU_SRAM3_SDMADACNR_POS                  10 /**< SRAM3_SDMADACNR Position */
 #define MXC_F_RPU_SRAM3_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SDMADACNR_POS)) /**< SRAM3_SDMADACNR Mask */

 #define MXC_F_RPU_SRAM3_SDMADACNW_POS                  11 /**< SRAM3_SDMADACNW Position */
 #define MXC_F_RPU_SRAM3_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SDMADACNW_POS)) /**< SRAM3_SDMADACNW Mask */

 #define MXC_F_RPU_SRAM3_SDMAIACNR_POS                  12 /**< SRAM3_SDMAIACNR Position */
 #define MXC_F_RPU_SRAM3_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SDMAIACNR_POS)) /**< SRAM3_SDMAIACNR Mask */

 #define MXC_F_RPU_SRAM3_SDMAIACNW_POS                  13 /**< SRAM3_SDMAIACNW Position */
 #define MXC_F_RPU_SRAM3_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SDMAIACNW_POS)) /**< SRAM3_SDMAIACNW Mask */

 #define MXC_F_RPU_SRAM3_CRYPTOACNR_POS                 14 /**< SRAM3_CRYPTOACNR Position */
 #define MXC_F_RPU_SRAM3_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_CRYPTOACNR_POS)) /**< SRAM3_CRYPTOACNR Mask */

 #define MXC_F_RPU_SRAM3_CRYPTOACNW_POS                 15 /**< SRAM3_CRYPTOACNW Position */
 #define MXC_F_RPU_SRAM3_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_CRYPTOACNW_POS)) /**< SRAM3_CRYPTOACNW Mask */

 #define MXC_F_RPU_SRAM3_SDIOACNR_POS                   16 /**< SRAM3_SDIOACNR Position */
 #define MXC_F_RPU_SRAM3_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SDIOACNR_POS)) /**< SRAM3_SDIOACNR Mask */

 #define MXC_F_RPU_SRAM3_SDIOACNW_POS                   17 /**< SRAM3_SDIOACNW Position */
 #define MXC_F_RPU_SRAM3_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM3_SDIOACNW_POS)) /**< SRAM3_SDIOACNW Mask */

/**@} end of group RPU_SRAM3_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SRAM4 RPU_SRAM4
 * @brief    SRAM4 Protection Register
 * @{
 */
 #define MXC_F_RPU_SRAM4_DMA0ACNR_POS                   0 /**< SRAM4_DMA0ACNR Position */
 #define MXC_F_RPU_SRAM4_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_DMA0ACNR_POS)) /**< SRAM4_DMA0ACNR Mask */

 #define MXC_F_RPU_SRAM4_DMA0ACNW_POS                   1 /**< SRAM4_DMA0ACNW Position */
 #define MXC_F_RPU_SRAM4_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_DMA0ACNW_POS)) /**< SRAM4_DMA0ACNW Mask */

 #define MXC_F_RPU_SRAM4_DMA1ACNR_POS                   2 /**< SRAM4_DMA1ACNR Position */
 #define MXC_F_RPU_SRAM4_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_DMA1ACNR_POS)) /**< SRAM4_DMA1ACNR Mask */

 #define MXC_F_RPU_SRAM4_DMA1ACNW_POS                   3 /**< SRAM4_DMA1ACNW Position */
 #define MXC_F_RPU_SRAM4_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_DMA1ACNW_POS)) /**< SRAM4_DMA1ACNW Mask */

 #define MXC_F_RPU_SRAM4_USBACNR_POS                    4 /**< SRAM4_USBACNR Position */
 #define MXC_F_RPU_SRAM4_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_USBACNR_POS)) /**< SRAM4_USBACNR Mask */

 #define MXC_F_RPU_SRAM4_USBACNW_POS                    5 /**< SRAM4_USBACNW Position */
 #define MXC_F_RPU_SRAM4_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_USBACNW_POS)) /**< SRAM4_USBACNW Mask */

 #define MXC_F_RPU_SRAM4_SYS0ACNR_POS                   6 /**< SRAM4_SYS0ACNR Position */
 #define MXC_F_RPU_SRAM4_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SYS0ACNR_POS)) /**< SRAM4_SYS0ACNR Mask */

 #define MXC_F_RPU_SRAM4_SYS0ACNW_POS                   7 /**< SRAM4_SYS0ACNW Position */
 #define MXC_F_RPU_SRAM4_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SYS0ACNW_POS)) /**< SRAM4_SYS0ACNW Mask */

 #define MXC_F_RPU_SRAM4_SYS1ACNR_POS                   8 /**< SRAM4_SYS1ACNR Position */
 #define MXC_F_RPU_SRAM4_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SYS1ACNR_POS)) /**< SRAM4_SYS1ACNR Mask */

 #define MXC_F_RPU_SRAM4_SYS1ACNW_POS                   9 /**< SRAM4_SYS1ACNW Position */
 #define MXC_F_RPU_SRAM4_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SYS1ACNW_POS)) /**< SRAM4_SYS1ACNW Mask */

 #define MXC_F_RPU_SRAM4_SDMADACNR_POS                  10 /**< SRAM4_SDMADACNR Position */
 #define MXC_F_RPU_SRAM4_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SDMADACNR_POS)) /**< SRAM4_SDMADACNR Mask */

 #define MXC_F_RPU_SRAM4_SDMADACNW_POS                  11 /**< SRAM4_SDMADACNW Position */
 #define MXC_F_RPU_SRAM4_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SDMADACNW_POS)) /**< SRAM4_SDMADACNW Mask */

 #define MXC_F_RPU_SRAM4_SDMAIACNR_POS                  12 /**< SRAM4_SDMAIACNR Position */
 #define MXC_F_RPU_SRAM4_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SDMAIACNR_POS)) /**< SRAM4_SDMAIACNR Mask */

 #define MXC_F_RPU_SRAM4_SDMAIACNW_POS                  13 /**< SRAM4_SDMAIACNW Position */
 #define MXC_F_RPU_SRAM4_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SDMAIACNW_POS)) /**< SRAM4_SDMAIACNW Mask */

 #define MXC_F_RPU_SRAM4_CRYPTOACNR_POS                 14 /**< SRAM4_CRYPTOACNR Position */
 #define MXC_F_RPU_SRAM4_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_CRYPTOACNR_POS)) /**< SRAM4_CRYPTOACNR Mask */

 #define MXC_F_RPU_SRAM4_CRYPTOACNW_POS                 15 /**< SRAM4_CRYPTOACNW Position */
 #define MXC_F_RPU_SRAM4_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_CRYPTOACNW_POS)) /**< SRAM4_CRYPTOACNW Mask */

 #define MXC_F_RPU_SRAM4_SDIOACNR_POS                   16 /**< SRAM4_SDIOACNR Position */
 #define MXC_F_RPU_SRAM4_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SDIOACNR_POS)) /**< SRAM4_SDIOACNR Mask */

 #define MXC_F_RPU_SRAM4_SDIOACNW_POS                   17 /**< SRAM4_SDIOACNW Position */
 #define MXC_F_RPU_SRAM4_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM4_SDIOACNW_POS)) /**< SRAM4_SDIOACNW Mask */

/**@} end of group RPU_SRAM4_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SRAM5 RPU_SRAM5
 * @brief    SRAM5 Protection Register
 * @{
 */
 #define MXC_F_RPU_SRAM5_DMA0ACNR_POS                   0 /**< SRAM5_DMA0ACNR Position */
 #define MXC_F_RPU_SRAM5_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_DMA0ACNR_POS)) /**< SRAM5_DMA0ACNR Mask */

 #define MXC_F_RPU_SRAM5_DMA0ACNW_POS                   1 /**< SRAM5_DMA0ACNW Position */
 #define MXC_F_RPU_SRAM5_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_DMA0ACNW_POS)) /**< SRAM5_DMA0ACNW Mask */

 #define MXC_F_RPU_SRAM5_DMA1ACNR_POS                   2 /**< SRAM5_DMA1ACNR Position */
 #define MXC_F_RPU_SRAM5_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_DMA1ACNR_POS)) /**< SRAM5_DMA1ACNR Mask */

 #define MXC_F_RPU_SRAM5_DMA1ACNW_POS                   3 /**< SRAM5_DMA1ACNW Position */
 #define MXC_F_RPU_SRAM5_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_DMA1ACNW_POS)) /**< SRAM5_DMA1ACNW Mask */

 #define MXC_F_RPU_SRAM5_USBACNR_POS                    4 /**< SRAM5_USBACNR Position */
 #define MXC_F_RPU_SRAM5_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_USBACNR_POS)) /**< SRAM5_USBACNR Mask */

 #define MXC_F_RPU_SRAM5_USBACNW_POS                    5 /**< SRAM5_USBACNW Position */
 #define MXC_F_RPU_SRAM5_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_USBACNW_POS)) /**< SRAM5_USBACNW Mask */

 #define MXC_F_RPU_SRAM5_SYS0ACNR_POS                   6 /**< SRAM5_SYS0ACNR Position */
 #define MXC_F_RPU_SRAM5_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SYS0ACNR_POS)) /**< SRAM5_SYS0ACNR Mask */

 #define MXC_F_RPU_SRAM5_SYS0ACNW_POS                   7 /**< SRAM5_SYS0ACNW Position */
 #define MXC_F_RPU_SRAM5_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SYS0ACNW_POS)) /**< SRAM5_SYS0ACNW Mask */

 #define MXC_F_RPU_SRAM5_SYS1ACNR_POS                   8 /**< SRAM5_SYS1ACNR Position */
 #define MXC_F_RPU_SRAM5_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SYS1ACNR_POS)) /**< SRAM5_SYS1ACNR Mask */

 #define MXC_F_RPU_SRAM5_SYS1ACNW_POS                   9 /**< SRAM5_SYS1ACNW Position */
 #define MXC_F_RPU_SRAM5_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SYS1ACNW_POS)) /**< SRAM5_SYS1ACNW Mask */

 #define MXC_F_RPU_SRAM5_SDMADACNR_POS                  10 /**< SRAM5_SDMADACNR Position */
 #define MXC_F_RPU_SRAM5_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SDMADACNR_POS)) /**< SRAM5_SDMADACNR Mask */

 #define MXC_F_RPU_SRAM5_SDMADACNW_POS                  11 /**< SRAM5_SDMADACNW Position */
 #define MXC_F_RPU_SRAM5_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SDMADACNW_POS)) /**< SRAM5_SDMADACNW Mask */

 #define MXC_F_RPU_SRAM5_SDMAIACNR_POS                  12 /**< SRAM5_SDMAIACNR Position */
 #define MXC_F_RPU_SRAM5_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SDMAIACNR_POS)) /**< SRAM5_SDMAIACNR Mask */

 #define MXC_F_RPU_SRAM5_SDMAIACNW_POS                  13 /**< SRAM5_SDMAIACNW Position */
 #define MXC_F_RPU_SRAM5_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SDMAIACNW_POS)) /**< SRAM5_SDMAIACNW Mask */

 #define MXC_F_RPU_SRAM5_CRYPTOACNR_POS                 14 /**< SRAM5_CRYPTOACNR Position */
 #define MXC_F_RPU_SRAM5_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_CRYPTOACNR_POS)) /**< SRAM5_CRYPTOACNR Mask */

 #define MXC_F_RPU_SRAM5_CRYPTOACNW_POS                 15 /**< SRAM5_CRYPTOACNW Position */
 #define MXC_F_RPU_SRAM5_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_CRYPTOACNW_POS)) /**< SRAM5_CRYPTOACNW Mask */

 #define MXC_F_RPU_SRAM5_SDIOACNR_POS                   16 /**< SRAM5_SDIOACNR Position */
 #define MXC_F_RPU_SRAM5_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SDIOACNR_POS)) /**< SRAM5_SDIOACNR Mask */

 #define MXC_F_RPU_SRAM5_SDIOACNW_POS                   17 /**< SRAM5_SDIOACNW Position */
 #define MXC_F_RPU_SRAM5_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM5_SDIOACNW_POS)) /**< SRAM5_SDIOACNW Mask */

/**@} end of group RPU_SRAM5_Register */

/**
 * @ingroup  rpu_registers
 * @defgroup RPU_SRAM6 RPU_SRAM6
 * @brief    SRAM6 Protection Register
 * @{
 */
 #define MXC_F_RPU_SRAM6_DMA0ACNR_POS                   0 /**< SRAM6_DMA0ACNR Position */
 #define MXC_F_RPU_SRAM6_DMA0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_DMA0ACNR_POS)) /**< SRAM6_DMA0ACNR Mask */

 #define MXC_F_RPU_SRAM6_DMA0ACNW_POS                   1 /**< SRAM6_DMA0ACNW Position */
 #define MXC_F_RPU_SRAM6_DMA0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_DMA0ACNW_POS)) /**< SRAM6_DMA0ACNW Mask */

 #define MXC_F_RPU_SRAM6_DMA1ACNR_POS                   2 /**< SRAM6_DMA1ACNR Position */
 #define MXC_F_RPU_SRAM6_DMA1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_DMA1ACNR_POS)) /**< SRAM6_DMA1ACNR Mask */

 #define MXC_F_RPU_SRAM6_DMA1ACNW_POS                   3 /**< SRAM6_DMA1ACNW Position */
 #define MXC_F_RPU_SRAM6_DMA1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_DMA1ACNW_POS)) /**< SRAM6_DMA1ACNW Mask */

 #define MXC_F_RPU_SRAM6_USBACNR_POS                    4 /**< SRAM6_USBACNR Position */
 #define MXC_F_RPU_SRAM6_USBACNR                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_USBACNR_POS)) /**< SRAM6_USBACNR Mask */

 #define MXC_F_RPU_SRAM6_USBACNW_POS                    5 /**< SRAM6_USBACNW Position */
 #define MXC_F_RPU_SRAM6_USBACNW                        ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_USBACNW_POS)) /**< SRAM6_USBACNW Mask */

 #define MXC_F_RPU_SRAM6_SYS0ACNR_POS                   6 /**< SRAM6_SYS0ACNR Position */
 #define MXC_F_RPU_SRAM6_SYS0ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SYS0ACNR_POS)) /**< SRAM6_SYS0ACNR Mask */

 #define MXC_F_RPU_SRAM6_SYS0ACNW_POS                   7 /**< SRAM6_SYS0ACNW Position */
 #define MXC_F_RPU_SRAM6_SYS0ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SYS0ACNW_POS)) /**< SRAM6_SYS0ACNW Mask */

 #define MXC_F_RPU_SRAM6_SYS1ACNR_POS                   8 /**< SRAM6_SYS1ACNR Position */
 #define MXC_F_RPU_SRAM6_SYS1ACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SYS1ACNR_POS)) /**< SRAM6_SYS1ACNR Mask */

 #define MXC_F_RPU_SRAM6_SYS1ACNW_POS                   9 /**< SRAM6_SYS1ACNW Position */
 #define MXC_F_RPU_SRAM6_SYS1ACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SYS1ACNW_POS)) /**< SRAM6_SYS1ACNW Mask */

 #define MXC_F_RPU_SRAM6_SDMADACNR_POS                  10 /**< SRAM6_SDMADACNR Position */
 #define MXC_F_RPU_SRAM6_SDMADACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SDMADACNR_POS)) /**< SRAM6_SDMADACNR Mask */

 #define MXC_F_RPU_SRAM6_SDMADACNW_POS                  11 /**< SRAM6_SDMADACNW Position */
 #define MXC_F_RPU_SRAM6_SDMADACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SDMADACNW_POS)) /**< SRAM6_SDMADACNW Mask */

 #define MXC_F_RPU_SRAM6_SDMAIACNR_POS                  12 /**< SRAM6_SDMAIACNR Position */
 #define MXC_F_RPU_SRAM6_SDMAIACNR                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SDMAIACNR_POS)) /**< SRAM6_SDMAIACNR Mask */

 #define MXC_F_RPU_SRAM6_SDMAIACNW_POS                  13 /**< SRAM6_SDMAIACNW Position */
 #define MXC_F_RPU_SRAM6_SDMAIACNW                      ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SDMAIACNW_POS)) /**< SRAM6_SDMAIACNW Mask */

 #define MXC_F_RPU_SRAM6_CRYPTOACNR_POS                 14 /**< SRAM6_CRYPTOACNR Position */
 #define MXC_F_RPU_SRAM6_CRYPTOACNR                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_CRYPTOACNR_POS)) /**< SRAM6_CRYPTOACNR Mask */

 #define MXC_F_RPU_SRAM6_CRYPTOACNW_POS                 15 /**< SRAM6_CRYPTOACNW Position */
 #define MXC_F_RPU_SRAM6_CRYPTOACNW                     ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_CRYPTOACNW_POS)) /**< SRAM6_CRYPTOACNW Mask */

 #define MXC_F_RPU_SRAM6_SDIOACNR_POS                   16 /**< SRAM6_SDIOACNR Position */
 #define MXC_F_RPU_SRAM6_SDIOACNR                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SDIOACNR_POS)) /**< SRAM6_SDIOACNR Mask */

 #define MXC_F_RPU_SRAM6_SDIOACNW_POS                   17 /**< SRAM6_SDIOACNW Position */
 #define MXC_F_RPU_SRAM6_SDIOACNW                       ((uint32_t)(0x1UL << MXC_F_RPU_SRAM6_SDIOACNW_POS)) /**< SRAM6_SDIOACNW Mask */

/**@} end of group RPU_SRAM6_Register */

#ifdef __cplusplus
}
#endif

#endif /* _RPU_REGS_H_ */
