/**
 * @file    pwrseq_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the PWRSEQ Peripheral Module.
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

#ifndef _PWRSEQ_REGS_H_
#define _PWRSEQ_REGS_H_

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
 * @ingroup     pwrseq
 * @defgroup    pwrseq_registers PWRSEQ_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the PWRSEQ Peripheral Module.
 * @details Power Sequencer / Low Power Control Register.
 */

/**
 * @ingroup pwrseq_registers
 * Structure type to access the PWRSEQ Registers.
 */
typedef struct {
    __IO uint32_t lpcn;                 /**< <tt>\b 0x00:</tt> PWRSEQ LPCN Register */
    __IO uint32_t lpwkst0;              /**< <tt>\b 0x04:</tt> PWRSEQ LPWKST0 Register */
    __IO uint32_t lpwken0;              /**< <tt>\b 0x08:</tt> PWRSEQ LPWKEN0 Register */
    __IO uint32_t lpwkst1;              /**< <tt>\b 0x0C:</tt> PWRSEQ LPWKST1 Register */
    __IO uint32_t lpwken1;              /**< <tt>\b 0x10:</tt> PWRSEQ LPWKEN1 Register */
    __IO uint32_t lpwkst2;              /**< <tt>\b 0x14:</tt> PWRSEQ LPWKST2 Register */
    __IO uint32_t lpwken2;              /**< <tt>\b 0x18:</tt> PWRSEQ LPWKEN2 Register */
    __IO uint32_t lpwkst3;              /**< <tt>\b 0x1C:</tt> PWRSEQ LPWKST3 Register */
    __IO uint32_t lpwken3;              /**< <tt>\b 0x20:</tt> PWRSEQ LPWKEN3 Register */
    __R  uint32_t rsv_0x24_0x2f[3];
    __IO uint32_t lppwst;               /**< <tt>\b 0x30:</tt> PWRSEQ LPPWST Register */
    __IO uint32_t lppwen;               /**< <tt>\b 0x34:</tt> PWRSEQ LPPWEN Register */
    __R  uint32_t rsv_0x38_0x3f[2];
    __IO uint32_t lpmemsd;              /**< <tt>\b 0x40:</tt> PWRSEQ LPMEMSD Register */
    __IO uint32_t lpvddpd;              /**< <tt>\b 0x44:</tt> PWRSEQ LPVDDPD Register */
    __IO uint32_t gp0;                  /**< <tt>\b 0x48:</tt> PWRSEQ GP0 Register */
    __IO uint32_t gp1;                  /**< <tt>\b 0x4C:</tt> PWRSEQ GP1 Register */
    __IO uint32_t lpmcstat;             /**< <tt>\b 0x50:</tt> PWRSEQ LPMCSTAT Register */
    __IO uint32_t lpmcreq;              /**< <tt>\b 0x54:</tt> PWRSEQ LPMCREQ Register */
} mxc_pwrseq_regs_t;

/* Register offsets for module PWRSEQ */
/**
 * @ingroup    pwrseq_registers
 * @defgroup   PWRSEQ_Register_Offsets Register Offsets
 * @brief      PWRSEQ Peripheral Register Offsets from the PWRSEQ Base Peripheral Address. 
 * @{
 */
 #define MXC_R_PWRSEQ_LPCN                  ((uint32_t)0x00000000UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST0               ((uint32_t)0x00000004UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN0               ((uint32_t)0x00000008UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST1               ((uint32_t)0x0000000CUL) /**< Offset from PWRSEQ Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN1               ((uint32_t)0x00000010UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0010</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST2               ((uint32_t)0x00000014UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0014</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN2               ((uint32_t)0x00000018UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_PWRSEQ_LPWKST3               ((uint32_t)0x0000001CUL) /**< Offset from PWRSEQ Base Address: <tt> 0x001C</tt> */ 
 #define MXC_R_PWRSEQ_LPWKEN3               ((uint32_t)0x00000020UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0020</tt> */ 
 #define MXC_R_PWRSEQ_LPPWST                ((uint32_t)0x00000030UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_PWRSEQ_LPPWEN                ((uint32_t)0x00000034UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0034</tt> */ 
 #define MXC_R_PWRSEQ_LPMEMSD               ((uint32_t)0x00000040UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_PWRSEQ_LPVDDPD               ((uint32_t)0x00000044UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0044</tt> */ 
 #define MXC_R_PWRSEQ_GP0                   ((uint32_t)0x00000048UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_PWRSEQ_GP1                   ((uint32_t)0x0000004CUL) /**< Offset from PWRSEQ Base Address: <tt> 0x004C</tt> */ 
 #define MXC_R_PWRSEQ_LPMCSTAT              ((uint32_t)0x00000050UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0050</tt> */ 
 #define MXC_R_PWRSEQ_LPMCREQ               ((uint32_t)0x00000054UL) /**< Offset from PWRSEQ Base Address: <tt> 0x0054</tt> */ 
