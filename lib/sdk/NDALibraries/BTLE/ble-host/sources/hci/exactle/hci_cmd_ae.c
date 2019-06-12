/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI Advertising Extensions (AE) command module.
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
#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "hci_cmd.h"
#include "hci_api.h"
#include "hci_main.h"
#include "hci_core.h"
#include "wsf_assert.h"

#include "ll_api.h"

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set advertising set random device address command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      pAddr       Random device address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetAdvSetRandAddrCmd(uint8_t advHandle, const uint8_t *pAddr)
{
  //LlSetAdvSetRandAddr(advHandle, pAddr);  // TODO: Implement LlSetAdvSetRandAddr(advHandle, pAddr)
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended advertising parameters command.
 *
 *  \param      advHandle    Advertising handle.
 *  \param      pExtAdvParam Extended advertising parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetExtAdvParamCmd(uint8_t advHandle, hciExtAdvParam_t *pExtAdvParam)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlSetExtAdvParam(advHandle, (LlExtAdvParam_t *)pExtAdvParam);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended advertising data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      fragPref    Fragment preference.
 *  \param      len         Data buffer length.
 *  \param      pData       Advertising data buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtAdvDataCmd(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len,
                           const uint8_t *pData)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlSetExtAdvData(advHandle, op, fragPref, len, pData);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended scan response data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      fragPref    Fragment preference.
 *  \param      len         Data buffer length.
 *  \param      pData       Scan response data buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtScanRespDataCmd(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len,
                                const uint8_t *pData)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlSetExtScanRespData(advHandle, op, fragPref, len, pData);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set extended advertising enable command.
 *
 *  \param      enable        Set to TRUE to enable advertising, FALSE to disable advertising.
 *  \param      numSets       Number of advertising sets.
 *  \param      pEnableParam  Advertising enable parameter array.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetExtAdvEnableCmd(uint8_t enable, uint8_t numSets, hciExtAdvEnableParam_t *pEnableParam)
{
  LlExtAdvEnable(enable, numSets, (LlExtAdvEnableParam_t *)pEnableParam);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read maximum advertising data length command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeReadMaxAdvDataLen(void)
{
  LlReadMaxAdvDataLen(&hciCoreCb.maxAdvDataLen);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read number of supported advertising sets command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeReadNumSupAdvSets(void)
{
  LlReadNumSupAdvSets(&hciCoreCb.numSupAdvSets);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE remove advertising set command.
 *
 *  \param      advHandle    Advertising handle.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
void HciLeRemoveAdvSet(uint8_t advHandle)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlRemoveAdvSet(advHandle);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE clear advertising sets command.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeClearAdvSets(void)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlClearAdvSets();
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising parameters command.
 *
 *  \param      advHandle       Advertising handle.
 *  \param      advIntervalMin  Periodic advertising interval minimum.
 *  \param      advIntervalMax  Periodic advertising interval maximum.
 *  \param      advProps        Periodic advertising properties.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvParamCmd(uint8_t advHandle, uint16_t advIntervalMin, uint16_t advIntervalMax,
                            uint16_t advProps)
{
  LlPerAdvParam_t perAdvParam;
  uint8_t status;

  perAdvParam.perAdvInterMin = advIntervalMin;
  perAdvParam.perAdvInterMax = advIntervalMax;
  perAdvParam.perAdvProp = advProps;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlSetPeriodicAdvParam(advHandle, &perAdvParam);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      len         Data buffer length.
 *  \param      pData       Advertising data buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvDataCmd(uint8_t advHandle, uint8_t op, uint8_t len, const uint8_t *pData)
{
  uint8_t status;

  /* return value used when asserts are enabled */
  /* coverity[returned_value] */
  status = LlSetPeriodicAdvData(advHandle, op, len, pData);
  (void)status;

  WSF_ASSERT(status == LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set periodic advertising enable command.
 *
 *  \param      enable      Set to TRUE to enable advertising, FALSE to disable advertising.
 *  \param      advHandle   Advertising handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetPerAdvEnableCmd(uint8_t enable, uint8_t advHandle)
{
  LlSetPeriodicAdvEnable(advHandle, enable);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read transmit power command.
 *
 *  \return     None.
 *
 *  \todo Implement LlReadTxPower().
 */
/*************************************************************************************************/
void HciLeReadTxPower(void)
{
  /* TODO: Send the levels in an event */
  int8_t minTxPower, maxTxPower;
  LlReadTxPower(&minTxPower, &maxTxPower);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE read RF path compensation command.
 *
 *  \return     None.
 *
 *  \todo Implement LlReadRfPathComp().
 */
/*************************************************************************************************/
void HciLeReadRfPathComp(void)
{
  /* TODO: Send the levels in an event */
  int16_t txPathComp, rxPathComp;
  LlReadRfPathComp(&txPathComp, &rxPathComp);
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE write RF path compensation command.
 *
 *  \param      txPathComp      RF transmit path compensation value.
 *  \param      rxPathComp      RF receive path compensation value.
 *
 *  \return     None.
 *
 *  \todo Implement LlWriteRfPathComp().
 */
/*************************************************************************************************/
void HciLeWriteRfPathComp(int16_t txPathComp, int16_t rxPathComp)
{
  LlWriteRfPathComp(txPathComp, rxPathComp);
}

