/* card10:
 * Copied from lib/sdk/Libraries/BTLE/stack/ble-profiles/sources/apps/fit/fit_main.c
 *
 * Also have a look at lib/sdk/Applications/EvKitExamples/BLE_fit/fit_main.c which has some changes
 * to this file regarding handling of OOB paring data
 *
 * This file contains some application logic taken from the "fit" example.
 *
 * Things have been renamed:
 * fit -> ble
 * Fit -> Ble
 * FIT -> BLE
 */
/* clang-format off */
/* clang-formet turned off for easier diffing against orginal file */
#include <stdio.h>
#include <string.h>
#include "wsf_types.h"
#include "util/bstream.h"
#include "fs_util.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "hci_api.h"
#include "dm_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "app_api.h"
#include "app_db.h"
#include "app_ui.h"
#include "app_hw.h"
#include "svc_ch.h"
#include "svc_core.h"
#include "svc_hrs.h"
#include "svc_dis.h"
#include "svc_batt.h"
#include "svc_rscs.h"
#include "bas/bas_api.h"
#include "hrps/hrps_api.h"
#include "rscp/rscp_api.h"

#include "modules/log.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! WSF message event starting value */
#define BLE_MSG_START               0xA0

/*! WSF message event enumeration */
enum
{
  BLE_BATT_TIMER_IND = BLE_MSG_START,                     /*! Battery measurement timer expired */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Application message type */
typedef union
{
  wsfMsgHdr_t     hdr;
  dmEvt_t         dm;
  attsCccEvt_t    ccc;
  attEvt_t        att;
} bleMsg_t;

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

/*! configurable parameters for advertising */
static const appAdvCfg_t bleAdvCfg =
{
  {0,             0,              0},                  /*! Advertising durations in ms */
  {500/0.625,     4000/0.625,     0}                   /*! Advertising intervals in 0.625 ms units */
};

/*! configurable parameters for slave */
static const appSlaveCfg_t bleSlaveCfg =
{
  1,                                      /*! Maximum connections */
};

/*! configurable parameters for security */
static const appSecCfg_t bleSecCfg =
{
  DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG,    /*! Authentication and bonding flags */
  0,                                      /*! Initiator key distribution flags */
  DM_KEY_DIST_LTK,                        /*! Responder key distribution flags */
  FALSE,                                  /*! TRUE if Out-of-band pairing data is present */
  TRUE                                    /*! TRUE to initiate security upon connection */
};

/*! configurable parameters for connection parameter update */
static const appUpdateCfg_t bleUpdateCfg =
{
  6000,                                   /*! Connection idle period in ms before attempting
                                              connection parameter update; set to zero to disable */
  800/1.25,                               /*! Minimum connection interval in 1.25ms units */
  1000/1.25,                              /*! Maximum connection interval in 1.25ms units */
  0,                                      /*! Connection latency */
  9000/10,                                /*! Supervision timeout in 10ms units */
  5                                       /*! Number of update attempts before giving up */
};

/*! battery measurement configuration */
static const basCfg_t bleBasCfg =
{
  30,       /*! Battery measurement timer expiration period in seconds */
  1,        /*! Perform battery measurement after this many timer periods */
  100       /*! Send battery level notification to peer when below this level. */
};

/*! SMP security parameter configuration */
static const smpCfg_t bleSmpCfg =
{
  3000,                                   /*! 'Repeated attempts' timeout in msec */
  SMP_IO_DISP_YES_NO,                     /*! I/O Capability */
  7,                                      /*! Minimum encryption key length */
  16,                                     /*! Maximum encryption key length */
  3,                                      /*! Attempts to trigger 'repeated attempts' timeout */
  DM_AUTH_MITM_FLAG,                      /*! Device authentication requirements */
};

/* Configuration structure */
static const attCfg_t bleAttCfg =
{
  15,                                  /* ATT server service discovery connection idle timeout in seconds */
  241,                                 /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,               /* transaction timeout in seconds */
  1                                    /* number of queued prepare writes supported by server */
};

/**************************************************************************************************
  Advertising Data
**************************************************************************************************/

/*! advertising data, discoverable mode */
static const uint8_t bleAdvDataDisc[] =
{
  /*! flags */
  2,                                      /*! length */
  DM_ADV_TYPE_FLAGS,                      /*! AD type */
  DM_FLAG_LE_GENERAL_DISC |               /*! flags */
  DM_FLAG_LE_BREDR_NOT_SUP,

  /*! tx power */
  2,                                      /*! length */
  DM_ADV_TYPE_TX_POWER,                   /*! AD type */
  0,                                      /*! tx power */

  /*! service UUID list */
  5,                                      /*! length */
  DM_ADV_TYPE_16_UUID,                    /*! AD type */
  UINT16_TO_BYTES(ATT_UUID_DEVICE_INFO_SERVICE),
  UINT16_TO_BYTES(ATT_UUID_BATTERY_SERVICE)
};

/*! scan data, discoverable mode */
uint8_t bleScanDataDisc[] =
{
  /*! device name */
  14,                                      /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'c','a','r','d','1','0','-','0','0','0','0','0','0'
};

/**************************************************************************************************
  Client Characteristic Configuration Descriptors
**************************************************************************************************/

/*! enumeration of client characteristic configuration descriptors */
enum
{
  BLE_GATT_SC_CCC_IDX,                    /*! GATT service, service changed characteristic */
  BLE_BATT_LVL_CCC_IDX,                   /*! Battery service, battery level characteristic */
  BLE_NUM_CCC_IDX
};

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t bleCccSet[BLE_NUM_CCC_IDX] =
{
  /* cccd handle          value range               security level */
  {GATT_SC_CH_CCC_HDL,    ATT_CLIENT_CFG_INDICATE,  DM_SEC_LEVEL_NONE},   /* BLE_GATT_SC_CCC_IDX */
  {BATT_LVL_CH_CCC_HDL,   ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE},   /* BLE_BATT_LVL_CCC_IDX */
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! WSF handler ID */
wsfHandlerId_t bleHandlerId;


static void BleHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
/*************************************************************************************************/
/*!
 *  \brief  Application DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void bleDmCback(dmEvt_t *pDmEvt)
{
  dmEvt_t *pMsg;
  uint16_t len;

  len = DmSizeOfEvt(pDmEvt);

  if ((pMsg = WsfMsgAlloc(len)) != NULL)
  {
    memcpy(pMsg, pDmEvt, len);
    WsfMsgSend(bleHandlerId, pMsg);
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
static void bleAttCback(attEvt_t *pEvt)
{
  attEvt_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
  {
    memcpy(pMsg, pEvt, sizeof(attEvt_t));
    pMsg->pValue = (uint8_t *) (pMsg + 1);
    memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
    WsfMsgSend(bleHandlerId, pMsg);
  }
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
static void bleCccCback(attsCccEvt_t *pEvt)
{
  attsCccEvt_t  *pMsg;
  appDbHdl_t    dbHdl;

  /* if CCC not set from initialization and there's a device record */
  if ((pEvt->handle != ATT_HANDLE_NONE) &&
      ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE))
  {
    /* store value in device database */
    AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
  }