/**@} end of group pwrseq_registers */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPCN PWRSEQ_LPCN
 * @brief    Low Power Control Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPCN_RAMRET_POS                   0 /**< LPCN_RAMRET Position */
 #define MXC_F_PWRSEQ_LPCN_RAMRET                       ((uint32_t)(0x3UL << MXC_F_PWRSEQ_LPCN_RAMRET_POS)) /**< LPCN_RAMRET Mask */
 #define MXC_V_PWRSEQ_LPCN_RAMRET_DIS                   ((uint32_t)0x0UL) /**< LPCN_RAMRET_DIS Value */
 #define MXC_S_PWRSEQ_LPCN_RAMRET_DIS                   (MXC_V_PWRSEQ_LPCN_RAMRET_DIS << MXC_F_PWRSEQ_LPCN_RAMRET_POS) /**< LPCN_RAMRET_DIS Setting */
 #define MXC_V_PWRSEQ_LPCN_RAMRET_EN1                   ((uint32_t)0x1UL) /**< LPCN_RAMRET_EN1 Value */
 #define MXC_S_PWRSEQ_LPCN_RAMRET_EN1                   (MXC_V_PWRSEQ_LPCN_RAMRET_EN1 << MXC_F_PWRSEQ_LPCN_RAMRET_POS) /**< LPCN_RAMRET_EN1 Setting */
 #define MXC_V_PWRSEQ_LPCN_RAMRET_EN2                   ((uint32_t)0x2UL) /**< LPCN_RAMRET_EN2 Value */
 #define MXC_S_PWRSEQ_LPCN_RAMRET_EN2                   (MXC_V_PWRSEQ_LPCN_RAMRET_EN2 << MXC_F_PWRSEQ_LPCN_RAMRET_POS) /**< LPCN_RAMRET_EN2 Setting */
 #define MXC_V_PWRSEQ_LPCN_RAMRET_EN3                   ((uint32_t)0x3UL) /**< LPCN_RAMRET_EN3 Value */
 #define MXC_S_PWRSEQ_LPCN_RAMRET_EN3                   (MXC_V_PWRSEQ_LPCN_RAMRET_EN3 << MXC_F_PWRSEQ_LPCN_RAMRET_POS) /**< LPCN_RAMRET_EN3 Setting */

 #define MXC_F_PWRSEQ_LPCN_OVR_POS                      4 /**< LPCN_OVR Position */
 #define MXC_F_PWRSEQ_LPCN_OVR                          ((uint32_t)(0x3UL << MXC_F_PWRSEQ_LPCN_OVR_POS)) /**< LPCN_OVR Mask */
 #define MXC_V_PWRSEQ_LPCN_OVR_0_9V                     ((uint32_t)0x0UL) /**< LPCN_OVR_0_9V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_0_9V                     (MXC_V_PWRSEQ_LPCN_OVR_0_9V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_0_9V Setting */
 #define MXC_V_PWRSEQ_LPCN_OVR_1_0V                     ((uint32_t)0x1UL) /**< LPCN_OVR_1_0V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_1_0V                     (MXC_V_PWRSEQ_LPCN_OVR_1_0V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_1_0V Setting */
 #define MXC_V_PWRSEQ_LPCN_OVR_1_1V                     ((uint32_t)0x2UL) /**< LPCN_OVR_1_1V Value */
 #define MXC_S_PWRSEQ_LPCN_OVR_1_1V                     (MXC_V_PWRSEQ_LPCN_OVR_1_1V << MXC_F_PWRSEQ_LPCN_OVR_POS) /**< LPCN_OVR_1_1V Setting */

 #define MXC_F_PWRSEQ_LPCN_BLKDET_POS                   6 /**< LPCN_BLKDET Position */
 #define MXC_F_PWRSEQ_LPCN_BLKDET                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_BLKDET_POS)) /**< LPCN_BLKDET Mask */

 #define MXC_F_PWRSEQ_LPCN_FVDDEN_POS                   7 /**< LPCN_FVDDEN Position */
 #define MXC_F_PWRSEQ_LPCN_FVDDEN                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_FVDDEN_POS)) /**< LPCN_FVDDEN Mask */

 #define MXC_F_PWRSEQ_LPCN_RREGEN_POS                   8 /**< LPCN_RREGEN Position */
 #define MXC_F_PWRSEQ_LPCN_RREGEN                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_RREGEN_POS)) /**< LPCN_RREGEN Mask */

 #define MXC_F_PWRSEQ_LPCN_BCKGRND_POS                  9 /**< LPCN_BCKGRND Position */
 #define MXC_F_PWRSEQ_LPCN_BCKGRND                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_BCKGRND_POS)) /**< LPCN_BCKGRND Mask */

 #define MXC_F_PWRSEQ_LPCN_FWKM_POS                     10 /**< LPCN_FWKM Position */
 #define MXC_F_PWRSEQ_LPCN_FWKM                         ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_FWKM_POS)) /**< LPCN_FWKM Mask */

 #define MXC_F_PWRSEQ_LPCN_BGOFF_POS                    11 /**< LPCN_BGOFF Position */
 #define MXC_F_PWRSEQ_LPCN_BGOFF                        ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_BGOFF_POS)) /**< LPCN_BGOFF Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDCMD_POS                   20 /**< LPCN_VDDCMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDCMD                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDCMD_POS)) /**< LPCN_VDDCMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VRTCMD_POS                   21 /**< LPCN_VRTCMD Position */
 #define MXC_F_PWRSEQ_LPCN_VRTCMD                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VRTCMD_POS)) /**< LPCN_VRTCMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDAMD_POS                   22 /**< LPCN_VDDAMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDAMD                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDAMD_POS)) /**< LPCN_VDDAMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDIOMD_POS                  23 /**< LPCN_VDDIOMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDIOMD                      ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDIOMD_POS)) /**< LPCN_VDDIOMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDIOHMD_POS                 24 /**< LPCN_VDDIOHMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDIOHMD                     ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDIOHMD_POS)) /**< LPCN_VDDIOHMD Mask */

 #define MXC_F_PWRSEQ_LPCN_PORVDDIOMD_POS               25 /**< LPCN_PORVDDIOMD Position */
 #define MXC_F_PWRSEQ_LPCN_PORVDDIOMD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_PORVDDIOMD_POS)) /**< LPCN_PORVDDIOMD Mask */

 #define MXC_F_PWRSEQ_LPCN_PORVDDIOHMD_POS              26 /**< LPCN_PORVDDIOHMD Position */
 #define MXC_F_PWRSEQ_LPCN_PORVDDIOHMD                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_PORVDDIOHMD_POS)) /**< LPCN_PORVDDIOHMD Mask */

 #define MXC_F_PWRSEQ_LPCN_VDDBMD_POS                   27 /**< LPCN_VDDBMD Position */
 #define MXC_F_PWRSEQ_LPCN_VDDBMD                       ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPCN_VDDBMD_POS)) /**< LPCN_VDDBMD Mask */

