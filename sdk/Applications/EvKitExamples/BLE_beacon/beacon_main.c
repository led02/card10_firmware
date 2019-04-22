/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Data transmitter sample application.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
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

#include <stdio.h>
#include <string.h>
#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_buf.h"
#include "wsf_assert.h"
#include "hci_api.h"
#include "sec_api.h"
#include "dm_api.h"
#include "smp_api.h"
#include "att_api.h"
#include "app_api.h"
#include "app_main.h"
#include "app_db.h"
#include "app_ui.h"
#include "svc_ch.h"
#include "svc_core.h"
#include "svc_wp.h"
#include "util/calc128.h"
#include "beacon_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Enumeration of client characteristic configuration descriptors */
enum
{
  BEACON_GATT_SC_CCC_IDX,           /*! GATT service, service changed characteristic */
  BEACON_WP_DAT_CCC_IDX,            /*! ARM Ltd. proprietary service, data transfer characteristic */
  BEACON_NUM_CCC_IDX
};

#ifndef BEACON_APP_ADV_TYPE

/* Non-scannable and connectable undirected advertising */
//#define BEACON_APP_ADV_TYPE DM_EXT_ADV_CONN_UNDIRECT

/* Scannable and non-connectable directed advertising */
//#define BEACON_APP_ADV_TYPE DM_EXT_ADV_SCAN_DIRECT

/* Non-scannable and non-connectable directed advertising */
#define BEACON_APP_ADV_TYPE DM_EXT_ADV_NONCONN_DIRECT

#endif /* BEACON_APP_ADV_TYPE */

#ifndef BEACON_APP_MODE
#define BEACON_APP_MODE APP_MODE_DISCOVERABLE
//#define BEACON_APP_MODE APP_MODE_CONNECTABLE
#endif /* BEACON_PP_MODEE */

#ifndef BEACON_APP_PEER_ADDR
#define BEACON_APP_PEER_ADDR "\x01\x03\x44\x8B\x05"
#endif /* BEACON_APP_PEER_ADDR */

#ifndef BEACON_APP_USE_LEGACY_PDU
#define BEACON_APP_USE_LEGACY_PDU 0
#endif /* BEACON_APP_USE_LEGACY_PDU */

#ifndef BLE_BEACON_USE_ANON_ADV
#define BLE_BEACON_USE_ANON_ADV 0
#endif /* BLE_BEACON_USE_ANON_ADV */

#ifndef BLE_BEACON_USE_FRAG_PREF
#define BLE_BEACON_USE_FRAG_PREF 1
#endif /* BLE_BEACON_USE_FRAG_PREF */

#ifndef BLE_BEACON_USE_PER_ADV
#define BLE_BEACON_USE_PER_ADV 0
#endif /* BLE_BEACON_USE_PER_ADV */

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

/*! configurable parameters for advertising */
static const appExtAdvCfg_t beaconExtAdvCfg =
{
  {    0},                  /*!< \brief Advertising durations for extended advertising in ms */
  {  200},                  /*!< \brief Advertising intervals for extended advertising in 0.625
                                                    ms units (20 ms to 10.24 s). */
  {    0},                  /*!< \brief Maximum number of extended advertising events Controller
                                                    will send prior to terminating extended advertising */

                            /*!  \brief Whether to use legacy advertising PDUs with extended
                                                    advertising. If set to TRUE then length of advertising
                                                    data cannot exceed 31 octets. */
#if BEACON_APP_USE_LEGACY_PDU
  {FALSE},
#else /* BEACON_APP_USE_LEGACY_PDU */
  {TRUE},
#endif /* BEACON_APP_USE_LEGACY_PDU */

  {    0}                   /*!< \brief Advertising intervals for periodic advertising in 1.25
                                                    ms units (7.5 ms to 81.91875 s). */
};

/*! configurable parameters for slave */
static const appSlaveCfg_t beaconSlaveCfg =
{
  1,                                      /*! Maximum connections */
};

/*! TRUE if Out-of-band pairing data is to be sent */
static const bool_t beaconSendOobData = FALSE;

/*! SMP security parameter configuration */
static const attCfg_t beaconAttCfg =
{
  15,                               /* ATT server service discovery connection idle timeout in seconds */
  241,                              /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,            /* transcation timeout in seconds */
  4                                 /* number of queued prepare writes supported by server */
};

/**************************************************************************************************
  Advertising Data
**************************************************************************************************/