  if ((pMsg = WsfMsgAlloc(sizeof(attsCccEvt_t))) != NULL)
  {
    memcpy(pMsg, pEvt, sizeof(attsCccEvt_t));
    WsfMsgSend(bleHandlerId, pMsg);
  }
}



/*************************************************************************************************/
/*!
 *  \brief  Process CCC state change.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void bleProcCccState(bleMsg_t *pMsg)
{
  APP_TRACE_INFO3("ccc state ind value:%d handle:%d idx:%d", pMsg->ccc.value, pMsg->ccc.handle, pMsg->ccc.idx);

  /* handle battery level CCC */
  if (pMsg->ccc.idx == BLE_BATT_LVL_CCC_IDX)
  {
    if (pMsg->ccc.value == ATT_CLIENT_CFG_NOTIFY)
    {
      BasMeasBattStart((dmConnId_t) pMsg->ccc.hdr.param, BLE_BATT_TIMER_IND, BLE_BATT_LVL_CCC_IDX);
    }
    else
    {
      BasMeasBattStop((dmConnId_t) pMsg->ccc.hdr.param);
    }
    return;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform UI actions on connection close.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void bleClose(bleMsg_t *pMsg)
{
  /* stop battery measurement */
  BasMeasBattStop((dmConnId_t) pMsg->hdr.param);
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
static void bleSetup(bleMsg_t *pMsg)
{
  char buf[32];
  char a, b, c, d, e, f, K;

  if (fs_read_text_file("mac.txt", buf, sizeof(buf)))
  {
    if (sscanf(buf, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c", &K,&K,&K,&K,&K,&K, &a, &b, &c, &d, &e, &f) == 12)
    {
      bleScanDataDisc[9]  = a;
      bleScanDataDisc[10] = b;
      bleScanDataDisc[11] = c;
      bleScanDataDisc[12] = d;
      bleScanDataDisc[13] = e;
      bleScanDataDisc[14] = f;
    }
  }
  
  /* set advertising and scan response data for discoverable mode */
  AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(bleAdvDataDisc), (uint8_t *) bleAdvDataDisc);
  AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(bleScanDataDisc), (uint8_t *) bleScanDataDisc);

  /* set advertising and scan response data for connectable mode */
  AppAdvSetData(APP_ADV_DATA_CONNECTABLE, 0, NULL);
  AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, 0, NULL);

#if 0
  /* TODO: card10: until we have an BLE dialog, be discoverable and bondable always */
  /* start advertising; automatically set connectable/discoverable mode and bondable mode */
  AppAdvStart(APP_MODE_AUTO_INIT);
#else
  /* enter discoverable and bondable mode mode by default */
  AppSetBondable(TRUE);
  AppAdvStart(APP_MODE_DISCOVERABLE);
#endif
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
static void bleProcMsg(bleMsg_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;
  hciLeConnCmplEvt_t *connOpen;

  switch(pMsg->hdr.event)
  {
    case BLE_BATT_TIMER_IND:
      BasProcMsg(&pMsg->hdr);
      break;

    case ATTS_HANDLE_VALUE_CNF:
      BasProcMsg(&pMsg->hdr);
      break;

    case ATTS_CCC_STATE_IND:
      bleProcCccState(pMsg);
      break;

    case DM_RESET_CMPL_IND:
      DmSecGenerateEccKeyReq();
      bleSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_ADV_START_IND:
      LOG_INFO("ble", "Advertisement started");
      uiEvent = APP_UI_ADV_START;
      break;

    case DM_ADV_STOP_IND:
      LOG_INFO("ble", "Advertisement stopped");
      uiEvent = APP_UI_ADV_STOP;
      break;

    case DM_CONN_OPEN_IND:
      connOpen = &pMsg->dm.connOpen;
      LOG_INFO("ble", "connection from %02X:%02X:%02X:%02X:%02X:%02X opened",
               connOpen->peerAddr[0], connOpen->peerAddr[1],
               connOpen->peerAddr[2], connOpen->peerAddr[3],
               connOpen->peerAddr[4], connOpen->peerAddr[5]);
      BasProcMsg(&pMsg->hdr);
      uiEvent = APP_UI_CONN_OPEN;
      break;

    case DM_CONN_CLOSE_IND:
      switch (pMsg->dm.connClose.reason)
      {
        case HCI_ERR_CONN_TIMEOUT:
          LOG_INFO("ble", "Connection closed (0x%02X), Connection timeout",
                   pMsg->dm.connClose.reason);
          break;
        case HCI_ERR_LOCAL_TERMINATED:
          LOG_INFO("ble", "Connection closed (0x%02X), Connection terminated by local host",
                   pMsg->dm.connClose.reason);
          break;
        case HCI_ERR_REMOTE_TERMINATED:
          LOG_INFO("ble", "Connection closed (0x%02X), Remote user terminated connection",
                   pMsg->dm.connClose.reason);
          break;
        case HCI_ERR_CONN_FAIL:
          LOG_INFO("ble", "Connection closed (0x%02X), Connection failed to be established",
                   pMsg->dm.connClose.reason);
          break;
        case HCI_ERR_MIC_FAILURE:
          LOG_INFO("ble", "Connection closed (0x%02X), Connection terminated due to MIC failure",
                   pMsg->dm.connClose.reason);
          break;
        default:
          LOG_INFO("ble", "Connection closed (0x%02X)",
                   pMsg->dm.connClose.reason);
          break;
      }
      bleClose(pMsg);
      uiEvent = APP_UI_CONN_CLOSE;
      break;

    case DM_SEC_PAIR_CMPL_IND:
      LOG_INFO("ble", "Secure pairing successful, auth: 0x%02X",
               pMsg->dm.pairCmpl.auth);
      uiEvent = APP_UI_SEC_PAIR_CMPL;
      break;

    case DM_SEC_PAIR_FAIL_IND:
      switch (pMsg->hdr.status) {
        case SMP_ERR_TIMEOUT:
          LOG_INFO("ble", "Secure pairing failed (0x%02X), Transaction timeout",
                   pMsg->hdr.status);
          break;
        case SMP_ERR_ATTEMPTS:
          LOG_INFO("ble", "Secure pairing failed (0x%02X), Repeated attempts",
                   pMsg->hdr.status);
          break;
        default:
          LOG_INFO("ble", "Secure pairing failed (0x%02X)",
                   pMsg->hdr.status);
          break;
      }
      uiEvent = APP_UI_SEC_PAIR_FAIL;
      break;

    case DM_SEC_ENCRYPT_IND:
      LOG_INFO("ble", "Encrypted handshake successful");
      uiEvent = APP_UI_SEC_ENCRYPT;
      break;

    case DM_SEC_ENCRYPT_FAIL_IND:
      LOG_INFO("ble", "Encrypted handshake failed");
      uiEvent = APP_UI_SEC_ENCRYPT_FAIL;
      break;

    case DM_SEC_AUTH_REQ_IND:
      AppHandlePasskey(&pMsg->dm.authReq);
      break;

    case DM_SEC_ECC_KEY_IND:
      DmSecSetEccKey(&pMsg->dm.eccMsg.data.key);
      break;

    case DM_SEC_COMPARE_IND:
      AppHandleNumericComparison(&pMsg->dm.cnfInd);
      break;

    case DM_HW_ERROR_IND:
      LOG_ERR("ble", "HW Error");
      uiEvent = APP_UI_HW_ERROR;
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
static void BleHandlerInit(void)
{
  APP_TRACE_INFO0("BleHandlerInit");

  /* store handler ID */
  bleHandlerId =WsfOsSetNextHandler(BleHandler);

  /* Set configuration pointers */
  pAppAdvCfg = (appAdvCfg_t *) &bleAdvCfg;
  pAppSlaveCfg = (appSlaveCfg_t *) &bleSlaveCfg;
  pAppSecCfg = (appSecCfg_t *) &bleSecCfg;
  pAppUpdateCfg = (appUpdateCfg_t *) &bleUpdateCfg;

  /* Initialize application framework */
  AppSlaveInit();

  /* Set stack configuration pointers */
  pSmpCfg = (smpCfg_t *) &bleSmpCfg;
  pAttCfg = (attCfg_t *) &bleAttCfg;

  /* initialize battery service server */
  BasInit(bleHandlerId, (basCfg_t *) &bleBasCfg);
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
static void BleHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (pMsg != NULL)
  {
    APP_TRACE_INFO1("Ble got evt %d", pMsg->event);

    if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END)
    {
      /* process advertising and connection-related messages */
      AppSlaveProcDmMsg((dmEvt_t *) pMsg);

      /* process security-related messages */
      AppSlaveSecProcDmMsg((dmEvt_t *) pMsg);
    }

    /* perform profile and user interface-related operations */
    bleProcMsg((bleMsg_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BleStart(void)
{

  BleHandlerInit();

  /* Register for stack callbacks */
  DmRegister(bleDmCback);
  DmConnRegister(DM_CLIENT_ID_APP, bleDmCback);
  AttRegister(bleAttCback);
  AttConnRegister(AppServerConnCback);
  AttsCccRegister(BLE_NUM_CCC_IDX, (attsCccSet_t *) bleCccSet, bleCccCback);

  /* Initialize attribute server database */
  SvcCoreAddGroup();
  SvcDisAddGroup(); // Device Information Service
  SvcBattCbackRegister(BasReadCback, NULL);
  SvcBattAddGroup();

  /* Reset the device */
  DmDevReset();
}
/* clang-format on */