/**@} end of group PWRSEQ_LPCN_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPWKST0 PWRSEQ_LPWKST0
 * @brief    Low Power I/O Wakeup Status Register 0. This register indicates the low power
 *           wakeup status for GPIO0.
 * @{
 */
 #define MXC_F_PWRSEQ_LPWKST0_WAKEST_POS                0 /**< LPWKST0_WAKEST Position */
 #define MXC_F_PWRSEQ_LPWKST0_WAKEST                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPWKST0_WAKEST_POS)) /**< LPWKST0_WAKEST Mask */

/**@} end of group PWRSEQ_LPWKST0_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPWKEN0 PWRSEQ_LPWKEN0
 * @brief    Low Power I/O Wakeup Enable Register 0. This register enables low power wakeup
 *           functionality for GPIO0.
 * @{
 */
 #define MXC_F_PWRSEQ_LPWKEN0_WAKEEN_POS                0 /**< LPWKEN0_WAKEEN Position */
 #define MXC_F_PWRSEQ_LPWKEN0_WAKEEN                    ((uint32_t)(0x7FFFFFFFUL << MXC_F_PWRSEQ_LPWKEN0_WAKEEN_POS)) /**< LPWKEN0_WAKEEN Mask */

/**@} end of group PWRSEQ_LPWKEN0_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPPWST PWRSEQ_LPPWST
 * @brief    Low Power Peripheral Wakeup Status Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPPWST_USBLSWKST_POS              0 /**< LPPWST_USBLSWKST Position */
 #define MXC_F_PWRSEQ_LPPWST_USBLSWKST                  ((uint32_t)(0x3UL << MXC_F_PWRSEQ_LPPWST_USBLSWKST_POS)) /**< LPPWST_USBLSWKST Mask */

 #define MXC_F_PWRSEQ_LPPWST_USBVBUSWKST_POS            2 /**< LPPWST_USBVBUSWKST Position */
 #define MXC_F_PWRSEQ_LPPWST_USBVBUSWKST                ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWST_USBVBUSWKST_POS)) /**< LPPWST_USBVBUSWKST Mask */

 #define MXC_F_PWRSEQ_LPPWST_SDMAWKST_POS               3 /**< LPPWST_SDMAWKST Position */
 #define MXC_F_PWRSEQ_LPPWST_SDMAWKST                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWST_SDMAWKST_POS)) /**< LPPWST_SDMAWKST Mask */

 #define MXC_F_PWRSEQ_LPPWST_BBMODEST_POS               16 /**< LPPWST_BBMODEST Position */
 #define MXC_F_PWRSEQ_LPPWST_BBMODEST                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWST_BBMODEST_POS)) /**< LPPWST_BBMODEST Mask */