/*! advertising data, discoverable mode */
static const uint8_t beaconAdvDataDisc[] =
{
  /*! flags */
  2,                                      /*! length */
  DM_ADV_TYPE_FLAGS,                      /*! AD type */
  DM_FLAG_LE_GENERAL_DISC |               /*! flags */
  DM_FLAG_LE_BREDR_NOT_SUP,

  /*! manufacturer specific data */
  3,                                      /*! length */
  DM_ADV_TYPE_MANUFACTURER,               /*! AD type */
  UINT16_TO_BYTES(HCI_ID_ARM),            /*! company ID */

  /*! device name */
#if BEACON_APP_USE_LEGACY_PDU
  17,                                    /*! length */
#else /* BEACON_APP_USE_LEGACY_PDU */
  102,                                    /*! length */
#endif /* BEACON_APP_USE_LEGACY_PDU */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'e',
  'x',
  't',
  ' ',
  'a',
  'd',
  'v',
  ' ',
  'b',
  'a',
  's',
  'i',
  'c',
  ' ',
  'c',
  'o'
#if ! BEACON_APP_USE_LEGACY_PDU
  ,
  'n',
  'n',
  'e',
  'c',
  't',
  'i',
  'o',
  'n',
  ' ',
  'd',
  'a',
  't',
  's',
  ' ',
  'a',
  'p',
  'p',
  ' ',
  'e',
  'x',
  't',
  ' ',
  'a',
  'd',
  'v',
  ' ',
  'b',
  'a',
  's',
  'i',
  'c',
  ' ',
  'c',
  'o',
  'n',
  'n',
  'e',
  'c',
  't',
  'i',
  'o',
  'n',
  ' ',
  'd',
  'a',
  't',
  's',
  ' ',
  'a',
  'p',
  'p',
  ' ',
  'e',
  'x',
  't',
  ' ',
  'a',
  'd',
  'v',
  ' ',
  'b',
  'a',
  's',
  'i',
  'c',
  ' ',
  'c',
  'o',
  'n',
  'n',
  'e',
  'c',
  't',
  'i',
  'o',
  'n',
  ' ',
  'd',
  'a',
  't',
  's',
  ' ',
  'a',
  'p',
  'p'
#endif /* BEACON_APP_USE_LEGACY_PDU */
};

/*! scan data, discoverable mode */
static const uint8_t beaconScanDataDisc[] =
{
  /*! device name */
  11,                                      /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'B',
  'L',
  'E',
  ' ',
  'b',
  'e',
  'a',
  'c',
  'o',
  'n',
  /*! device name */
#if BEACON_APP_USE_LEGACY_PDU
  18,                                    /*! length */
#else /* BEACON_APP_USE_LEGACY_PDU */
  102,                                    /*! length */
#endif /* BEACON_APP_USE_LEGACY_PDU */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  's',
  'c',
  'a',
  'n',
  ' ',
  'r',
  'e',
  's',
  'p',
  'o',
  'n',
  's',
  'e',
  ' ',
  't',
  'o',
  ' '
#if BEACON_APP_USE_LEGACY_PDU
  ,
  'a',
  'u',
  'x',
  ' ',
  's',
  'c',
  'a',
  'n',
  ' ',
  'r',
  'e',
  'q',
  'u',
  'e',
  's',
  't',
  ' ',
  's',
  'c',
  'a',
  'n',
  ' ',
  'r',
  'e',
  's',
  'p',
  'o',
  'n',
  's',
  'e',
  ' ',
  't',
  'o',
  ' ',
  'a',
  'u',
  'x',
  ' ',
  's',
  'c',
  'a',
  'n',
  ' ',
  'r',
  'e',
  'q',
  'u',
  'e',
  's',
  't',
  ' ',
  's',
  'c',
  'a',
  'n',
  ' ',
  'r',
  'e',
  's',
  'p',
  'o',
  'n',
  's',
  'e',
  ' ',
  't',
  'o',
  ' ',
  'a',
  'u',
  'x',
  ' ',
  's',
  'c',
  'a',
  'n',
  ' ',
  'r',
  'e',
  'q',
  'u',
  'e',
  's',
  't'
#endif /* BEACON_APP_USE_LEGACY_PDU */
};

/*! advertising data, connactable mode */
static const uint8_t datsAdvDataConn[] =
{
  /*! device name */
  11,                                      /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'B',
  'L',
  'E',
  ' ',
  'b',
  'e',
  'a',
  'c',
  'o',
  'n'
};

