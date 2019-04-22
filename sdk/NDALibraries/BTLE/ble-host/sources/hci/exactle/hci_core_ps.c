/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI core platform-specific module single-chip.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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
#include "wsf_trace.h"
#include "util/bda.h"
#include "util/bstream.h"
#include "hci_core.h"
#include "hci_tr.h"
#include "hci_cmd.h"
#include "hci_evt.h"
#include "hci_api.h"
#include "hci_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

static uint32_t hciEvtMask;     /* Controller Event Mask */
static uint32_t hciLeEvtMask;   /* Controller LE Event Mask */
static bool_t   hciEvtMaskMeta; /* Controller Meta bit Event Mask */

/*************************************************************************************************/
/*!
 *  \brief  HCI core initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreInit(void)
{
  LlEvtRegister(hciCoreEvtProcessLlEvt);
  LlAclRegister(hciCoreNumCmplPkts, hciCoreAclRecvPending);

  /* synchronize with LL */
  hciCoreCb.numBufs = LlGetAclTxBufs();
  hciCoreCb.availBufs = LlGetAclTxBufs();
  hciCoreCb.bufSize = LlGetAclMaxSize();
  hciCoreCb.leSupFeat = HciGetLeSupFeat();

  /* if LL Privacy is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_PRIVACY) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_PRIVACY))
  {
    LlReadResolvingListSize(&hciCoreCb.resListSize);
  }
  else
  {
    hciCoreCb.resListSize = 0;
  }

  /* if LE Extended Advertising is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_LE_EXT_ADV) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_LE_EXT_ADV))
  {
    LlReadMaxAdvDataLen(&hciCoreCb.maxAdvDataLen);
    LlReadNumSupAdvSets(&hciCoreCb.numSupAdvSets);
  }
  else
  {
    hciCoreCb.maxAdvDataLen = 0;
    hciCoreCb.numSupAdvSets = 0;
  }

  /* set the event masks to the default events */
  hciEvtMask = ((0x1 << LL_RESET_CNF) |
    (0x1 << LL_ADV_ENABLE_CNF) |
    (0x1 << LL_SCAN_ENABLE_CNF) |
    (0x1 << LL_DISCONNECT_IND) |
    (0x1 << LL_READ_REMOTE_VER_INFO_CNF) |
    (0x1 << LL_ENC_CHANGE_IND) |
    (0x1 << LL_ENC_KEY_REFRESH_IND) |
    (0x1 << LL_EXT_ADV_ENABLE_CNF) |
    (0x1 << LL_ADV_SET_TERM_IND) |
    (0x1 << LL_PER_ADV_ENABLE_CNF) |
    (0x1 << LL_ERROR_IND));

  hciLeEvtMask = ((0x1 << LL_CONN_IND) | 
    (0x1 << LL_ADV_REPORT_IND) |
    (0x1 << LL_CONN_UPDATE_IND) |
    (0x1 << LL_READ_REMOTE_FEAT_CNF) |
    (0x1 << LL_LTK_REQ_IND));

  hciEvtMaskMeta = 0;
}

/*************************************************************************************************/
/*!
 *  \brief  Process received HCI events.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return Always TRUE.
 */