/**@} end of group PWRSEQ_LPPWST_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPPWEN PWRSEQ_LPPWEN
 * @brief    Low Power Peripheral Wakeup Enable Register.
 * @{
 */
 #define MXC_F_PWRSEQ_LPPWEN_USBLSWKEN_POS              0 /**< LPPWEN_USBLSWKEN Position */
 #define MXC_F_PWRSEQ_LPPWEN_USBLSWKEN                  ((uint32_t)(0x3UL << MXC_F_PWRSEQ_LPPWEN_USBLSWKEN_POS)) /**< LPPWEN_USBLSWKEN Mask */

 #define MXC_F_PWRSEQ_LPPWEN_USBVBUSWKEN_POS            2 /**< LPPWEN_USBVBUSWKEN Position */
 #define MXC_F_PWRSEQ_LPPWEN_USBVBUSWKEN                ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_USBVBUSWKEN_POS)) /**< LPPWEN_USBVBUSWKEN Mask */

 #define MXC_F_PWRSEQ_LPPWEN_SDMAWKEN_POS               3 /**< LPPWEN_SDMAWKEN Position */
 #define MXC_F_PWRSEQ_LPPWEN_SDMAWKEN                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPPWEN_SDMAWKEN_POS)) /**< LPPWEN_SDMAWKEN Mask */

