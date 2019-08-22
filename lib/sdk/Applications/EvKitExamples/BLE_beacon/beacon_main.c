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
#include "hci_vs.h"
#include "dm_api.h"
#include "smp_api.h"
#include "att_api.h"
#include "app_api.h"
#include "app_main.h"
#include "app_db.h"
#include "app_ui.h"
#include "util/calc128.h"
#include "beacon_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

// Defined in dm_adv_ae.c
#define DM_ADV_SCANNABLE(advType)           (((advType) == DM_ADV_CONN_UNDIRECT)      || \
                                             ((advType) == DM_ADV_SCAN_UNDIRECT)      || \
                                             ((advType) == DM_EXT_ADV_SCAN_DIRECT))

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

/*! configurable parameters for advertising */
/* These intervals directly impact energy usage during the non-connected/advertising mode */
#ifdef BTLE_APP_USE_LEGACY_API
static const appAdvCfg_t beaconAdvCfg =
{
  { 1000,     0,     0},                  /*! Advertising durations in ms */
  {   96,   200,     0}                   /*! Advertising intervals in 0.625 ms units */
};
#else /* BTLE_APP_USE_LEGACY_API */
static const appExtAdvCfg_t beaconExtAdvCfg =
{
  {    0,     0,     0}, /*! Advertising durations for extended advertising in ms */
  {   96,   200,     0}, /*! Advertising intervals for extended advertising in 0.625 ms units (20 ms to 10.24 s). */
  {    0,     0,     0}, /*! Maximum number of extended advertising events controller will send prior to terminating extended advertising */
  {    0,     0,     0}, /*! Whether to use legacy advertising PDUs with extended advertising. If set to TRUE then length of advertising data cannot exceed 31 octets. */
  {    6,     0,     0}  /*! Advertising intervals for periodic advertising in 1.25 ms units (7.5 ms to 81.91875 s). */
};

#endif /* BTLE_APP_USE_LEGACY_API */

/*! configurable parameters for slave */
static const appSlaveCfg_t beaconSlaveCfg =
{
  1,                                      /*! Maximum connections */
};

/**************************************************************************************************
  Advertising Data
**************************************************************************************************/

#ifdef BTLE_APP_USE_LEGACY_API
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
  UINT16_TO_BYTES(HCI_ID_ARM)             /*! company ID */
};

/*! scan data, discoverable mode */
static const uint8_t beaconScanDataDisc[] =
{
  /*! device name */
  13,                                     /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'M',
  'a',
  'x',
  'i',
  'm',
  ' ',
  'B',
  'e',
  'a',
  'c',
  'o',
  'n'
};

#else /* BTLE_APP_USE_LEGACY_API */

#if (defined(BTLE_APP_ADV_TYPE) && (DM_ADV_SCANNABLE(BTLE_APP_ADV_TYPE)))

static const uint8_t beaconExtScanDataDisc[] =
{
  /*! device name */
  13,                                    /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'M',
  'a',
  'x',
  'i',
  'm',
  ' ',
  'B',
  'e',
  'a',
  'c',
  'o',
  'n'
};

#else /* (defined(BTLE_APP_ADV_TYPE) && (DM_ADV_SCANNABLE(BTLE_APP_ADV_TYPE))) */

/*! extended advertising and scan data, discoverable mode */
static const uint8_t beaconExtAdvDataDisc[] =
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
  13,                                     /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'M',
  'a',
  'x',
  'i',
  'm',
  ' ',
  'B',
  'e',
  'a',
  'c',
  'o',
  'n'
};

#endif /* (defined(BTLE_APP_ADV_TYPE) && (DM_ADV_SCANNABLE(BTLE_APP_ADV_TYPE))) */

#ifdef BTLE_APP_ENABLE_PERIODIC
static const uint8_t beaconPerAdvData[] =
{
  /*! device name */
  22,                                    /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'M',
  'a',
  'x',
  'i',
  'm',
  ' ',
  'P',
  'e',
  'r',
  'i',
  'o',
  'd',
  'i',
  'c',
  ' ',
  'B',
  'e',
  'a',
  'c',
  'o',
  'n'
};

#endif /* BTLE_APP_ENABLE_PERIODIC */

