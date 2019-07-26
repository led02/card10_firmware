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
#include "datc/datc_api.h"
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
  uint16_t          hdlList[DM_CONN_MAX][APP_DB_HDL_LIST_LEN];   /*! Cached handle list */
  wsfHandlerId_t    handlerId;                      /*! WSF hander ID */
  bool_t            scanning;                       /*! TRUE if scanning */
  bool_t            autoConnect;                    /*! TRUE if auto-connecting */
  uint8_t           discState[DM_CONN_MAX];         /*! Service discovery state */
  uint8_t           hdlListLen;                     /*! Cached handle list length */
} datcCb;

/*! connection control block */
typedef struct {
  appDbHdl_t          dbHdl;                        /*! Device database record handle type */
  uint8_t             addrType;                     /*! Type of address of device to connect to */
  bdAddr_t            addr;                         /*! Address of device to connect to */
  bool_t              doConnect;                    /*! TRUE to issue connect on scan complete */
} datcConnInfo_t;

datcConnInfo_t datcConnInfo;

/*! test control block */
struct
{
  unsigned int counter;
  unsigned int connectCount;
  unsigned int phy;
  uint8_t connId;
  wsfTimer_t timer;
} testCb;

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

#ifdef BTLE_APP_USE_LEGACY_API
/*! configurable parameters for master */
static const appMasterCfg_t datcMasterCfg =
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
static const uint8_t datcExtMasterScanPhysCfg =
  HCI_SCAN_PHY_LE_1M_BIT |
  //HCI_SCAN_PHY_LE_2M_BIT |
  //HCI_SCAN_PHY_LE_CODED_BIT |
  0;
static const appExtMasterCfg_t datcExtMasterCfg =
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

/*! configurable parameters for security */
static const appSecCfg_t datcSecCfg =
{
  DM_AUTH_BOND_FLAG,                                         /*! Authentication and bonding flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK | DM_KEY_DIST_CSRK,      /*! Initiator key distribution flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK | DM_KEY_DIST_CSRK,      /*! Responder key distribution flags */
  FALSE,                                                     /*! TRUE if Out-of-band pairing data is present */
  FALSE                                                      /*! TRUE to initiate security upon connection */
};

/*! TRUE if Out-of-band pairing data is to be sent */
static const bool_t datcSendOobData = FALSE;

/*! SMP security parameter configuration */
static const smpCfg_t datcSmpCfg =
{
  3000,                                   /*! 'Repeated attempts' timeout in msec */
  SMP_IO_NO_IN_NO_OUT,                    /*! I/O Capability */
  7,                                      /*! Minimum encryption key length */
  16,                                     /*! Maximum encryption key length */
  3,                                      /*! Attempts to trigger 'repeated attempts' timeout */
  0,                                      /*! Device authentication requirements */
};

/*! Connection parameters */
static const hciConnSpec_t datcConnCfg =
{
  6,                                      /*! Minimum connection interval in 1.25ms units */
  6,                                      /*! Maximum connection interval in 1.25ms units */
  0,                                      /*! Connection latency */
  600,                                    /*! Supervision timeout in 10ms units */
  0,                                      /*! Minimum CE length, in 0.625 ms units */
  16                                      /*! Maximum CE length, in 0.625 ms units */
};

/*! Configurable parameters for service and characteristic discovery */
static const appDiscCfg_t datcDiscCfg =
{
  FALSE                                   /*! TRUE to wait for a secure connection before initiating discovery */
};

static const appCfg_t datcAppCfg =
{
  FALSE,                                  /*! TRUE to abort service discovery if service not found */
  TRUE                                    /*! TRUE to disconnect if ATT transaction times out */
};

/*! ATT configurable parameters (increase MTU) */
static const attCfg_t datcAttCfg =
{
  15,                               /* ATT server service discovery connection idle timeout in seconds */
  300,                              /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,            /* transaction timeout in seconds */
  4                                 /* number of queued prepare writes supported by server */
};

/*! local IRK */
static uint8_t localIrk[] =
{
  0xA6, 0xD9, 0xFF, 0x70, 0xD6, 0x1E, 0xF0, 0xA4, 0x46, 0x5F, 0x8D, 0x68, 0x19, 0xF3, 0xB4, 0x96
};

/**************************************************************************************************
  ATT Client Discovery Data
**************************************************************************************************/

