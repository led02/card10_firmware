/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Baseband driver interface file.
 *
 *  $Date: 2019-02-25 17:25:06 +0000 (Mon, 25 Feb 2019) $
 *  $Revision: 41190 $
 *
 *  Copyright (c) 2013 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef BB_DRV_H
#define BB_DRV_H

#include "wsf_types.h"
#include "cfg_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup BB_DRV_INIT
 *  \{
 *  This section contains driver routines which initializes as well as enables the sleep mode
 *  of the BB hardware.
 */

void BbDrvInit(void);
void BbDrvEnable(uint8_t protId);
void BbDrvDisable(uint8_t protId);

/*! \} */    /* BB_DRV_INIT */

/*! \addtogroup BB_DRV_CLOCK
 *  \{
 *  This section contains driver routines related to the BB clock, packet time stamping and
 *  time to BB clock tick utilities.
 */

uint32_t BbDrvGetCurrentTime(void);
uint16_t BbDrvGetSleepClockAccuracy(void);
uint32_t BbDrvGetStartTimestamp(void);
uint32_t BbDrvGetEndTimestamp(void);
uint32_t BbDrvConvertUsToTicks(uint32_t us);
uint32_t BbDrvConvertTicksToUs(uint32_t ticks);

/*! \} */    /* BB_DRV_CLOCK */

#ifdef __cplusplus
};
#endif

#endif /* BB_DRV_H */