/*************************************************************************************************/
bool_t hciCoreEvtProcessLlEvt(LlEvt_t *pEvt)
{
  LlEvt_t *pMsg;
  uint8_t msgLen = sizeof(LlEvt_t);

  /* Check to see if the event is unmasked */
  if ((hciEvtMask & (0x1 << pEvt->hdr.event)) || (hciEvtMaskMeta && (hciLeEvtMask & (0x1 << pEvt->hdr.event)))) 
  {
    switch (pEvt->hdr.event)
    {
      case LL_EXT_ADV_REPORT_IND:
        msgLen += pEvt->extAdvReportInd.len;
        break;

      case LL_ADV_REPORT_IND:
        msgLen += pEvt->advReportInd.len;
        break;

      default:
        break;
    }

    if ((pMsg = WsfMsgAlloc(msgLen)) != NULL)
    {
      /* copy event to message buffer */
      memcpy(pMsg, pEvt, sizeof(LlEvt_t));

      switch (pEvt->hdr.event)
      {
        case LL_ADV_REPORT_IND:
          pMsg->advReportInd.pData = (uint8_t *) (pMsg+1);
          memcpy(pMsg->advReportInd.pData, pEvt->advReportInd.pData, pEvt->advReportInd.len);
          break;

        case LL_EXT_ADV_REPORT_IND:
          pMsg->extAdvReportInd.pData = (uint8_t *) (pMsg+1);
          memcpy((uint8_t *)pMsg->extAdvReportInd.pData, pEvt->extAdvReportInd.pData, pEvt->extAdvReportInd.len);
          break;

        default:
          break;
      }

      WsfMsgEnq(&hciCb.rxQueue, HCI_EVT_TYPE, pMsg);
      WsfSetEvent(hciCb.handlerId, HCI_EVT_RX);
    }
  }
  else
  {
    /* The event is masked. For some events, the LL must be notified or other action taken. */
    switch (pEvt->hdr.event)
    {
      case LL_ENH_CONN_IND:
        /* If Enhanced Connection Complete is masked, send Connection Complete instead. */
        if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
        {
          /* copy event to message buffer */
          memcpy(pMsg, pEvt, sizeof(LlEvt_t));

          pMsg->hdr.event = LL_CONN_IND;

          WsfMsgEnq(&hciCb.rxQueue, HCI_EVT_TYPE, pMsg);
          WsfSetEvent(hciCb.handlerId, HCI_EVT_RX);
        }
        break;
      case LL_REM_CONN_PARAM_IND:
        LlRemoteConnParamReqNegReply(pEvt->hdr.param, LL_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE);
        break;
      case LL_LTK_REQ_IND:
        LlLtkReqNegReply(pEvt->hdr.param);
        break;
    }
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  LL ACL receive buffer pending handler.
 *
 *  \param  handle    Connection handle.
 *  \param  numBufs   Number of buffers completed.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreAclRecvPending(uint16_t handle, uint8_t numBufs)
{
  uint8_t *pBuf;

  while ((pBuf = LlRecvAclData()) != NULL)
  {
    WsfMsgEnq(&hciCb.rxQueue, HCI_ACL_TYPE, pBuf);
    LlRecvAclDataComplete(1);
  }

  WsfSetEvent(hciCb.handlerId, HCI_EVT_RX);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an HCI Number of Completed Packets event.
 *
 *  \param  pMsg    Message containing the HCI Number of Completed Packets event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCoreNumCmplPkts(uint16_t handle, uint8_t numBufs)
{
  hciCoreConn_t   *pConn;

  if ((pConn = hciCoreConnByHandle(handle)) != NULL)
  {
    /* decrement outstanding buffer count to controller */
    pConn->outBufs -= (uint8_t) numBufs;

    /* decrement queued buffer count for this connection */
    pConn->queuedBufs -= (uint8_t) numBufs;

    /* call flow control callback */
    if (pConn->flowDisabled && pConn->queuedBufs <=  hciCoreCb.aclQueueLo)
    {
      pConn->flowDisabled = FALSE;
      (*hciCb.flowCback)(handle, FALSE);
    }

    /* service TX data path */
    hciCoreTxReady(numBufs);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for core HCI.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciCoreHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  uint8_t         *pBuf;
  wsfHandlerId_t  handlerId;

  /* Handle events */
  if (event & HCI_EVT_RX)
  {
    /* Process rx queue */
    while ((pBuf = WsfMsgDeq(&hciCb.rxQueue, &handlerId)) != NULL)
    {
      /* Handle incoming HCI events */
      if (handlerId == HCI_EVT_TYPE)
      {
        /* Parse/process events */
        hciEvtProcessMsg(pBuf);

        /* Free buffer */
        WsfMsgFree(pBuf);
      }
      /* Handle ACL data */
      else
      {
        /* Reassemble */
        if ((pBuf = hciCoreAclReassembly(pBuf)) != NULL)
        {
          /* Call ACL callback; client will free buffer */
          hciCb.aclCback(pBuf);
        }
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Return a pointer to the BD address of this device.
 *
 *  \return Pointer to the BD address.
 */
/*************************************************************************************************/
uint8_t *HciGetBdAddr(void)
{
  return hciCoreCb.bdAddr;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the white list size.
 *
 *  \return White list size.
 */
/*************************************************************************************************/
uint8_t HciGetWhiteListSize(void)
{
  return LlGetWhitelistSize();
}

/*************************************************************************************************/
/*!
 *  \brief  Return the advertising transmit power.
 *
 *  \return Advertising transmit power.
 */
/*************************************************************************************************/
int8_t HciGetAdvTxPwr(void)
{
  int8_t advTxPwr = 0;
  LlGetAdvTxPower(&advTxPwr);
  return advTxPwr;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the ACL buffer size supported by the controller.
 *
 *  \return ACL buffer size.
 */
/*************************************************************************************************/
uint16_t HciGetBufSize(void)
{
  return hciCoreCb.bufSize;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the number of ACL buffers supported by the controller.
 *
 *  \return Number of ACL buffers.
 */
/*************************************************************************************************/
uint8_t HciGetNumBufs(void)
{
  return hciCoreCb.numBufs;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the states supported by the controller.
 *
 *  \return Pointer to the supported states array.
 */
/*************************************************************************************************/
uint8_t *HciGetSupStates(void)
{
  static uint8_t supStates[8];

  LlGetSupStates(supStates);

  return supStates;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the LE supported features supported by the controller.
 *
 *  \return Supported features.
 */
/*************************************************************************************************/
uint16_t HciGetLeSupFeat(void)
{
  uint16_t supFeat;
  uint8_t  feat[HCI_LE_STATES_LEN];

  LlGetFeatures(feat);

  BYTES_TO_UINT16(supFeat, feat);

  return supFeat;
}

/*************************************************************************************************/
/*!
 *  \brief  Get the maximum reassembled RX ACL packet length.
 *
 *  \return ACL packet length.
 */
/*************************************************************************************************/
uint16_t HciGetMaxRxAclLen(void)
{
  return hciCoreCb.maxRxAclLen;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the resolving list size.
 *
 *  \return resolving list size.
 */
/*************************************************************************************************/
uint8_t HciGetResolvingListSize(void)
{
  return hciCoreCb.resListSize;
}

/*************************************************************************************************/
/*!
*  \brief  Whether LL Privacy is supported.
*
*  \return TRUE if LL Privacy is supported. FALSE, otherwise.
*/
/*************************************************************************************************/
bool_t HciLlPrivacySupported(void)
{
  return (hciCoreCb.resListSize > 0) ? TRUE : FALSE;
}

/*************************************************************************************************/
/*!
*  \brief  Get the maximum advertisement (or scan response) data length supported by the Controller.
*
*  \return Maximum advertisement data length.
*/
/*************************************************************************************************/
uint16_t HciGetMaxAdvDataLen(void)
{
  return hciCoreCb.maxAdvDataLen;
}

/*************************************************************************************************/
/*!
*  \brief  Get the maximum number of advertising sets supported by the Controller.
*
*  \return Maximum number of advertising sets.
*/
/*************************************************************************************************/
uint8_t HciGetNumSupAdvSets(void)
{
  return hciCoreCb.numSupAdvSets;
}

/*************************************************************************************************/
/*!
*  \brief  Whether LE Advertising Extensions is supported.
*
*  \return TRUE if LE Advertising Extensions is supported. FALSE, otherwise.
*/
/*************************************************************************************************/
bool_t HciLeAdvExtSupported(void)
{
  return (hciCoreCb.numSupAdvSets > 0) ? TRUE : FALSE;
}

/*************************************************************************************************/
/*!
 *  \fn     HciLeSetEventMaskCmd
 *
 *  \brief  HCI LE set event mask command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetEventMaskCmd(uint8_t *pLeEventMask)
{
  // Translate the HCI_EVT_MASK_LE to the LL Events
  if (pLeEventMask[0] & HCI_EVT_MASK_LE_CONN_CMPL_EVT) {
    hciLeEvtMask |= (0x1 << LL_CONN_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_CONN_IND);
  }
  if (pLeEventMask[0] & HCI_EVT_MASK_LE_ADV_REPORT_EVT) {
    hciLeEvtMask |= (0x1 << LL_ADV_REPORT_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_ADV_REPORT_IND);
  }
  if (pLeEventMask[0] & HCI_EVT_MASK_LE_CONN_UPDATE_CMPL_EVT) {
    hciLeEvtMask |= (0x1 << LL_CONN_UPDATE_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_CONN_UPDATE_IND);
  }
  if (pLeEventMask[0] & HCI_EVT_MASK_LE_READ_REMOTE_FEAT_CMPL_EVT) {
    hciLeEvtMask |= (0x1 << LL_READ_REMOTE_FEAT_CNF);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_READ_REMOTE_FEAT_CNF);
  }
  if (pLeEventMask[0] & HCI_EVT_MASK_LE_LTK_REQ_EVT) {
    hciLeEvtMask |= (0x1 << LL_LTK_REQ_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_LTK_REQ_IND);
  }
  if (pLeEventMask[0] & HCI_EVT_MASK_LE_REMOTE_CONN_PARAM_REQ_EVT) {
    hciLeEvtMask |= (0x1 << LL_REM_CONN_PARAM_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_REM_CONN_PARAM_IND);
  }
  if (pLeEventMask[0] & HCI_EVT_MASK_LE_DATA_LEN_CHANGE_EVT) {
    hciLeEvtMask |= (0x1 << LL_DATA_LEN_CHANGE_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_DATA_LEN_CHANGE_IND);
  }
  if (pLeEventMask[1] & HCI_EVT_MASK_LE_ENHANCED_CONN_CMPL_EVT) {
    hciLeEvtMask |= (0x1 << LL_ENH_CONN_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_ENH_CONN_IND);
  }
  if (pLeEventMask[1] & HCI_EVT_MASK_LE_DIRECT_ADV_REPORT_EVT) {
    hciLeEvtMask |= (0x1 << LL_DIR_ADV_REPORT_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_DIR_ADV_REPORT_IND);
  }
  if (pLeEventMask[1] & HCI_EVT_MASK_LE_EXT_ADV_REPORT_EVT) {
    hciLeEvtMask |= (0x1 << LL_EXT_ADV_REPORT_IND);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_EXT_ADV_REPORT_IND);
  }
  if (pLeEventMask[1] & HCI_EVT_MASK_LE_EXT_ADV_REPORT_EVT) {
    hciLeEvtMask |= (0x1 << LL_EXT_SCAN_ENABLE_CNF);
  } else {
    hciLeEvtMask &= ~(0x1 << LL_EXT_SCAN_ENABLE_CNF);
  }
}

/*************************************************************************************************/
/*!
 *  \fn     HciSetEventMaskCmd
 *
 *  \brief  HCI set event mask command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciSetEventMaskCmd(uint8_t *pEventMask)
{
  // Translate the HCI_EVT_MASK to the LL Events
  if (pEventMask[0] & HCI_EVT_MASK_DISCONNECT_CMPL) {
    hciEvtMask |= (0x1 << LL_DISCONNECT_IND);
  } else {
    hciEvtMask &= ~(0x1 << LL_DISCONNECT_IND);
  }
  if (pEventMask[0] & HCI_EVT_MASK_ENC_CHANGE) {
    hciEvtMask |= (0x1 <<  LL_ENC_CHANGE_IND);
  } else {
    hciEvtMask &= ~(0x1 <<  LL_ENC_CHANGE_IND);
  }
  if (pEventMask[1] & HCI_EVT_MASK_READ_REMOTE_VER_INFO_CMPL) {
    hciEvtMask |= (0x1 << LL_READ_REMOTE_VER_INFO_CNF);
  } else {
    hciEvtMask &= ~(0x1 << LL_READ_REMOTE_VER_INFO_CNF);
  }
  if (pEventMask[1] & HCI_EVT_MASK_HW_ERROR) {
    hciEvtMask |= (0x1 << LL_ERROR_IND);
  } else {
    hciEvtMask &= ~(0x1 << LL_ERROR_IND);
  }
  if (pEventMask[5] & HCI_EVT_MASK_ENC_KEY_REFRESH_CMPL) {
    hciEvtMask |= (0x1 << LL_ENC_KEY_REFRESH_IND);
  } else {
    hciEvtMask &= ~(0x1 << LL_ENC_KEY_REFRESH_IND);
  }
  if (pEventMask[7] & HCI_EVT_MASK_LE_META) {
    hciEvtMaskMeta = 1;
  } else {
    hciEvtMaskMeta = 0;
  }
}

/*************************************************************************************************/
/*!
 *  \fn     HciSetEventMaskPage2Cmd
 *
 *  \brief  HCI set event mask Page 2 command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciSetEventMaskPage2Cmd(uint8_t *pEventMask)
{
  // Translate the HCI_EVT_MASK to the LL Events
  if (pEventMask[2] & HCI_EVT_MASK_AUTH_PAYLOAD_TIMEOUT) {
    hciEvtMask |= (0x1 << LL_AUTH_PAYLOAD_TIMEOUT_IND);
  } else {
    hciEvtMask &= ~(0x1 << LL_AUTH_PAYLOAD_TIMEOUT_IND);
  }
}

/*************************************************************************************************/
/*!
 *  \fn     HciGetLeSugDataLenCmd
 *
 *  \brief  Returns the suggested maximum TX octets and time.
 *
 *  \return Status code.
 */
/*************************************************************************************************/
void HciLeGetDefDataLen(uint16_t *txOctets, uint16_t *txTime)
{
  LlReadDefaulTxDataLen(txOctets, txTime);
}

/*************************************************************************************************/
/*!
 *  \fn     HciGetLeMaxDataLenCmd
 *
 *  \brief  Returns the maximum TX and RX octets and time
 *
 *  \return Status code.
 */
/*************************************************************************************************/
void HciLeGetMaxDataLen(uint16_t *txOctets, uint16_t *txTime, uint16_t *rxOctets, uint16_t *rxTime)
{
  LlReadMaximumDataLen(txOctets, txTime, rxOctets, rxTime);
}
