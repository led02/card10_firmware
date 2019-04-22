/*************************************************************************************************/
/*!
 *  \file   
 *
 *  \brief  
 *
 *          $Date: 2018-11-15 02:01:14 +0000 (Thu, 15 Nov 2018) $
 *          $Revision: 39269 $
 *
 *  Copyright (c) 2012 Wicentric, Inc., all rights reserved.
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

#include <stdio.h>
#include <string.h>
#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "hci_api.h"
#include "dm_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "app_cfg.h"
#include "app_api.h"
#include "app_db.h"
#include "app_ui.h"
#include "svc_ch.h"
#include "gatt/gatt_api.h"
#include "wpc/wpc_api.h"
#include "scanner_api.h"

#include "app_config.h"

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! application control block */
struct
{
  wsfHandlerId_t    handlerId;                      /*! WSF hander ID */
  bool_t            scanning;                       /*! TRUE if scanning */
} scannerCb;

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

/*! configurable parameters for master */

#ifdef USE_EXTENDED_ADV
static const appExtMasterCfg_t scannerExtMasterCfg =
{
  {160},                                   /*! The scan interval, in 0.625 ms units */
  {160},                                   /*! The scan window, in 0.625 ms units  */
  0,                                       /*! The scan duration in ms */
  0,                                       /*! The scan period in ms */
  DM_DISC_MODE_NONE,                       /*! The GAP discovery mode */
  {
#if 0
    DM_SCAN_TYPE_ACTIVE                    /*! The scan type (active or passive) */
#else
    DM_SCAN_TYPE_PASSIVE                   /*! The scan type (active or passive) */
#endif
  }
};

#else /* USE_EXTENDED_ADV */
static const appMasterCfg_t scannerMasterCfg =
{
  160,                                     /*! The scan interval, in 0.625 ms units */
  160,                                     /*! The scan window, in 0.625 ms units  */
  0,                                       /*! The scan duration in ms */
  DM_DISC_MODE_NONE,                       /*! The GAP discovery mode */
#if 1
  DM_SCAN_TYPE_ACTIVE                      /*! The scan type (active or passive) */
#else
  DM_SCAN_TYPE_PASSIVE                     /*! The scan type (active or passive) */
#endif
};

#endif /* USE_EXTENDED_ADV */

/*************************************************************************************************/
/*!
 *  \fn     scannerDmCback
 *        
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

  reportLen = 0;
  if (pDmEvt->hdr.event == DM_SCAN_REPORT_IND)
  {
    reportLen = pDmEvt->scanReport.len;
  }
#ifdef USE_EXTENDED_ADV
  else
  {
    reportLen = 0;
  }
#else /* USE_EXTENDED_ADV */
  else if (pDmEvt->hdr.event == DM_EXT_SCAN_REPORT_IND)
  {
    if (pDmEvt->extScanReport.pData)
      reportLen = pDmEvt->extScanReport.len;
  }
#endif /* USE_EXTENDED_ADV */

  if ((pMsg = WsfMsgAlloc(len + reportLen)) != NULL)
  {
    memcpy(pMsg, pDmEvt, len);
    if (pDmEvt->hdr.event == DM_SCAN_REPORT_IND)
    {
      pMsg->scanReport.pData = (uint8_t *) ((uint8_t *) pMsg + len);
      memcpy(pMsg->scanReport.pData, pDmEvt->scanReport.pData, reportLen);
    }
#ifdef USE_EXTENDED_ADV
    else if (pDmEvt->hdr.event == DM_EXT_SCAN_REPORT_IND)
    {
      pMsg->extScanReport.pData = (uint8_t *) ((uint8_t *) pMsg + len);
      memcpy(pMsg->extScanReport.pData, pDmEvt->extScanReport.pData, reportLen);
    }
#endif /* USE_EXTENDED_ADV */
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
  scannerCb.scanning = TRUE;
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
  scannerCb.scanning = FALSE;
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

  printf("scannerScanReport() %x : %02x:%02x:%02x:%02x:%02x:%02x\n", pMsg->scanReport.addrType,
                                                                  pMsg->scanReport.addr[5],
                                                                  pMsg->scanReport.addr[4],
                                                                  pMsg->scanReport.addr[3],
                                                                  pMsg->scanReport.addr[2],
                                                                  pMsg->scanReport.addr[1],
                                                                  pMsg->scanReport.addr[0]);

  printf("  len %u, rssi %d, evtType %x, addrType %x", pMsg->scanReport.len, pMsg->scanReport.rssi, pMsg->scanReport.eventType, pMsg->scanReport.addrType);

  if (pMsg->scanReport.pData[1] == DM_ADV_TYPE_LOCAL_NAME)
  {
    uint8_t name[32];
    memset(name, 0, sizeof(name));
    memcpy(name, &pMsg->scanReport.pData[2], pMsg->scanReport.pData[0]);
    printf(" | %s\n", name);
  }
  else {
    printf(", data %2u:", pMsg->scanReport.len);
    int i;
    for (i = 0; i < pMsg->scanReport.len; i++) {
      printf(" %02x", pMsg->scanReport.pData[i]);
    }
    printf("\n");
  }
}