/*! Discovery states:  enumeration of services to be discovered */
enum
{
  DATC_DISC_GATT_SVC,      /*! GATT service */
  DATC_DISC_GAP_SVC,       /*! GAP service */
  DATC_DISC_WP_SVC,        /*! ARM Ltd. proprietary service */
  DATC_DISC_SVC_MAX        /*! Discovery complete */
};

/*! the Client handle list, datcCb.hdlList[], is set as follows:
 *
 *  ------------------------------- <- DATC_DISC_GATT_START
 *  | GATT svc changed handle     |
 *  -------------------------------
 *  | GATT svc changed ccc handle |
 *  ------------------------------- <- DATC_DISC_GAP_START
 *  | GAP central addr res handle |
 *  -------------------------------
 *  | GAP RPA Only handle         |
 *  ------------------------------- <- DATC_DISC_WP_START
 *  | WP handles                  |
 *  | ...                         |
 *  -------------------------------
 */

/*! Start of each service's handles in the the handle list */
#define DATC_DISC_GATT_START       0
#define DATC_DISC_GAP_START        (DATC_DISC_GATT_START + GATT_HDL_LIST_LEN)
#define DATC_DISC_WP_START         (DATC_DISC_GAP_START + GAP_HDL_LIST_LEN)
#define DATC_DISC_HDL_LIST_LEN     (DATC_DISC_WP_START + WPC_P1_HDL_LIST_LEN)

/*! Pointers into handle list for each service's handles */
static uint16_t *pDatcGattHdlList[DM_CONN_MAX];
static uint16_t *pDatcGapHdlList[DM_CONN_MAX];
static uint16_t *pDatcWpHdlList[DM_CONN_MAX];

/* LESC OOB configuration */
static dmSecLescOobCfg_t *datcOobCfg;

/**************************************************************************************************
  ATT Client Configuration Data
**************************************************************************************************/

/*
 * Data for configuration after service discovery
 */

/* Default value for CCC indications */
const uint8_t datcCccIndVal[2] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_INDICATE)};

/* Default value for CCC notifications */
const uint8_t datcCccNtfVal[2] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_NOTIFY)};

/* List of characteristics to configure after service discovery */
static const attcDiscCfg_t datcDiscCfgList[] =
{
  /* Write:  GATT service changed ccc descriptor */
  {datcCccIndVal, sizeof(datcCccIndVal), (GATT_SC_CCC_HDL_IDX + DATC_DISC_GATT_START)},

  /* Write:  Proprietary data service changed ccc descriptor */
  {datcCccNtfVal, sizeof(datcCccNtfVal), (WPC_P1_NA_CCC_HDL_IDX + DATC_DISC_WP_START)},
};

/* Characteristic configuration list length */
#define DATC_DISC_CFG_LIST_LEN   (sizeof(datcDiscCfgList) / sizeof(attcDiscCfg_t))

/* sanity check:  make sure configuration list length is <= handle list length */
WSF_CT_ASSERT(DATC_DISC_CFG_LIST_LEN <= DATC_DISC_HDL_LIST_LEN);

