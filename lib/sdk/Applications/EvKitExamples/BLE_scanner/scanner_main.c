/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Proprietary data transfer client sample application for Nordic-ble.
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
#include "wsf_assert.h"
#include "wsf_buf.h"
#include "hci_api.h"
#include "dm_api.h"
#include "gap/gap_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "app_cfg.h"
#include "app_api.h"
#include "app_db.h"
#include "app_ui.h"
#include "svc_core.h"
#include "svc_ch.h"
#include "gatt/gatt_api.h"
#include "wpc/wpc_api.h"
#include "scanner_api.h"
#include "util/calc128.h"

/**************************************************************************************************
Macros
**************************************************************************************************/

#define TEST_TIMER_IND   0x99

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! application control block */
struct
{
  wsfHandlerId_t    handlerId;                      /*! WSF hander ID */
  bool_t            scanning;                       /*! TRUE if scanning */
} scannerCb;

/*! test control block */
struct
{
  unsigned int counter;
  wsfTimer_t timer;
} testCb;

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

#ifdef BTLE_APP_USE_LEGACY_API
/*! configurable parameters for master */
static const appMasterCfg_t scannerMasterCfg =
{
  420,                                     /*! The scan interval, in 0.625 ms units */
  420,                                     /*! The scan window, in 0.625 ms units  */
  0,                                       /*! The scan duration in ms */
  DM_DISC_MODE_NONE,                       /*! The GAP discovery mode */
#ifdef BTLE_APP_USE_ACTIVE_SCANNING
  DM_SCAN_TYPE_ACTIVE
#else /* BTLE_APP_USE_ACTIVE_SCANNING */
  DM_SCAN_TYPE_PASSIVE
#endif /* BTLE_APP_USE_ACTIVE_SCANNING */
                                           /*!< The scan type (active or passive) */
};

#else /* BTLE_APP_USE_LEGACY_API */
/*! configurable parameters for extended master */
static const uint8_t scannerExtMasterScanPhysCfg =
  HCI_SCAN_PHY_LE_1M_BIT |
  //HCI_SCAN_PHY_LE_2M_BIT |
  //HCI_SCAN_PHY_LE_CODED_BIT |
  0;
static const appExtMasterCfg_t scannerExtMasterCfg =
{
  { 420, 420, 420 },                       /*! \brief The scan interval, in 0.625 ms units */
  { 420, 420, 420 },                       /*! \brief The scan window, in 0.625 ms units.   Must be less than or equal to scan interval. */
  0,                                       /*! \brief The scan duration in ms.  Set to zero or both duration and period to non-zero to scan until stopped. */
  0,                                       /*! \brief The scan period, in 1.28 sec units.  Set to zero to disable periodic scanning. */
  DM_DISC_MODE_NONE,                       /*! \brief The GAP discovery mode (general, limited, or none) */
#ifdef BTLE_APP_USE_ACTIVE_SCANNING
  {
    DM_SCAN_TYPE_ACTIVE,
    DM_SCAN_TYPE_ACTIVE,
    DM_SCAN_TYPE_ACTIVE
  }
#else /* BTLE_APP_USE_ACTIVE_SCANNING */
  {
    DM_SCAN_TYPE_PASSIVE,
    DM_SCAN_TYPE_PASSIVE,
    DM_SCAN_TYPE_PASSIVE
  }
#endif /* BTLE_APP_USE_ACTIVE_SCANNING */
                                           /*!< \brief The scan type (active or passive) */
};

#endif /* BTLE_APP_USE_LEGACY_API */

/**************************************************************************************************
  ATT Client Discovery Data
**************************************************************************************************/

/*! Discovery states:  enumeration of services to be discovered */
enum
{
  SCANNER_DISC_GATT_SVC,      /*! GATT service */
  SCANNER_DISC_GAP_SVC,       /*! GAP service */
  SCANNER_DISC_WP_SVC,        /*! ARM Ltd. proprietary service */
  SCANNER_DISC_SVC_MAX        /*! Discovery complete */
};

/**************************************************************************************************
  ATT Client Configuration Data
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Application DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void scannerDmCback(dmEvt_t *pDmEvt)
{
  dmEvt_t   *pMsg;
  uint16_t  len;
  uint16_t  reportLen;

  len = DmSizeOfEvt(pDmEvt);

  if (pDmEvt->hdr.event == DM_SCAN_REPORT_IND)
  {
    reportLen = pDmEvt->scanReport.len;
  }
  else
  {
    reportLen = 0;
  }

  if ((pMsg = WsfMsgAlloc(len + reportLen)) != NULL)
  {
    memcpy(pMsg, pDmEvt, len);
    if (pDmEvt->hdr.event == DM_SCAN_REPORT_IND)
    {
      pMsg->scanReport.pData = (uint8_t *) ((uint8_t *) pMsg + len);
      memcpy(pMsg->scanReport.pData, pDmEvt->scanReport.pData, reportLen);
    }
    WsfMsgSend(scannerCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform actions on scan start.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void scannerScanStart(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    scannerCb.scanning = TRUE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform actions on scan stop.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void scannerScanStop(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    scannerCb.scanning = FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a scan report.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void scannerScanReport(dmEvt_t *pMsg)
{
  /* disregard if not scanning */
  if (!scannerCb.scanning)
  {
    return;
  }

  printf("scannerScanReport() %x : %02x:%02x:%02x:%02x:%02x:%02x",pMsg->scanReport.addrType,
                                                                  pMsg->scanReport.addr[5],
                                                                  pMsg->scanReport.addr[4],
                                                                  pMsg->scanReport.addr[3],
                                                                  pMsg->scanReport.addr[2],
                                                                  pMsg->scanReport.addr[1],
                                                                  pMsg->scanReport.addr[0]);

  printf("  len %u, rssi %d, evtType %x, addrType %x\n", pMsg->scanReport.len, pMsg->scanReport.rssi, pMsg->scanReport.eventType, pMsg->scanReport.addrType);
}

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
/*************************************************************************************************/
/*!
 *  \brief  Handle a scan report.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void scannerExtScanReport(dmEvt_t *pMsg)
{
  /* disregard if not scanning */
  if (!scannerCb.scanning)
  {
    return;
  }

  printf("scannerExtScanReport() %x : %02x:%02x:%02x:%02x:%02x:%02x", pMsg->extScanReport.addrType,
                                                                      pMsg->extScanReport.addr[5],
                                                                      pMsg->extScanReport.addr[4],
                                                                      pMsg->extScanReport.addr[3],
                                                                      pMsg->extScanReport.addr[2],
                                                                      pMsg->extScanReport.addr[1],
                                                                      pMsg->extScanReport.addr[0]);

  printf("  len %u, rssi %d, evtType %x, addrType %x\n", pMsg->extScanReport.len, pMsg->extScanReport.rssi, pMsg->extScanReport.eventType, pMsg->extScanReport.addrType);
}
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