#ifdef USE_EXTENDED_ADV
/*************************************************************************************************/
/*!
 *  \brief  Handle an extended scan report.
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

  printf("scannerExtScanReport() %x : %02x:%02x:%02x:%02x:%02x:%02x\n", pMsg->extScanReport.addrType,
                                                                  pMsg->extScanReport.addr[5],
                                                                  pMsg->extScanReport.addr[4],
                                                                  pMsg->extScanReport.addr[3],
                                                                  pMsg->extScanReport.addr[2],
                                                                  pMsg->extScanReport.addr[1],
                                                                  pMsg->extScanReport.addr[0]);

  printf("  len %u, rssi %d, evtType %x, addrType %x", pMsg->extScanReport.len, pMsg->extScanReport.rssi, pMsg->extScanReport.eventType, pMsg->extScanReport.addrType);

  if (pMsg->extScanReport.pData[1] == DM_ADV_TYPE_LOCAL_NAME)
  {
    uint8_t name[32];
    memset(name, 0, sizeof(name));
    memcpy(name, &pMsg->extScanReport.pData[2], pMsg->extScanReport.pData[0]);
    printf(" | %s\n", name);
  }
  else {
    printf(", data %2u:", pMsg->extScanReport.len);
    int i;
    for (i = 0; i < pMsg->extScanReport.len; i++) {
      printf(" %02x", pMsg->extScanReport.pData[i]);
    }
    printf("\n");
  }
}
#endif /* USE_EXTENDED_ADV */

/*************************************************************************************************/
/*!
 *  \brief  Set up procedures that need to be performed after device reset.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef USE_EXTENDED_ADV
static void scannerSetup(dmEvt_t *pMsg)
{
  scannerCb.scanning = FALSE;
 
  AppExtScanStart(
      0x01, // Support PHY 1M
      scannerExtMasterCfg.discMode,
      scannerExtMasterCfg.scanType,
      scannerExtMasterCfg.scanDuration,
      scannerExtMasterCfg.scanPeriod);
}
#else /* USE_EXTENDED_ADV */
static void scannerSetup(dmEvt_t *pMsg)
{
#if 0
  /* If this is defined to one, scanning will be limited to the EvKit dats peer */
  DmDevWhiteListAdd(DM_ADDR_PUBLIC, (bdAddr_t){0x02, 0x00, 0x44, 0x8B, 0x05, 0x00});
  DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_SCAN, HCI_FILT_WHITE_LIST);
#endif

  scannerCb.scanning = FALSE;

  AppScanStart(scannerMasterCfg.discMode, scannerMasterCfg.scanType, scannerMasterCfg.scanDuration);

  /* 0x3 : 1M and 2M PHYs */
  DmSetDefaultPhy(0, 0x3, 0x3);
}
#endif /* USE_EXTENDED_ADV*/

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
      scannerSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_SCAN_START_IND:
      scannerScanStart(pMsg);
      uiEvent = APP_UI_SCAN_START;
      break;
         
    case DM_SCAN_STOP_IND:
      scannerScanStop(pMsg);
      uiEvent = APP_UI_SCAN_STOP;
      break;

#ifdef USE_EXTENDED_ADV
    case DM_EXT_SCAN_START_IND:
      scannerScanStart(pMsg);
      uiEvent = APP_UI_SCAN_START;
      break;
         
    case DM_EXT_SCAN_STOP_IND:
      scannerScanStop(pMsg);
      uiEvent = APP_UI_SCAN_STOP;
      break;
#endif /* USE_EXTENDED_ADV */

    case DM_SCAN_REPORT_IND:
      scannerScanReport(pMsg);
      break;

#ifdef USE_EXTENDED_ADV
    case DM_EXT_SCAN_REPORT_IND:
      scannerExtScanReport(pMsg);
      break;
#endif /* USE_EXTENDED_ADV */

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
void ScannerHandlerInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("ScannerHandlerInit");
  
  /* store handler ID */
  scannerCb.handlerId = handlerId;
  
  /* Set configuration pointers */
#ifdef USE_EXTENDED_ADV
  pAppExtMasterCfg = (appExtMasterCfg_t *) &scannerExtMasterCfg;
#else /* USE_EXTENDED_ADV */
  pAppMasterCfg = (appMasterCfg_t *) &scannerMasterCfg;
#endif /* USE_EXTENDED_ADV */
  
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
    APP_TRACE_INFO1("Scanner got evt 0x%x", pMsg->event);

    /* process DM messages */
    if ((pMsg->event >= DM_CBACK_START) && (pMsg->event <= DM_CBACK_END))
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
  DmConnRegister(DM_CLIENT_ID_APP, scannerDmCback);

  /* Reset the device */
  DmDevReset();  
}