/*************************************************************************************************/
/*!
 *  \brief  Application DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcDmCback(dmEvt_t *pDmEvt)
{
  dmEvt_t   *pMsg;
  uint16_t  len;
  uint16_t  reportLen;

  if (pDmEvt->hdr.event == DM_SEC_ECC_KEY_IND)
  {
    DmSecSetEccKey(&pDmEvt->eccMsg.data.key);

    // If the local device sends OOB data.
    if (datcSendOobData)
    {
      uint8_t oobLocalRandom[SMP_RAND_LEN];
      SecRand(oobLocalRandom, SMP_RAND_LEN);
      DmSecCalcOobReq(oobLocalRandom, pDmEvt->eccMsg.data.key.pubKey_x);
    }
  }
  else if (pDmEvt->hdr.event == DM_SEC_CALC_OOB_IND)
  {
    if (datcOobCfg == NULL)
    {
      datcOobCfg = WsfBufAlloc(sizeof(dmSecLescOobCfg_t));
    }

    if (datcOobCfg)
    {
      Calc128Cpy(datcOobCfg->localConfirm, pDmEvt->oobCalcInd.confirm);
      Calc128Cpy(datcOobCfg->localRandom, pDmEvt->oobCalcInd.random);
    }
  }
  else
  {
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
      WsfMsgSend(datcCb.handlerId, pMsg);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application  ATT callback.
 *
 *  \param  pEvt    ATT callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcAttCback(attEvt_t *pEvt)
{
  attEvt_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
  {
    memcpy(pMsg, pEvt, sizeof(attEvt_t));
    pMsg->pValue = (uint8_t *) (pMsg + 1);
    memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
    WsfMsgSend(datcCb.handlerId, pMsg);
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
static void datcScanStart(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    datcCb.scanning = TRUE;
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
static void datcScanStop(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    datcCb.scanning = FALSE;
    datcCb.autoConnect = FALSE;

    /* Open connection */
    if (datcConnInfo.doConnect)
    {
#ifdef BTLE_APP_USE_LEGACY_API
      testCb.connId = AppConnOpen(datcConnInfo.addrType, datcConnInfo.addr, datcConnInfo.dbHdl);
#else
      testCb.connId = AppExtConnOpen(datcExtMasterScanPhysCfg, datcConnInfo.addrType, datcConnInfo.addr, datcConnInfo.dbHdl);
#endif
      datcConnInfo.doConnect = FALSE;
    }
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
static void datcScanReport(dmEvt_t *pMsg)
{
  uint8_t *pData;
  appDbHdl_t dbHdl;
  bool_t  connect = FALSE;

  /* disregard if not scanning or autoconnecting */
  if (!datcCb.scanning || !datcCb.autoConnect)
  {
    return;
  }

  printf("datcScanReport() %x : %02x:%02x:%02x:%02x:%02x:%02x\n", pMsg->scanReport.addrType,
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
    printf(", data");
    int i;
    for (i = 0; i < pMsg->scanReport.len; i++) {
      printf(" %02x", pMsg->scanReport.pData[i]);
    }
    printf("\n");
  }

  /* if we already have a bond with this device then connect to it */
  if ((dbHdl = AppDbFindByAddr(pMsg->scanReport.addrType, pMsg->scanReport.addr)) != APP_DB_HDL_NONE)
  {
    /* if this is a directed advertisement where the initiator address is an RPA */
    if (DM_RAND_ADDR_RPA(pMsg->scanReport.directAddr, pMsg->scanReport.directAddrType))
    {
      /* resolve direct address to see if it's addressed to us */
      AppMasterResolveAddr(pMsg, dbHdl, APP_RESOLVE_DIRECT_RPA);
    }
    else
    {
      connect = TRUE;
    }
  }
  /* if the peer device uses an RPA */
  else if (DM_RAND_ADDR_RPA(pMsg->scanReport.addr, pMsg->scanReport.addrType))
  {
    /* resolve advertiser's RPA to see if we already have a bond with this device */
    AppMasterResolveAddr(pMsg, APP_DB_HDL_NONE, APP_RESOLVE_ADV_RPA);
  }
  /* find vendor-specific advertising data */
  else if ((pData = DmFindAdType(DM_ADV_TYPE_MANUFACTURER, pMsg->scanReport.len,
                                 pMsg->scanReport.pData)) != NULL)
  {
    /* check length and vendor ID */
    if (pData[DM_AD_LEN_IDX] >= 3 && BYTES_UINT16_CMP(&pData[DM_AD_DATA_IDX], HCI_ID_ARM))
    {
      connect = TRUE;
    }
  }

  if (connect)
  {
    /* stop scanning and connect */
    datcCb.autoConnect = FALSE;
    AppScanStop();

    /* Store peer information for connect on scan stop */
    datcConnInfo.addrType = DmHostAddrType(pMsg->scanReport.addrType);
    memcpy(datcConnInfo.addr, pMsg->scanReport.addr, sizeof(bdAddr_t));
    datcConnInfo.dbHdl = dbHdl;
    datcConnInfo.doConnect = TRUE;
  }
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
static void datcExtScanReport(dmEvt_t *pMsg)
{
  uint8_t *pData;
  appDbHdl_t dbHdl;
  bool_t  connect = FALSE;

  /* disregard if not scanning or autoconnecting */
  if (!datcCb.scanning || !datcCb.autoConnect)
  {
    return;
  }

  printf("datcExtScanReport() %x : %02x:%02x:%02x:%02x:%02x:%02x\n",
      pMsg->extScanReport.addrType,
      pMsg->extScanReport.addr[5],
      pMsg->extScanReport.addr[4],
      pMsg->extScanReport.addr[3],
      pMsg->extScanReport.addr[2],
      pMsg->extScanReport.addr[1],
      pMsg->extScanReport.addr[0]);

  printf("  directedAddr ( %x : %02x:%02x:%02x:%02x:%02x:%02x )\n",
      pMsg->extScanReport.directAddrType,
      pMsg->extScanReport.directAddr[5],
      pMsg->extScanReport.directAddr[4],
      pMsg->extScanReport.directAddr[3],
      pMsg->extScanReport.directAddr[2],
      pMsg->extScanReport.directAddr[1],
      pMsg->extScanReport.directAddr[0]);

  printf("  evtType %x,\n", pMsg->extScanReport.eventType);
  printf("  addrType %x,\n", pMsg->extScanReport.addrType);
  printf("  priPhy %u,\n", pMsg->extScanReport.priPhy);
  printf("  secPhy %u,\n", pMsg->extScanReport.secPhy);
  printf("  advSid 0x%02x,\n", pMsg->extScanReport.advSid);
  printf("  txPower %i,\n", pMsg->extScanReport.txPower);
  printf("  rssi %d,\n", pMsg->extScanReport.rssi);
  printf("  perAdvInter %i,\n", pMsg->extScanReport.perAdvInter);
  printf("  directAddrType %x,\n", pMsg->extScanReport.directAddrType);
  printf("  len %u,\n", pMsg->extScanReport.len);

  if (pMsg->extScanReport.pData[1] == DM_ADV_TYPE_LOCAL_NAME)
  {
    uint8_t name[32];
    memset(name, 0, sizeof(name));
    memcpy(name, &pMsg->extScanReport.pData[2], pMsg->extScanReport.pData[0]);
    printf(" | %s\n", name);
  }
  else {
    printf(", data");
    int i;
    for (i = 0; i < pMsg->extScanReport.len; i++) {
      printf(" %02x", pMsg->extScanReport.pData[i]);
    }
    printf("\n");
  }

  /* if we already have a bond with this device then connect to it */
  if ((dbHdl = AppDbFindByAddr(pMsg->extScanReport.addrType, pMsg->extScanReport.addr)) != APP_DB_HDL_NONE)
  {
    /* if this is a directed advertisement where the initiator address is an RPA */
    if (DM_RAND_ADDR_RPA(pMsg->extScanReport.directAddr, pMsg->extScanReport.directAddrType))
    {
      /* resolve direct address to see if it's addressed to us */
      AppMasterResolveAddr(pMsg, dbHdl, APP_RESOLVE_DIRECT_RPA);
    }
    else
    {
      connect = TRUE;
    }
  }
  /* if the peer device uses an RPA */
  else if (DM_RAND_ADDR_RPA(pMsg->extScanReport.addr, pMsg->extScanReport.addrType))
  {
    /* resolve advertiser's RPA to see if we already have a bond with this device */
    AppMasterResolveAddr(pMsg, APP_DB_HDL_NONE, APP_RESOLVE_ADV_RPA);
  }
  /* find vendor-specific advertising data */
  else if ((pData = DmFindAdType(DM_ADV_TYPE_MANUFACTURER, pMsg->extScanReport.len,
                                 pMsg->extScanReport.pData)) != NULL)
  {
    /* check length and vendor ID */
    if (pData[DM_AD_LEN_IDX] >= 3 && BYTES_UINT16_CMP(&pData[DM_AD_DATA_IDX], HCI_ID_ARM))
    {
      connect = TRUE;
    }
  }

  if (connect)
  {
    /* stop scanning and connect */
    datcCb.autoConnect = FALSE;
    AppExtScanStop();

    /* Store peer information for connect on scan stop */
    datcConnInfo.addrType = DmHostAddrType(pMsg->extScanReport.addrType);
    memcpy(datcConnInfo.addr, pMsg->extScanReport.addr, sizeof(bdAddr_t));
    datcConnInfo.dbHdl = dbHdl;
    datcConnInfo.doConnect = TRUE;
  }
}
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

/*************************************************************************************************/
/*!
 *  \brief  Perform UI actions on connection open.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcOpen(dmEvt_t *pMsg)
{
  testCb.connectCount = testCb.counter;
}

/*************************************************************************************************/
/*!
 *  \brief  Perform UI actions on connection close.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcClose(dmEvt_t *pMsg)
{
  testCb.connId = DM_CONN_ID_NONE;

  if (!datcCb.scanning)
  {
    datcCb.autoConnect = TRUE;
#ifdef BTLE_APP_USE_LEGACY_API
    AppScanStart(datcMasterCfg.discMode, datcMasterCfg.scanType, datcMasterCfg.scanDuration);
#else /* BTLE_APP_USE_LEGACY_API */
    AppExtScanStart(
        datcExtMasterScanPhysCfg,
        datcExtMasterCfg.discMode,
        datcExtMasterCfg.scanType,
        datcExtMasterCfg.scanDuration,
        datcExtMasterCfg.scanPeriod);
#endif /* BTLE_APP_USE_LEGACY_API */
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a received ATT notification.
 *
 *  \param  pMsg    Pointer to ATT callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcValueNtf(attEvt_t *pMsg)
{
  /* print the received data */
  printf("%s\n", pMsg->pValue);
}

/*************************************************************************************************/
/*!
 *  \brief  Set up procedures that need to be performed after device reset.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcSetup(dmEvt_t *pMsg)
{
  datcCb.scanning = FALSE;
  datcCb.autoConnect = FALSE;
  datcConnInfo.doConnect = FALSE;

  testCb.timer.handlerId = datcCb.handlerId;
  testCb.timer.msg.event = TEST_TIMER_IND;
  WsfTimerStartMs(&testCb.timer, 1000);
  testCb.connectCount = 0x80000000;

  /* Scanning will be limited to the EvKit dats peer */
  DmDevWhiteListAdd(DM_ADDR_PUBLIC, (bdAddr_t){0x02, 0x00, 0x44, 0x8B, 0x05, 0x00});
  DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_SCAN, HCI_FILT_WHITE_LIST);
  DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, HCI_FILT_WHITE_LIST);

  DmSetDefaultPhy(0, HCI_PHY_LE_1M_BIT | HCI_PHY_LE_2M_BIT, HCI_PHY_LE_1M_BIT | HCI_PHY_LE_2M_BIT);
  testCb.phy = HCI_PHY_LE_1M_BIT;

  DmConnSetConnSpec((hciConnSpec_t *) &datcConnCfg);
}

