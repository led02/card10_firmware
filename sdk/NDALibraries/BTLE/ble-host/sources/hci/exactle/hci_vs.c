/*************************************************************************************************/
/*!
 *  \file   hci_vs.c
 *
 *  \brief  HCI vendor specific functions for single-chip.
 *
 *          $Date: 2019-02-25 17:25:06 +0000 (Mon, 25 Feb 2019) $
 *          $Revision: 41190 $
 *
 *  Copyright (c) 2011 Wicentric, Inc., all rights reserved.
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

#include "wsf_types.h"
#include "wsf_msg.h"
#include "hci_api.h"
#include "ll_api.h"

/*************************************************************************************************/
/*!
 *  \fn     hciCoreResetStart
 *
 *  \brief  Start the HCI reset sequence.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreResetStart(void)
{
  /* send an HCI Reset command to start the sequence */
  HciResetCmd();
}

/*************************************************************************************************/
/*!
 *  \fn     HciVsInit
 *
 *  \brief  Vendor-specific controller initialization function.
 *
 *  \param  param    Vendor-specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVsInit(uint8_t param)
{
  /* unused */
}

/*************************************************************************************************/
/*!
 *  \fn     HciVsSetFeatures
 *        
 *  \brief  Set the features set for the controller.
 *
 *  \param  features     Pointer to new feature set buffer. Should be 8 bytes in length.
 *  \param  featuresMask Pointer to new feature mask set buffer. Should be 8 bytes in length.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVsSetFeatures(const uint8_t *features, const uint8_t *featuresMask)
{
  LlSetFeatures(features, featuresMask);
}

/*************************************************************************************************/
/*!
 *  \fn     HciVsSetPower
 *        
 *  \brief  Vendor-specific set power level.
 *
 *  \param  param    power level in dBm. 
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVsSetTxPower(int8_t power)
{
  LlSetTxPower(power);
}

