/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI PHY command module.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "hci_cmd.h"
#include "hci_api.h"
#include "hci_main.h"

#include "ll_api.h"

/*************************************************************************************************/
/*!
*  \brief  HCI read PHY command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeReadPhyCmd(uint16_t handle)
{
  hciLeReadPhyCmdCmplEvt_t evt;

  evt.hdr.param = handle;
  evt.hdr.event = HCI_LE_READ_PHY_CMD_CMPL_CBACK_EVT;
  evt.hdr.status = LlReadPhy(handle, &evt.txPhy, &evt.rxPhy);

  evt.handle = handle;
  evt.status = evt.hdr.status;

  hciCb.evtCback((hciEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
*  \brief  HCI set default PHY command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeSetDefaultPhyCmd(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys)
{
  hciLeSetDefPhyCmdCmplEvt_t evt;

  evt.hdr.param = 0;
  evt.hdr.event = HCI_LE_SET_DEF_PHY_CMD_CMPL_CBACK_EVT;
  evt.hdr.status = LlSetDefaultPhy(allPhys, txPhys, rxPhys);

  evt.status = evt.hdr.status;

  hciCb.evtCback((hciEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
*  \brief  HCI set PHY command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeSetPhyCmd(uint16_t handle, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
  LlSetPhy(handle, allPhys, txPhys, rxPhys, phyOptions);
}