/*************************************************************************************************/
/*!
 *  \brief  GAP service discovery has completed.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcDiscGapCmpl(dmConnId_t connId)
{
  appDbHdl_t dbHdl;

  /* if RPA Only attribute found on peer device */
  if ((pDatcGapHdlList[connId-1][GAP_RPAO_HDL_IDX] != ATT_HANDLE_NONE) &&
      ((dbHdl = AppDbGetHdl(connId)) != APP_DB_HDL_NONE))
  {
    /* update DB */
    AppDbSetPeerRpao(dbHdl, TRUE);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Discovery callback.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    Service or configuration status.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcDiscCback(dmConnId_t connId, uint8_t status)
{
  switch(status)
  {
    case APP_DISC_INIT:
      /* set handle list when initialization requested */
      AppDiscSetHdlList(connId, datcCb.hdlListLen, datcCb.hdlList[connId-1]);
      break;

    case APP_DISC_SEC_REQUIRED:
      /* initiate security */
      AppMasterSecurityReq(connId);
      break;

    case APP_DISC_START:
      /* initialize discovery state */
      datcCb.discState[connId-1] = DATC_DISC_GATT_SVC;

      /* discover GATT service */
      GattDiscover(connId, pDatcGattHdlList[connId-1]);
      break;

    case APP_DISC_FAILED:
      if (pAppCfg->abortDisc)
      {
        /* if discovery failed for proprietary data service then disconnect */
        if (datcCb.discState[connId-1] == DATC_DISC_WP_SVC)
        {
          AppConnClose(connId);
          break;
        }
      }
      /* else fall through and continue discovery */

    case APP_DISC_CMPL:
      /* next discovery state */
      datcCb.discState[connId-1]++;

      if (datcCb.discState[connId-1] == DATC_DISC_GAP_SVC)
      {
        /* discover GAP service */
        GapDiscover(connId, pDatcGapHdlList[connId-1]);
      }
      else if (datcCb.discState[connId-1] == DATC_DISC_WP_SVC)
      {
        /* discover proprietary data service */
        WpcP1Discover(connId, pDatcWpHdlList[connId-1]);
      }
      else
      {
        /* discovery complete */
        AppDiscComplete(connId, APP_DISC_CMPL);

        /* GAP service discovery completed */
        datcDiscGapCmpl(connId);

        /* start configuration */
        AppDiscConfigure(connId, APP_DISC_CFG_START, DATC_DISC_CFG_LIST_LEN,
                         (attcDiscCfg_t *) datcDiscCfgList, DATC_DISC_HDL_LIST_LEN, datcCb.hdlList[connId-1]);
      }
      break;

    case APP_DISC_CFG_START:
        /* start configuration */
        AppDiscConfigure(connId, APP_DISC_CFG_START, DATC_DISC_CFG_LIST_LEN,
                         (attcDiscCfg_t *) datcDiscCfgList, DATC_DISC_HDL_LIST_LEN, datcCb.hdlList[connId-1]);
      break;

    case APP_DISC_CFG_CMPL:
      AppDiscComplete(connId, status);
      break;

    case APP_DISC_CFG_CONN_START:
      /* no connection setup configuration */
      break;

    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send example data.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcSendData(dmConnId_t connId)
{
  uint8_t str[] = "hello world";

  if (pDatcWpHdlList[WPC_P1_DAT_HDL_IDX] != ATT_HANDLE_NONE)
  {
    AttcWriteCmd(connId, pDatcWpHdlList[connId-1][WPC_P1_DAT_HDL_IDX], sizeof(str), str);
  }
}

/*************************************************************************************************/
static void testTimerHandler(void)
{
  testCb.counter++;

  if (testCb.counter == 2)
  {
    datcCb.autoConnect = TRUE;
#ifdef BTLE_APP_USE_LEGACY_API
    AppScanStart(datcMasterCfg.discMode, datcMasterCfg.scanType, datcMasterCfg.scanDuration);
#else /* BTLE_APP_USE_LEGACY_API */
    AppExtScanStart(
        datcExtMasterScanPhysCfg,
        datcExtMasterCfg.discMode,
        datcExtMasterCfg.scanType,
        datcExtMasterCfg.scanDuration,
        datcExtMasterCfg.scanPeriod);
#endif /* BTLE_APP_USE_LEGACY_API */
  }

  if ((((testCb.counter - testCb.connectCount) % 5) == 0) &&
      (testCb.connId != DM_CONN_ID_NONE) &&
      (datcCb.discState[testCb.connId-1] == DATC_DISC_SVC_MAX))
  {
    datcSendData(testCb.connId);
    DmConnReadRssi(testCb.connId);
  }

  if ((((testCb.counter - testCb.connectCount) % 10) == 0) &&
      (testCb.connId != DM_CONN_ID_NONE) &&
      (datcCb.discState[testCb.connId-1] == DATC_DISC_SVC_MAX))
  {
    if(testCb.phy == HCI_PHY_LE_1M_BIT) {
      /* Change from 1M to 2M PHY */
      testCb.phy = HCI_PHY_LE_2M_BIT;
    } else {
      /* Change from Coded to 1M PHY */
      testCb.phy = HCI_PHY_LE_1M_BIT;
    }

    DmSetPhy(testCb.connId, 0, testCb.phy, testCb.phy, 0);
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
static void datcProcMsg(dmEvt_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;

  switch(pMsg->hdr.event)
  {
    case ATTC_HANDLE_VALUE_NTF:
      datcValueNtf((attEvt_t *) pMsg);
      break;

    case ATTC_WRITE_CMD_RSP:
      break;

    case DM_RESET_CMPL_IND:
      DmSecGenerateEccKeyReq();
      datcSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_SCAN_START_IND:
      datcScanStart(pMsg);
      uiEvent = APP_UI_SCAN_START;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_EXT_SCAN_START_IND:
      datcScanStart(pMsg);
      uiEvent = APP_UI_EXT_SCAN_START_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_SCAN_STOP_IND:
      datcScanStop(pMsg);
      uiEvent = APP_UI_SCAN_STOP;
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_EXT_SCAN_STOP_IND:
      datcScanStop(pMsg);
      uiEvent = APP_UI_EXT_SCAN_STOP_IND;
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_SCAN_REPORT_IND:
      datcScanReport(pMsg);
      break;

#ifndef BTLE_APP_IGNORE_EXT_EVENTS
    case DM_EXT_SCAN_REPORT_IND:
      datcExtScanReport(pMsg);
      break;
#endif /* BTLE_APP_IGNORE_EXT_EVENTS */

    case DM_CONN_OPEN_IND:
      datcOpen(pMsg);
      uiEvent = APP_UI_CONN_OPEN;
      break;

    case DM_CONN_READ_RSSI_IND:
      printf("Connection 0x%x RSSI: %d\n", pMsg->readRssi.handle, pMsg->readRssi.rssi);
      break;

    case DM_CONN_CLOSE_IND:
      printf("Connection closed status 0x%x, reason 0x%x", pMsg->connClose.status, pMsg->connClose.reason);
      switch (pMsg->connClose.reason)
      {
        case HCI_ERR_CONN_TIMEOUT:      printf(" TIMEOUT\n");         break;
        case HCI_ERR_LOCAL_TERMINATED:  printf(" LOCAL TERM\n");      break;
        case HCI_ERR_REMOTE_TERMINATED: printf(" REMOTE TERM\n");     break;
        case HCI_ERR_CONN_FAIL:         printf(" FAIL ESTABLISH\n");  break;
        case HCI_ERR_MIC_FAILURE:       printf(" MIC FAILURE\n");     break;
      }
      datcClose(pMsg);
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


        /* TODO: Fill datsOobCfg peerConfirm and peerRandom with value passed out of band */

        if (datcOobCfg != NULL)
        {
          DmSecSetOob(connId, datcOobCfg);
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

    case DM_ADV_NEW_ADDR_IND:
      break;

    case TEST_TIMER_IND:
      testTimerHandler();
      break;

    case DM_VENDOR_SPEC_IND:
      break;

    case DM_PHY_UPDATE_IND:
      APP_TRACE_INFO2("DM_PHY_UPDATE_IND - RX: %d, TX: %d", pMsg->phyUpdate.rxPhy, pMsg->phyUpdate.txPhy);
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
void DatcHandlerInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("DatcHandlerInit");

  /* store handler ID */
  datcCb.handlerId = handlerId;

  /* set handle list length */
  datcCb.hdlListLen = DATC_DISC_HDL_LIST_LEN;

  /* Set configuration pointers */
#ifdef BTLE_APP_USE_LEGACY_API
  pAppMasterCfg = (appMasterCfg_t *) &datcMasterCfg;
#else /* BTLE_APP_USE_LEGACY_API */
  pAppExtMasterCfg = (appExtMasterCfg_t *) &datcExtMasterCfg;
#endif /* BTLE_APP_USE_LEGACY_API */
  pAppSecCfg = (appSecCfg_t *) &datcSecCfg;
  pAppDiscCfg = (appDiscCfg_t *) &datcDiscCfg;
  pAppCfg = (appCfg_t *)&datcAppCfg;
  pSmpCfg = (smpCfg_t *) &datcSmpCfg;
  pAttCfg = (attCfg_t *) &datcAttCfg;

  /* Initialize application framework */
  AppMasterInit();
  AppDiscInit();

  /* Set IRK for the local device */
  DmSecSetLocalIrk(localIrk);
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
void DatcHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (pMsg != NULL)
  {
    if (pMsg->event != TEST_TIMER_IND)
    {
      APP_TRACE_INFO2("Datc got evt 0x%x, param %u", pMsg->event, pMsg->param);
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
    datcProcMsg((dmEvt_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the pointers into the handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void datcInitSvcHdlList()
{
  uint8_t i;

  for (i=0; i<DM_CONN_MAX; i++)
  {
    /*! Pointers into handle list for each service's handles */
    pDatcGattHdlList[i] = &datcCb.hdlList[i][DATC_DISC_GATT_START];
    pDatcGapHdlList[i] = &datcCb.hdlList[i][DATC_DISC_GAP_START];
    pDatcWpHdlList[i] = &datcCb.hdlList[i][DATC_DISC_WP_START];
  }
}
/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DatcStart(void)
{
  /* Initialize handle pointers */
  datcInitSvcHdlList();

  /* Register for stack callbacks */
  DmRegister(datcDmCback);
  DmConnRegister(DM_CLIENT_ID_APP, datcDmCback);
  AttRegister(datcAttCback);

  /* Register for app framework discovery callbacks */
  AppDiscRegister(datcDiscCback);

  /* Initialize attribute server database */
  SvcCoreAddGroup();

  /* Reset the device */
  DmDevReset();
}
