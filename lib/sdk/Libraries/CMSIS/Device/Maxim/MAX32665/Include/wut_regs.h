/**
 * @file    wut_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the WUT Peripheral Module.
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

#ifndef _WUT_REGS_H_
#define _WUT_REGS_H_

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
 * @ingroup     wut
 * @defgroup    wut_registers WUT_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the WUT Peripheral Module.
 * @details Wakeup Timer.
 */

/**
 * @ingroup wut_registers
 * Structure type to access the WUT Registers.
 */
typedef struct {
    __IO uint32_t cnt;                  /**< <tt>\b 0x00:</tt> WUT CNT Register */
    __IO uint32_t cmp;                  /**< <tt>\b 0x04:</tt> WUT CMP Register */
    __IO uint32_t pwm;                  /**< <tt>\b 0x08:</tt> WUT PWM Register */
    __IO uint32_t intr;                 /**< <tt>\b 0x0C:</tt> WUT INTR Register */
    __IO uint32_t ctrl;                 /**< <tt>\b 0x10:</tt> WUT CTRL Register */
    __IO uint32_t nolap;                /**< <tt>\b 0x14:</tt> WUT NOLAP Register */
    __IO uint32_t preset;               /**< <tt>\b 0x18:</tt> WUT PRESET Register */
    __IO uint32_t reload;               /**< <tt>\b 0x1C:</tt> WUT RELOAD Register */
    __IO uint32_t snapshot;             /**< <tt>\b 0x20:</tt> WUT SNAPSHOT Register */
} mxc_wut_regs_t;

/* Register offsets for module WUT */
/**
 * @ingroup    wut_registers
 * @defgroup   WUT_Register_Offsets Register Offsets
 * @brief      WUT Peripheral Register Offsets from the WUT Base Peripheral Address. 
 * @{
 */
 #define MXC_R_WUT_CNT                      ((uint32_t)0x00000000UL) /**< Offset from WUT Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_WUT_CMP                      ((uint32_t)0x00000004UL) /**< Offset from WUT Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_WUT_PWM                      ((uint32_t)0x00000008UL) /**< Offset from WUT Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_WUT_INTR                     ((uint32_t)0x0000000CUL) /**< Offset from WUT Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_WUT_CTRL                     ((uint32_t)0x00000010UL) /**< Offset from WUT Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_WUT_NOLAP                    ((uint32_t)0x00000014UL) /**< Offset from WUT Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_WUT_PRESET                   ((uint32_t)0x00000018UL) /**< Offset from WUT Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_WUT_RELOAD                   ((uint32_t)0x0000001CUL) /**< Offset from WUT Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_WUT_SNAPSHOT                 ((uint32_t)0x00000020UL) /**< Offset from WUT Base Address: <tt> 0x0020</tt> */ 
/**@} end of group wut_registers */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_CNT WUT_CNT
 * @brief    Wakeup Timer Count Register.
 * @{
 */
/**@} end of group WUT_CNT_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_CMP WUT_CMP
 * @brief    Wakeup Timer Compare Register.
 * @{
 */
/**@} end of group WUT_CMP_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_PWM WUT_PWM
 * @brief    Wakeup Timer PWM Register.
 * @{
 */
/**@} end of group WUT_PWM_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_INTR WUT_INTR
 * @brief    Wakeup Timer Interrupt Register.
 * @{
 */
/**@} end of group WUT_INTR_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_CTRL WUT_CTRL
 * @brief    Wakeup Timer Control Register.
 * @{
 */
/**@} end of group WUT_CTRL_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_NOLAP WUT_NOLAP
 * @brief    Wakeup Timer Non-overlap Register.
 * @{
 */
/**@} end of group WUT_NOLAP_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_PRESET WUT_PRESET
 * @brief    Wakeup Timer Base Band Event Timer Preset Register.
 * @{
 */
/**@} end of group WUT_PRESET_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_RELOAD WUT_RELOAD
 * @brief    Wakeup Timer Base Band Event Timer Preset Load Register.
 * @{
 */
/**@} end of group WUT_RELOAD_Register */

/**
 * @ingroup  wut_registers
 * @defgroup WUT_SNAPSHOT WUT_SNAPSHOT
 * @brief    Wakeup Timer Base Band Event Timer Snapshot Register.
 * @{
 */
/**@} end of group WUT_SNAPSHOT_Register */

#ifdef __cplusplus
}
#endif

#endif /* _WUT_REGS_H_ */