#endif /* BTLE_APP_USE_LEGACY_API */

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! application control block */
static struct
{
  wsfHandlerId_t    handlerId;        /* WSF handler ID */
} beaconCb;


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

  len = DmSizeOfEvt(pDmEvt);

  if ((pMsg = WsfMsgAlloc(len)) != NULL)
  {
    memcpy(pMsg, pDmEvt, len);
    WsfMsgSend(beaconCb.handlerId, pMsg);
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
#ifndef BTLE_APP_USE_LEGACY_API
  uint8_t aAdvHandles[1];
  const size_t nAdvHandles = (sizeof(aAdvHandles)/sizeof(aAdvHandles[0]));
#endif /* BTLE_APP_USE_LEGACY_API */

  /* For extended advertisements, select handle. */
#ifndef BTLE_APP_USE_LEGACY_API
  aAdvHandles[0] = DM_ADV_HANDLE_DEFAULT;
#endif /* BTLE_APP_USE_LEGACY_API */

  /* Send scannable advertisements. */
#ifdef BTLE_APP_ADV_TYPE
#ifdef BTLE_APP_USE_LEGACY_API
  AppSetAdvType(BTLE_APP_ADV_TYPE);
#else /* BTLE_APP_USE_LEGACY_API */
  AppExtSetAdvType(aAdvHandles[0], BTLE_APP_ADV_TYPE);
#endif /* BTLE_APP_USE_LEGACY_API */
#endif /* BTLE_APP_ADV_TYPE */

  /* set advertising and scan response data for discoverable mode */
#ifdef BTLE_APP_USE_LEGACY_API
  AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(beaconAdvDataDisc), (uint8_t *) beaconAdvDataDisc);
  AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(beaconScanDataDisc), (uint8_t *) beaconScanDataDisc);
#else /* BTLE_APP_USE_LEGACY_API */
#if (defined(BTLE_APP_ADV_TYPE) && (DM_ADV_SCANNABLE(BTLE_APP_ADV_TYPE)))
  AppExtAdvSetData(aAdvHandles[0], APP_SCAN_DATA_DISCOVERABLE, sizeof(beaconExtScanDataDisc), (uint8_t *) beaconExtScanDataDisc, HCI_EXT_ADV_DATA_LEN);
#else /* (defined(BTLE_APP_ADV_TYPE) && (DM_ADV_SCANNABLE(BTLE_APP_ADV_TYPE))) */
  AppExtAdvSetData(aAdvHandles[0], APP_ADV_DATA_DISCOVERABLE, sizeof(beaconExtAdvDataDisc), (uint8_t *) beaconExtAdvDataDisc, HCI_EXT_ADV_DATA_LEN);
#endif /* (defined(BTLE_APP_ADV_TYPE) && (DM_ADV_SCANNABLE(BTLE_APP_ADV_TYPE))) */
#endif /* BTLE_APP_USE_LEGACY_API */

  /* set periodic advertisement data. */
#ifdef BTLE_APP_ENABLE_PERIODIC
  AppPerAdvSetData(aAdvHandles[0], sizeof(beaconPerAdvData), (uint8_t *) beaconPerAdvData, HCI_EXT_ADV_DATA_LEN);
#endif /* BTLE_APP_ENABLE_PERIODIC */

  /* start advertising; automatically set connectable/discoverable mode and bondable mode */
#ifdef BTLE_APP_USE_LEGACY_API
  AppAdvStart(APP_MODE_DISCOVERABLE);
#else /* BTLE_APP_USE_LEGACY_API */
  AppExtAdvStart(nAdvHandles, &aAdvHandles[0], APP_MODE_DISCOVERABLE);
#endif /* BTLE_APP_USE_LEGACY_API */

  /* start periodic advertisements */
#ifdef BTLE_APP_ENABLE_PERIODIC
  AppPerAdvStart(aAdvHandles[0]);
#endif /* BTLE_APP_ENABLE_PERIODIC */
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
      beaconSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_ADV_START_IND:
      uiEvent = APP_UI_ADV_START;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_ADV_SET_START_IND:
      uiEvent = APP_UI_ADV_SET_START_IND;
      break;

    case DM_PER_ADV_SET_START_IND:
      uiEvent = APP_UI_PER_ADV_SET_START_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_ADV_STOP_IND:
      uiEvent = APP_UI_ADV_STOP;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
     case DM_ADV_SET_STOP_IND:
      uiEvent = APP_UI_ADV_SET_STOP_IND;
      break;

    case DM_PER_ADV_SET_STOP_IND:
      uiEvent = APP_UI_PER_ADV_SET_STOP_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_VENDOR_SPEC_IND:
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
#ifdef BTLE_APP_USE_LEGACY_API
  pAppAdvCfg = (appAdvCfg_t *) &beaconAdvCfg;
#else /* BTLE_APP_USE_LEGACY_API */
  pAppExtAdvCfg = (appExtAdvCfg_t *) &beaconExtAdvCfg;
#endif /* BTLE_APP_USE_LEGACY_API */

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

    if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END)
    {
      /* process advertising and connection-related messages */
      AppSlaveProcDmMsg((dmEvt_t *) pMsg);
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

  WsfBufDiagRegister(beaconWsfBufDiagnostics);

  /* Reset the device */
  DmDevReset();
}