/*************************************************************************************************/
/*!
 *  \brief  Set up procedures that need to be performed after device reset.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void scannerSetup(dmEvt_t *pMsg)
{
  scannerCb.scanning = FALSE;

  testCb.timer.handlerId = scannerCb.handlerId;
  testCb.timer.msg.event = TEST_TIMER_IND;
  WsfTimerStartMs(&testCb.timer, 1000);

  /* If this is defined to one, scanning will be limited to the peer */
#if 0
  DmDevWhiteListAdd(DM_ADDR_PUBLIC, (bdAddr_t){0x02, 0x00, 0x44, 0x8B, 0x05, 0x00});
  DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_SCAN, HCI_FILT_WHITE_LIST);
  DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, HCI_FILT_WHITE_LIST);
#endif
}



/*************************************************************************************************/
static void testTimerHandler(void)
{
  testCb.counter++;

  if (testCb.counter == 2)
  {
#ifdef BTLE_APP_USE_LEGACY_API
    AppScanStart(scannerMasterCfg.discMode, scannerMasterCfg.scanType, scannerMasterCfg.scanDuration);
#else /* BTLE_APP_USE_LEGACY_API */
    AppExtScanStart(
        scannerExtMasterScanPhysCfg,
        scannerExtMasterCfg.discMode,
        scannerExtMasterCfg.scanType,
        scannerExtMasterCfg.scanDuration,
        scannerExtMasterCfg.scanPeriod);
#endif /* BTLE_APP_USE_LEGACY_API */
  }

  WsfTimerStartMs(&testCb.timer, 1000);
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
static void scannerProcMsg(dmEvt_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;

  switch(pMsg->hdr.event)
  {
    case DM_RESET_CMPL_IND:
      DmSecGenerateEccKeyReq();
      scannerSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_SCAN_START_IND:
      scannerScanStart(pMsg);
      uiEvent = APP_UI_SCAN_START;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_EXT_SCAN_START_IND:
      scannerScanStart(pMsg);
      uiEvent = APP_UI_EXT_SCAN_START_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_SCAN_STOP_IND:
      scannerScanStop(pMsg);
      uiEvent = APP_UI_SCAN_STOP;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_EXT_SCAN_STOP_IND:
      scannerScanStop(pMsg);
      uiEvent = APP_UI_EXT_SCAN_STOP_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_SCAN_REPORT_IND:
      scannerScanReport(pMsg);
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_EXT_SCAN_REPORT_IND:
      scannerExtScanReport(pMsg);
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case TEST_TIMER_IND:
      testTimerHandler();
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
void ScannerHandlerInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("ScannerHandlerInit");

  /* store handler ID */
  scannerCb.handlerId = handlerId;

  /* Set configuration pointers */
#ifdef BTLE_APP_USE_LEGACY_API
  pAppMasterCfg = (appMasterCfg_t *) &scannerMasterCfg;
#else /* BTLE_APP_USE_LEGACY_API */
  pAppExtMasterCfg = (appExtMasterCfg_t *) &scannerExtMasterCfg;
#endif /* BTLE_APP_USE_LEGACY_API */

  /* Initialize application framework */
  AppMasterInit();
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
void ScannerHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (pMsg != NULL)
  {
    if (pMsg->event != TEST_TIMER_IND)
    {
      APP_TRACE_INFO2("Scanner got evt 0x%x, param %u", pMsg->event, pMsg->param);
    }

    /* process ATT messages */
    if (pMsg->event <= ATT_CBACK_END)
    {
      /* process discovery-related ATT messages */
      AppDiscProcAttMsg((attEvt_t *) pMsg);
    }
    /* process DM messages */
    else if (pMsg->event <= DM_CBACK_END)
    {
      /* process advertising and connection-related messages */
      AppMasterProcDmMsg((dmEvt_t *) pMsg);

      /* process security-related messages */
      AppMasterSecProcDmMsg((dmEvt_t *) pMsg);

      /* process discovery-related messages */
      AppDiscProcDmMsg((dmEvt_t *) pMsg);
    }

    /* perform profile and user interface-related operations */
    scannerProcMsg((dmEvt_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ScannerStart(void)
{

  /* Register for stack callbacks */
  DmRegister(scannerDmCback);

  /* Reset the device */
  DmDevReset();
}