/**************************************************************************************************
  Client Characteristic Configuration Descriptors
**************************************************************************************************/

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t beaconCccSet[BEACON_NUM_CCC_IDX] =
{
  /* cccd handle          value range               security level */
  {GATT_SC_CH_CCC_HDL,    ATT_CLIENT_CFG_INDICATE,  DM_SEC_LEVEL_NONE},   /* BEACON_GATT_SC_CCC_IDX */
  {WP_DAT_CH_CCC_HDL,     ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE}    /* BEACON_WP_DAT_CCC_IDX */
};

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! application control block */
static struct
{
  wsfHandlerId_t    handlerId;        /* WSF handler ID */
} beaconCb;

/* LESC OOB configuration */
static dmSecLescOobCfg_t *beaconOobCfg;

/*************************************************************************************************/
/*!
 *  \brief  Send notification containing data.
 *
 *  \param  connId      DM connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void beaconSendData(dmConnId_t connId)
{
  uint8_t str[] = "hello back";

  if (AttsCccEnabled(connId, BEACON_WP_DAT_CCC_IDX))
  {
    /* send notification */
    AttsHandleValueNtf(connId, WP_DAT_HDL, sizeof(str), str);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void beaconDmCback(dmEvt_t *pDmEvt)
{
  dmEvt_t   *pMsg;
  uint16_t  len;

  if (pDmEvt->hdr.event == DM_SEC_ECC_KEY_IND)
  {
    DmSecSetEccKey(&pDmEvt->eccMsg.data.key);

    // If the local device sends OOB data.
    if (beaconSendOobData)
    {
      uint8_t oobLocalRandom[SMP_RAND_LEN];
      SecRand(oobLocalRandom, SMP_RAND_LEN);
      DmSecCalcOobReq(oobLocalRandom, pDmEvt->eccMsg.data.key.pubKey_x);
    }
  }
  else if (pDmEvt->hdr.event == DM_SEC_CALC_OOB_IND)
  {
    if (beaconOobCfg == NULL)
    {
      beaconOobCfg = WsfBufAlloc(sizeof(dmSecLescOobCfg_t));
    }

    if (beaconOobCfg)
    {
      Calc128Cpy(beaconOobCfg->localConfirm, pDmEvt->oobCalcInd.confirm);
      Calc128Cpy(beaconOobCfg->localRandom, pDmEvt->oobCalcInd.random);
    }
  }
  else
  {
    len = DmSizeOfEvt(pDmEvt);

    if ((pMsg = WsfMsgAlloc(len)) != NULL)
    {
      memcpy(pMsg, pDmEvt, len);
      WsfMsgSend(beaconCb.handlerId, pMsg);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application ATT callback.
 *
 *  \param  pEvt    ATT callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void beaconAttCback(attEvt_t *pEvt)
{
}

/*************************************************************************************************/
/*!
 *  \brief  Application ATTS client characteristic configuration callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void beaconCccCback(attsCccEvt_t *pEvt)
{
  appDbHdl_t    dbHdl;

  /* if CCC not set from initialization and there's a device record */
  if ((pEvt->handle != ATT_HANDLE_NONE) &&
      ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE))
  {
    /* store value in device database */
    AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for proprietary data service.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t beaconWpWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                          uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
  /* print received data */
  APP_TRACE_INFO0((const char*) pValue);

  /* send back some data */
  beaconSendData(connId);

  return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
*
*  \brief  Get peer key from a device database record.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return Pointer to peer key if key is valid or NULL if not valid.
*/
/*************************************************************************************************/
static dmSecKey_t *beaconGetPeerKey(dmEvt_t *pMsg)
{
  appConnCb_t *pCb;

  /* look up app connection control block from DM connection ID */
  pCb = &appConnCb[pMsg->hdr.param - 1];

  /* if database record handle valid */
  if (pCb->dbHdl != APP_DB_HDL_NONE)
  {
    return AppDbGetKey(pCb->dbHdl, DM_KEY_IRK, NULL);
  }

  return NULL;
}

/*************************************************************************************************/
/*!
*
*  \brief  Handle add device to resolving list indication.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return None.
*/
/*************************************************************************************************/
static void beaconPrivAddDevToResListInd(dmEvt_t *pMsg)
{
  dmSecKey_t *pPeerKey;

  /* if peer IRK present */
  if ((pPeerKey = beaconGetPeerKey(pMsg)) != NULL)
  {
    /* set advertising peer address */
    AppExtSetAdvPeerAddr(DM_ADV_HANDLE_DEFAULT, pPeerKey->irk.addrType, pPeerKey->irk.bdAddr);
  }
}

/*************************************************************************************************/
/*!
*
*  \brief  Handle remove device from resolving list indication.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return None.
*/
/*************************************************************************************************/
static void beaconPrivRemDevFromResListInd(dmEvt_t *pMsg)
{
  uint8_t * pPeerAddr;

  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    appConnCb_t *pCb;

    /* look up app connection control block from DM connection ID */
    pCb = &appConnCb[pMsg->hdr.param - 1];

    if (pCb->dbHdl != APP_DB_HDL_NONE)
    {
      pPeerAddr = (uint8_t *) BEACON_APP_PEER_ADDR;

      /* clear advertising peer address and its type */
      AppExtSetAdvPeerAddr(DM_ADV_HANDLE_DEFAULT, HCI_ADDR_TYPE_PUBLIC, pPeerAddr);
    }
  }
}

/*************************************************************************************************/
/*!
 *
 *  \brief  Display stack version.
 *
 *  \param  version    version number.
 *
 *  \return None.
 */
/*************************************************************************************************/
void beaconDisplayStackVersion(const char *pVersion)
{
  APP_TRACE_INFO1("Stack Version: %s", pVersion);
}

/*************************************************************************************************/
/*!
 *  \brief  Set up advertising and other procedures that need to be performed after
 *          device reset.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void beaconSetup(dmEvt_t *pMsg)
{
  uint8_t advHandle;
#if BLE_BEACON_USE_PER_ADV
  uint8_t periodicAdvHandle;
#endif /* BLE_BEACON_USE_PER_ADV */
  uint8_t chmap;

  chmap = 0x7; // Bit 0 -> Channel 37; Bit 1 -> Channel 38; Bit 2 -> Channel 39;

  /* set advertising and scan response data for discoverable mode */
  AppExtAdvSetData(DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_DISCOVERABLE, sizeof(beaconAdvDataDisc), (uint8_t *) beaconAdvDataDisc, HCI_EXT_ADV_DATA_LEN);
  AppExtAdvSetData(DM_ADV_HANDLE_DEFAULT, APP_SCAN_DATA_DISCOVERABLE, sizeof(beaconScanDataDisc), (uint8_t *) beaconScanDataDisc, HCI_EXT_ADV_DATA_LEN);

  /* set advertising data for connectable mode */
  AppExtAdvSetData(DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_CONNECTABLE, sizeof(datsAdvDataConn), (uint8_t *) datsAdvDataConn, HCI_EXT_ADV_DATA_LEN);

  /* set advertising type */
  AppExtSetAdvType(DM_ADV_HANDLE_DEFAULT, BEACON_APP_ADV_TYPE);

  /* Primary_Advertising_Channel_Map update */
  DmAdvSetChannelMap(DM_ADV_HANDLE_DEFAULT, chmap);
  DmAdvIncTxPwr(DM_ADV_HANDLE_DEFAULT, TRUE, HCI_TX_PWR_MIN);

#if BLE_BEACON_USE_ANON_ADV
  /* Anonymous Advertising */
  DmAdvOmitAdvAddr(DM_ADV_HANDLE_DEFAULT, TRUE);
#endif /* BLE_BEACON_USE_ANON_ADV */

#if BLE_BEACON_USE_FRAG_PREF
  DmAdvSetFragPref(DM_ADV_HANDLE_DEFAULT, HCI_ADV_DATA_FRAG_PREF_FRAG);
#else /* BLE_BEACON_USE_FRAG_PREF */
  DmAdvSetFragPref(DM_ADV_HANDLE_DEFAULT, HCI_ADV_DATA_FRAG_PREF_NO_FRAG);
#endif /* BLE_BEACON_USE_FRAG_PREF */

  /* start advertising; automatically set connectable/discoverable mode and bondable mode */
  advHandle = DM_ADV_HANDLE_DEFAULT;

  // Set advertising mode.
  AppExtAdvStart(1, &advHandle, BEACON_APP_MODE);

#if BLE_BEACON_USE_PER_ADV
  // Select handle used for periodic advertising.
  periodicAdvHandle = DM_ADV_HANDLE_DEFAULT;

  // Start periodic advertising.
  AppPerAdvStart(perAdvHandle);
#endif /* BLE_BEACON_USE_PER_ADV */
}

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void beaconProcMsg(dmEvt_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;

  switch(pMsg->hdr.event)
  {
    case DM_RESET_CMPL_IND:
      DmSecGenerateEccKeyReq();
      beaconSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_ADV_START_IND:
      uiEvent = APP_UI_ADV_START;
      break;

    case DM_ADV_STOP_IND:
      uiEvent = APP_UI_ADV_STOP;
      break;

    case DM_ADV_SET_START_IND:
      uiEvent = APP_UI_ADV_SET_START_IND;
      break;

    case DM_ADV_SET_STOP_IND:
      uiEvent = APP_UI_ADV_SET_STOP_IND;
      break;

    case DM_CONN_OPEN_IND:
      uiEvent = APP_UI_CONN_OPEN;
      break;

    case DM_CONN_CLOSE_IND:
      uiEvent = APP_UI_CONN_CLOSE;
      break;

    case DM_SEC_PAIR_CMPL_IND:
      uiEvent = APP_UI_SEC_PAIR_CMPL;
      break;

    case DM_SEC_PAIR_FAIL_IND:
      uiEvent = APP_UI_SEC_PAIR_FAIL;
      break;

    case DM_SEC_ENCRYPT_IND:
      uiEvent = APP_UI_SEC_ENCRYPT;
      break;

    case DM_SEC_ENCRYPT_FAIL_IND:
      uiEvent = APP_UI_SEC_ENCRYPT_FAIL;
      break;

    case DM_SEC_AUTH_REQ_IND:

      if (pMsg->authReq.oob)
      {
        dmConnId_t connId = (dmConnId_t) pMsg->hdr.param;

        /* TODO: Perform OOB Exchange with the peer. */


        /* TODO: Fill beaconOobCfg peerConfirm and peerRandom with value passed out of band */

        if (beaconOobCfg != NULL)
        {
          DmSecSetOob(connId, beaconOobCfg);
        }

        DmSecAuthRsp(connId, 0, NULL);
      }
      else
      {
        AppHandlePasskey(&pMsg->authReq);
      }
      break;

    case DM_SEC_COMPARE_IND:
      AppHandleNumericComparison(&pMsg->cnfInd);
      break;

    case DM_PRIV_ADD_DEV_TO_RES_LIST_IND:
      beaconPrivAddDevToResListInd(pMsg);
      break;

    case DM_PRIV_REM_DEV_FROM_RES_LIST_IND:
      beaconPrivRemDevFromResListInd(pMsg);
      break;

    case DM_ADV_NEW_ADDR_IND:
      break;

    default:
      break;
  }

  if (uiEvent != APP_UI_NONE)
  {
    AppUiAction(uiEvent);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application handler init function called during system initialization.
 *
 *  \param  handlerID  WSF handler ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BeaconHandlerInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("BeaconHandlerInit");

  /* store handler ID */
  beaconCb.handlerId = handlerId;

  /* Set configuration pointers */
  pAppSlaveCfg = (appSlaveCfg_t *) &beaconSlaveCfg;
  pAppExtAdvCfg = (appExtAdvCfg_t *) &beaconExtAdvCfg;
  pAttCfg = (attCfg_t *) &beaconAttCfg;

  /* Initialize application framework */
  AppSlaveInit();
}

/*************************************************************************************************/
/*!
 *  \brief  Callback for WSF buffer diagnostic messages.
 *
 *  \param  pInfo     Diagnostics message
 *
 *  \return None.
 */
/*************************************************************************************************/
static void beaconWsfBufDiagnostics(WsfBufDiag_t *pInfo)
{
  if (pInfo->type == WSF_BUF_ALLOC_FAILED)
  {
    APP_TRACE_INFO2("Beacon got WSF Buffer Allocation Failure - Task: %d Len: %d",
                     pInfo->param.alloc.taskId, pInfo->param.alloc.len);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for application.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BeaconHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (pMsg != NULL)
  {
    APP_TRACE_INFO1("Beacon got evt 0x%x", pMsg->event);

    if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END && pMsg->event != DM_ADV_SET_STOP_IND)
    {
      /* process advertising and connection-related messages */
      AppSlaveProcDmMsg((dmEvt_t *) pMsg);

      /* process security-related messages */
      AppSlaveSecProcDmMsg((dmEvt_t *) pMsg);
    }

    /* perform profile and user interface-related operations */
    beaconProcMsg((dmEvt_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BeaconStart(void)
{
  /* Register for stack callbacks */
  DmRegister(beaconDmCback);
  DmConnRegister(DM_CLIENT_ID_APP, beaconDmCback);
  AttRegister(beaconAttCback);
  AttConnRegister(AppServerConnCback);
  AttsCccRegister(BEACON_NUM_CCC_IDX, (attsCccSet_t *) beaconCccSet, beaconCccCback);

  /* Initialize attribute server database */
  SvcCoreAddGroup();
  SvcWpCbackRegister(NULL, beaconWpWriteCback);
  SvcWpAddGroup();

  WsfBufDiagRegister(beaconWsfBufDiagnostics);

  /* Reset the device */
  DmDevReset();
}
