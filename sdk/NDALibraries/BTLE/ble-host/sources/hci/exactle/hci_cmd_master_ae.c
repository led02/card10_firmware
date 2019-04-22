/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI Advertising Extensions (AE) command module for master.
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

#include <string.h>
#include <limits.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_queue.h"
#include "wsf_timer.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include "hci_cmd.h"
#include "hci_tr.h"
#include "hci_api.h"
#include "hci_main.h"

#include "ll_api.h"

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended scanning parameters command.
 *
 *  \param      ownAddrType     Address type used by this device.
 *  \param      scanFiltPolicy  Scan filter policy.
 *  \param      scanPhys        Scanning PHYs.
 *  \param      pScanParam      Scanning parameter array.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtScanParamCmd(
    uint8_t ownAddrType,
    uint8_t scanFiltPolicy,
    uint8_t scanPhys,
    hciExtScanParam_t *pScanParam)
{
  LlExtScanParam_t param;
  uint8_t status;

  uint8_t scanPhysIndex;
  uint8_t scanPhysBit;

  hciExtScanParam_t * pScanParamIter;

  WSF_ASSERT((sizeof(scanPhys) * CHAR_BIT) >= LL_MAX_NUM_PHYS);

  param.ownAddrType = ownAddrType;
  param.scanFiltPolicy = scanFiltPolicy;

  pScanParamIter = pScanParam;
  for (scanPhysIndex = 0, scanPhysBit = 1;
      scanPhysIndex < LL_MAX_NUM_PHYS;
      scanPhysIndex ++, scanPhysBit <<= 1)
  {
    if ((scanPhys & scanPhysBit) == 0)
    {
      param.phys[scanPhysIndex] = FALSE;
      memset(&param.phyParam[scanPhysIndex],
          '\0',
          sizeof(param.phyParam[scanPhysIndex]));
    }
    else
    {
      param.phys[scanPhysIndex] = TRUE;
      param.phyParam[scanPhysIndex].scanType = pScanParamIter->scanType;
      param.phyParam[scanPhysIndex].scanInterval = pScanParamIter->scanInterval;
      param.phyParam[scanPhysIndex].scanWindow = pScanParamIter->scanWindow;
      pScanParamIter++;
    }
  }

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlSetExtScanParam((LlExtScanParam_t *)&param);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE extended scan enable command.
 *
 *  \param      enable          Set to TRUE to enable scanning, FALSE to disable scanning.
 *  \param      filterDup       Set to TRUE to filter duplicates.
 *  \param      duration        Duration.
 *  \param      period          Period.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeExtScanEnableCmd(uint8_t enable, uint8_t filterDup, uint16_t duration, uint16_t period)
{
  LlExtScanEnable(enable, filterDup, duration, period);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE extended create connection command.
 *
 *  \param      pInitParam      Initiating parameters.
 *  \param      pScanParam      Initiating scan parameters.
 *  \param      pConnSpec       Connection specification.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeExtCreateConnCmd(hciExtInitParam_t *pInitParam, hciExtInitScanParam_t *pScanParam,
                           hciConnSpec_t *pConnSpec)
{
  uint8_t status;
  (void)status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlExtCreateConn(
      (LlExtInitParam_t *)pInitParam,
      (LlExtInitScanParam_t *)pScanParam,
      (LlConnSpec_t *)pConnSpec);

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE periodic advertising create sync command.
 *
 *  \param      filterPolicy    Filter policy.
 *  \param      advSid          Advertising SID.
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      skip            Number of periodic advertising packets that can be skipped after
 *                              successful receive.
 *  \param      syncTimeout     Synchronization timeout.
 *  \param      unused          Reserved for future use (must be zero).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvCreateSyncCmd(uint8_t filterPolicy, uint8_t advSid, uint8_t advAddrType,
                              uint8_t *pAdvAddr, uint16_t skip, uint16_t syncTimeout, uint8_t unused)
{
  LlPerAdvCreateSyncCmd_t param;
  uint8_t status;
  (void)status;

  param.filterPolicy = filterPolicy;
  param.advSID = advSid;
  param.advAddrType = advAddrType;
  param.pAdvAddr = pAdvAddr;
  param.skip = skip;
  param.syncTimeOut = syncTimeout;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlPeriodicAdvCreateSync(&param);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE periodic advertising create sync cancel command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvCreateSyncCancelCmd(void)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlPeriodicAdvCreateSyncCancel();
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE periodic advertising terminate sync command.
 *
 *  \param      syncHandle      Sync handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLePerAdvTerminateSyncCmd(uint16_t syncHandle)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlPeriodicAdvTerminateSync(syncHandle);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE add device to periodic advertiser list command.
 *
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      advSid          Advertising SID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeAddDeviceToPerAdvListCmd(uint8_t advAddrType, uint8_t *pAdvAddr, uint8_t advSid)
{
  LlDevicePerAdvList_t param;
  uint8_t status;

  param.advAddrType = advAddrType;
  param.pAdvAddr = pAdvAddr;
  param.advSID = advSid;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlAddDeviceToPeriodicAdvList(&param);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE remove device from periodic advertiser list command.
 *
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      advSid          Advertising SID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeRemoveDeviceFromPerAdvListCmd(uint8_t advAddrType, uint8_t *pAdvAddr, uint8_t advSid)
{
  LlDevicePerAdvList_t param;
  uint8_t status;

  param.advAddrType = advAddrType;
  param.pAdvAddr = pAdvAddr;
  param.advSID = advSid;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlRemoveDeviceFromPeriodicAdvList(&param);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE clear periodic advertiser list command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeClearPerAdvListCmd(void)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlClearPeriodicAdvList();
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}