/**@} end of group PWRSEQ_LPPWEN_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPMEMSD PWRSEQ_LPMEMSD
 * @brief    Low Power Memory Shutdown Control.
 * @{
 */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM0SD_POS               0 /**< LPMEMSD_SRAM0SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM0SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM0SD_POS)) /**< LPMEMSD_SRAM0SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM1SD_POS               1 /**< LPMEMSD_SRAM1SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM1SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM1SD_POS)) /**< LPMEMSD_SRAM1SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM2SD_POS               2 /**< LPMEMSD_SRAM2SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM2SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM2SD_POS)) /**< LPMEMSD_SRAM2SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM3SD_POS               3 /**< LPMEMSD_SRAM3SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM3SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM3SD_POS)) /**< LPMEMSD_SRAM3SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM4SD_POS               4 /**< LPMEMSD_SRAM4SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM4SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM4SD_POS)) /**< LPMEMSD_SRAM4SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM5SD_POS               5 /**< LPMEMSD_SRAM5SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM5SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM5SD_POS)) /**< LPMEMSD_SRAM5SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SRAM6SD_POS               6 /**< LPMEMSD_SRAM6SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SRAM6SD                   ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SRAM6SD_POS)) /**< LPMEMSD_SRAM6SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_ICACHESD_POS              7 /**< LPMEMSD_ICACHESD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_ICACHESD                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_ICACHESD_POS)) /**< LPMEMSD_ICACHESD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_ICACHEXIPSD_POS           8 /**< LPMEMSD_ICACHEXIPSD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_ICACHEXIPSD               ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_ICACHEXIPSD_POS)) /**< LPMEMSD_ICACHEXIPSD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_SCACHESD_POS              9 /**< LPMEMSD_SCACHESD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_SCACHESD                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_SCACHESD_POS)) /**< LPMEMSD_SCACHESD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_CRYPTOSD_POS              10 /**< LPMEMSD_CRYPTOSD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_CRYPTOSD                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_CRYPTOSD_POS)) /**< LPMEMSD_CRYPTOSD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_USBFIFOSD_POS             11 /**< LPMEMSD_USBFIFOSD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_USBFIFOSD                 ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_USBFIFOSD_POS)) /**< LPMEMSD_USBFIFOSD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_ROMSD_POS                 12 /**< LPMEMSD_ROMSD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_ROMSD                     ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_ROMSD_POS)) /**< LPMEMSD_ROMSD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_ROM1SD_POS                13 /**< LPMEMSD_ROM1SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_ROM1SD                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_ROM1SD_POS)) /**< LPMEMSD_ROM1SD Mask */

 #define MXC_F_PWRSEQ_LPMEMSD_IC1SD_POS                 14 /**< LPMEMSD_IC1SD Position */
 #define MXC_F_PWRSEQ_LPMEMSD_IC1SD                     ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPMEMSD_IC1SD_POS)) /**< LPMEMSD_IC1SD Mask */

/**@} end of group PWRSEQ_LPMEMSD_Register */

/**
 * @ingroup  pwrseq_registers
 * @defgroup PWRSEQ_LPVDDPD PWRSEQ_LPVDDPD
 * @brief    Low Power VDD Domain Power Down Control.
 * @{
 */
 #define MXC_F_PWRSEQ_LPVDDPD_VREGOBPD_POS              0 /**< LPVDDPD_VREGOBPD Position */
 #define MXC_F_PWRSEQ_LPVDDPD_VREGOBPD                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPVDDPD_VREGOBPD_POS)) /**< LPVDDPD_VREGOBPD Mask */

 #define MXC_F_PWRSEQ_LPVDDPD_VREGODPD_POS              1 /**< LPVDDPD_VREGODPD Position */
 #define MXC_F_PWRSEQ_LPVDDPD_VREGODPD                  ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPVDDPD_VREGODPD_POS)) /**< LPVDDPD_VREGODPD Mask */

 #define MXC_F_PWRSEQ_LPVDDPD_VDD2PD_POS                8 /**< LPVDDPD_VDD2PD Position */
 #define MXC_F_PWRSEQ_LPVDDPD_VDD2PD                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPVDDPD_VDD2PD_POS)) /**< LPVDDPD_VDD2PD Mask */

 #define MXC_F_PWRSEQ_LPVDDPD_VDD3PD_POS                9 /**< LPVDDPD_VDD3PD Position */
 #define MXC_F_PWRSEQ_LPVDDPD_VDD3PD                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPVDDPD_VDD3PD_POS)) /**< LPVDDPD_VDD3PD Mask */

 #define MXC_F_PWRSEQ_LPVDDPD_VDD4PD_POS                10 /**< LPVDDPD_VDD4PD Position */
 #define MXC_F_PWRSEQ_LPVDDPD_VDD4PD                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPVDDPD_VDD4PD_POS)) /**< LPVDDPD_VDD4PD Mask */

 #define MXC_F_PWRSEQ_LPVDDPD_VDD5PD_POS                11 /**< LPVDDPD_VDD5PD Position */
 #define MXC_F_PWRSEQ_LPVDDPD_VDD5PD                    ((uint32_t)(0x1UL << MXC_F_PWRSEQ_LPVDDPD_VDD5PD_POS)) /**< LPVDDPD_VDD5PD Mask */

/**@} end of group PWRSEQ_LPVDDPD_Register */

#ifdef __cplusplus
}
#endif

#endif /* _PWRSEQ_REGS_H_ */
