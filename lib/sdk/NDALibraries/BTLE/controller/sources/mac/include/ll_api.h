/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer interface file.
 *
 *  $Date: 2019-02-25 17:25:06 +0000 (Mon, 25 Feb 2019) $
 *  $Revision: 41190 $
 *
 *  Copyright (c) 2013 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 *
 *  \addtogroup LL_API          Link Layer (LL) API
 *  \{
 *
 *  The LL subsystem implements the Link Layer as defined in the Bluetooth core specification.
 *  The LL API is designed to be compatible with the standard HCI specification for low energy
 *  controllers. In addition, the LL API is designed to interface efficiently to the HCI layer
 *  API of Wicentric's host stack.
 *
 *  \defgroup LL_API_INIT       Initialization and Reset
 *  \defgroup LL_API_PARAM      Parameter Interface
 *  \defgroup LL_API_CTRL       Link Control Interface
 *  \defgroup LL_API_DATA       Data Interface
 *  \defgroup LL_API_CBACK      Callback Data Types
 *  \defgroup LL_API_TEST       Test Mode Interface
 *  \defgroup LL_API_SCENARIOS  Scenarios
 *
 *  \}
 */
/*************************************************************************************************/

#ifndef LL_API_H
#define LL_API_H

#include "wsf_types.h"
#include "wsf_os.h"
#include "util/bda.h"
#include "cfg_mac.h"
#include "ll_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define LL_FEAT_LEN             8       /*!< Length of features byte array */
#define LL_RAND_LEN             8       /*!< Length of random number */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      The following status values are used in the LL API. */
enum
{
  LL_SUCCESS                                            = 0x00,
  LL_ERROR_CODE_UNKNOWN_HCI_CMD                         = 0x01,
  LL_ERROR_CODE_UNKNOWN_CONN_ID                         = 0x02,
  LL_ERROR_CODE_HW_FAILURE                              = 0x03,
  LL_ERROR_CODE_PAGE_TIMEOUT                            = 0x04,
  LL_ERROR_CODE_AUTH_FAILURE                            = 0x05,
  LL_ERROR_CODE_PIN_KEY_MISSING                         = 0x06,
  LL_ERROR_CODE_MEM_CAP_EXCEEDED                        = 0x07,
  LL_ERROR_CODE_CONN_TIMEOUT                            = 0x08,
  LL_ERROR_CODE_CONN_LIMIT_EXCEEDED                     = 0x09,
  LL_ERROR_CODE_SYNCH_CONN_LIMIT_EXCEEDED               = 0x0A,
  LL_ERROR_CODE_ACL_CONN_ALREADY_EXISTS                 = 0x0B,
  LL_ERROR_CODE_CMD_DISALLOWED                          = 0x0C,
  LL_ERROR_CODE_CONN_REJ_LIMITED_RESOURCES              = 0x0D,
  LL_ERROR_CODE_CONN_REJECTED_SECURITY_REASONS          = 0x0E,
  LL_ERROR_CODE_CONN_REJECTED_UNACCEPTABLE_BDADDR       = 0x0F,
  LL_ERROR_CODE_CONN_ACCEPT_TIMEOUT_EXCEEDED            = 0x10,
  LL_ERROR_CODE_UNSUPPORTED_FEATURE_PARAM_VALUE         = 0x11,
  LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS                  = 0x12,
  LL_ERROR_CODE_REMOTE_USER_TERM_CONN                   = 0x13,
  LL_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES   = 0x14,
  LL_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_POWER_OFF       = 0x15,
  LL_ERROR_CODE_CONN_TERM_BY_LOCAL_HOST                 = 0x16,
  LL_ERROR_CODE_REPEATED_ATTEMPTS                       = 0x17,
  LL_ERROR_CODE_PAIRING_NOT_ALLOWED                     = 0x18,
  LL_ERROR_CODE_UNKNOWN_LMP_PDU                         = 0x19,
  LL_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE              = 0x1A,
  LL_ERROR_CODE_SCO_OFFSET_REJ                          = 0x1B,
  LL_ERROR_CODE_SCO_INTERVAL_REJ                        = 0x1C,
  LL_ERROR_CODE_SCO_AIR_MODE_REJ                        = 0x1D,
  LL_ERROR_CODE_INVALID_LMP_PARAMS                      = 0x1E,
  LL_ERROR_CODE_UNSPECIFIED_ERROR                       = 0x1F,
  LL_ERROR_CODE_UNSUPPORTED_LMP_PARAM_VAL               = 0x20,
  LL_ERROR_CODE_ROLE_CHANGE_NOT_ALLOWED                 = 0x21,
  LL_ERROR_CODE_LMP_LL_RESP_TIMEOUT                     = 0x22,
  LL_ERROR_CODE_LMP_ERR_TRANSACTION_COLLISION           = 0x23,
  LL_ERROR_CODE_LMP_PDU_NOT_ALLOWED                     = 0x24,
  LL_ERROR_CODE_ENCRYPT_MODE_NOT_ACCEPTABLE             = 0x25,
  LL_ERROR_CODE_LINK_KEY_CAN_NOT_BE_CHANGED             = 0x26,
  LL_ERROR_CODE_REQ_QOS_NOT_SUPPORTED                   = 0x27,
  LL_ERROR_CODE_INSTANT_PASSED                          = 0x28,
  LL_ERROR_CODE_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED     = 0x29,
  LL_ERROR_CODE_DIFFERENT_TRANSACTION_COLLISION         = 0x2A,
  LL_ERROR_CODE_RESERVED1                               = 0x2B,
  LL_ERROR_CODE_QOS_UNACCEPTABLE_PARAM                  = 0x2C,
  LL_ERROR_CODE_QOS_REJ                                 = 0x2D,
  LL_ERROR_CODE_CHAN_ASSESSMENT_NOT_SUPPORTED           = 0x2E,
  LL_ERROR_CODE_INSUFFICIENT_SECURITY                   = 0x2F,
  LL_ERROR_CODE_PARAM_OUT_OF_MANDATORY_RANGE            = 0x30,
  LL_ERROR_CODE_RESERVED2                               = 0x31,
  LL_ERROR_CODE_ROLE_SWITCH_PENDING                     = 0x32,
  LL_ERROR_CODE_RESERVED3                               = 0x33,
  LL_ERROR_CODE_RESERVED_SLOT_VIOLATION                 = 0x34,
  LL_ERROR_CODE_ROLE_SWITCH_FAILED                      = 0x35,
  LL_ERROR_CODE_EXTENDED_INQUIRY_RESP_TOO_LARGE         = 0x36,
  LL_ERROR_CODE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST    = 0x37,
  LL_ERROR_CODE_HOST_BUSY_PAIRING                       = 0x38,
  LL_ERROR_CODE_CONN_REJ_NO_SUITABLE_CHAN_FOUND         = 0x39,
  LL_ERROR_CODE_CONTROLLER_BUSY                         = 0x3A,
  LL_ERROR_CODE_UNACCEPTABLE_CONN_INTERVAL              = 0x3B,
  LL_ERROR_CODE_DIRECTED_ADV_TIMEOUT                    = 0x3C,
  LL_ERROR_CODE_ADV_TIMEOUT                             = 0x3C,
  LL_ERROR_CODE_CONN_TERM_MIC_FAILURE                   = 0x3D,
  LL_ERROR_CODE_CONN_FAILED_TO_ESTABLISH                = 0x3E,
  LL_ERROR_CODE_MAC_CONN_FAILED                         = 0x3F,
  LL_ERROR_CODE_COARSE_CLOCK_ADJUSTMENT_REJECTED        = 0x40,
  LL_ERROR_CODE_TYPE0_SUBMAP_NOT_DEFINED                = 0x41,
  LL_ERROR_CODE_UNKNOWN_ADVERTISING_IDENTIFIER          = 0x42,
  LL_ERROR_CODE_LIMIT_REACHED                           = 0x43,
  LL_ERROR_CODE_OPERATION_CANCELLED_BY_HOST             = 0x44
};

/*! \addtogroup LL_API_PARAM
 *  \{ */

/*! \brief      The features bitmask indicates the LE features supported by the LL. */
enum
{
  LL_FEAT_ENCRYPTION                = (1 << 0),     /*!< Encryption supported */
  LL_FEAT_CONN_PARAM_REQ_PROC       = (1 << 1),     /*!< Connection Parameters Request Procedure supported */
  LL_FEAT_EXT_REJECT_IND            = (1 << 2),     /*!< Extended Reject Indication supported */
  LL_FEAT_SLV_INIT_FEAT_EXCH        = (1 << 3),     /*!< Slave-Initiated Features Exchange supported */
  LL_FEAT_LE_PING                   = (1 << 4),     /*!< LE Ping supported */
  LL_FEAT_LE_DATA_LENGTH            = (1 << 5),     /*!< LE Data packet length extension supported */
  LL_FEAT_LL_PRIVACY                = (1 << 6),     /*!< LL Privacy supported */
  LL_FEAT_EXT_SCAN_POLICY           = (1 << 7),     /*!< Extended Scanner Filter Policy supported */
  LL_FEAT_LE_2M_PHY                 = (1 << 8),     /*!< LE 2M PHY supported */
  LL_FEAT_STABLE_MODULATION_IDX_TX  = (1 << 9),     /*!< Stable Modulation Index - Transmitter supported */
  LL_FEAT_STABLE_MODULATION_IDX_RX  = (1 << 10),    /*!< Stable Modulation Index - Receiver supported */
  LL_FEAT_LE_CODED_PHY              = (1 << 11),    /*!< LE Coded PHY supported */
  LL_FEAT_LE_EXT_ADV                = (1 << 12),    /*!< LE Extended Advertising supported */
  LL_FEAT_LE_PERIODIC_ADV           = (1 << 13),    /*!< LE Periodic Advertising supported */
  LL_FEAT_CH_SEL_ALGO_2             = (1 << 14),    /*!< Channel Selection Algorithm #2 supported */
  LL_FEAT_LE_PWR_CLASS_1            = (1 << 15),    /*!< LE Power Class 1 supported */
  LL_FEAT_MIN_NUM_USED_CH_PROCEDURE = (1 << 16),    /*!< Minimum Number of Used Channels Procedure supported */
  LL_FEAT_REMOTE_PK_VALID           = (1 << 27)     /*!< Remote Public Key Validation. Erratum 10734 */
};

/*! \brief      The supported state bitmask indicates the LE states supported by the LL. */
enum
{
  LL_SUP_STATE_NON_CONN_ADV                 = (0x1ULL <<  0), /*!< Non-connectable Advertising State supported. */
  LL_SUP_STATE_SCAN_ADV                     = (0x1ULL <<  1), /*!< Scannable Advertising State supported. */
  LL_SUP_STATE_CONN_ADV                     = (0x1ULL <<  2), /*!< Connectable Advertising State supported. */
  LL_SUP_STATE_DIRECT_ADV                   = (0x1ULL <<  3), /*!< Directed Advertising State supported. */
  LL_SUP_STATE_PASS_SCAN                    = (0x1ULL <<  4), /*!< Passive Scanning State supported. */
  LL_SUP_STATE_ACT_SCAN                     = (0x1ULL <<  5), /*!< Active Scanning State supported. */
  LL_SUP_STATE_INIT                         = (0x1ULL <<  6), /*!< Initiating State supported. Connection State in the Master Role supported is also supported. */
  LL_SUP_STATE_CONN_SLV                     = (0x1ULL <<  7), /*!< Connection State in the Slave Role supported. */
  LL_SUP_STATE_NON_CONN_ADV_AND_PASS_SCAN   = (0x1ULL <<  8), /*!< Non-connectable Advertising State and Passive Scanning State combination supported. */
  LL_SUP_STATE_SCAN_ADV_AND_PASS_SCAN       = (0x1ULL <<  9), /*!< Scannable Advertising State and Passive Scanning State combination supported. */
  LL_SUP_STATE_CONN_ADV_AND_PASS_SCAN       = (0x1ULL << 10), /*!< Connectable Advertising State and Passive Scanning State combination supported. */
  LL_SUP_STATE_DIRECT_ADV_AND_PASS_SCAN     = (0x1ULL << 11), /*!< Directed Advertising State and Passive Scanning State combination supported. */
  LL_SUP_STATE_NON_CONN_ADV_AND_ACT_SCAN    = (0x1ULL << 12), /*!< Non-connectable Advertising State and Active Scanning State combination supported. */
  LL_SUP_STATE_SCAN_ADV_AND_ACT_SCAN        = (0x1ULL << 13), /*!< Scannable Advertising State and Active Scanning State combination supported. */
  LL_SUP_STATE_CONN_ADV_AND_ACT_SCAN        = (0x1ULL << 14), /*!< Connectable Advertising State and Active Scanning State combination supported. */
  LL_SUP_STATE_DIRECT_ADV_ACT_SCAN          = (0x1ULL << 15), /*!< Directed Advertising State and Active Scanning State combination supported. */
  LL_SUP_STATE_NON_CONN_ADV_AND_INIT        = (0x1ULL << 16), /*!< Non-connectable Advertising State and Initiating State combination supported. */
  LL_SUP_STATE_SCAN_ADV_AND_INIT            = (0x1ULL << 17), /*!< Scannable Advertising State and Initiating State combination supported */
  LL_SUP_STATE_NON_CONN_ADV_MST             = (0x1ULL << 18), /*!< Non-connectable Advertising State and Master Role combination supported. */
  LL_SUP_STATE_SCAN_ADV_MST                 = (0x1ULL << 19), /*!< Scannable Advertising State and Master Role combination supported. */
  LL_SUP_STATE_NON_CONN_ADV_SLV             = (0x1ULL << 20), /*!< Non-connectable Advertising State and Slave Role combination supported. */
  LL_SUP_STATE_SCAN_ADV_SLV                 = (0x1ULL << 21), /*!< Scannable Advertising State and Slave Role combination supported. */
  LL_SUP_STATE_PASS_SCAN_AND_INIT           = (0x1ULL << 22), /*!< Passive Scanning State and Initiating State combination supported. */
  LL_SUP_STATE_ACT_SCAN_INIT                = (0x1ULL << 23), /*!< Active Scanning State and Initiating State combination supported. */
  LL_SUP_STATE_PASS_SCAN_MST                = (0x1ULL << 24), /*!< Passive Scanning State and Master Role combination supported. */
  LL_SUP_STATE_ACT_SCAN_MST                 = (0x1ULL << 25), /*!< Active Scanning State and Master Role combination supported. */
  LL_SUP_STATE_PASS_SCAN_SLV                = (0x1ULL << 26), /*!< Passive Scanning state and Slave Role combination supported. */
  LL_SUP_STATE_ACT_SCAN_SLV                 = (0x1ULL << 27), /*!< Active Scanning state and Slave Role combination supported. */
  LL_SUP_STATE_INIT_MST                     = (0x1ULL << 28), /*!< Initiating State and Master Role combination supported. Master Role and Master Role combination is also supported. */
  LL_SUP_STATE_LD_DIRECT_ADV                = (0x1ULL << 29), /*!< Low Duty Cycle Directed Advertising State. */
  LL_SUP_STATE_LD_DIRECT_ADV_PASS_SCAN      = (0x1ULL << 30), /*!< Low Duty Cycle Directed Advertising State and Passive Scanning State. */
  LL_SUP_STATE_LD_DIRECT_ADV_ACT_SCAN       = (0x1ULL << 31), /*!< Low Duty Cycle Directed Advertising State and Active Scanning State. */
  LL_SUP_STATE_CONN_ADV_INIT                = (0x1ULL << 32), /*!< Connectable Advertising State and Initiating State. */
  LL_SUP_STATE_HD_DIRECT_ADV_INIT           = (0x1ULL << 33), /*!< High Duty Cycle Directed Advertising State and Initiating State. */
  LL_SUP_STATE_LD_DIRECT_ADV_INIT           = (0x1ULL << 34), /*!< Low Duty Cycle Directed Advertising State and Initiating State. */
  LL_SUP_STATE_CONN_ADV_MST                 = (0x1ULL << 35), /*!< Connectable Advertising State and Connection State (Master Role). */
  LL_SUP_STATE_HD_DIRECT_ADV_MST            = (0x1ULL << 36), /*!< High Duty Cycle Directed Advertising State and Connection State (Master Role). */
  LL_SUP_STATE_LD_DIRECT_ADV_MST            = (0x1ULL << 37), /*!< Low Duty Cycle Directed Advertising State and Connection State (Master Role). */
  LL_SUP_STATE_CONN_ADV_SLV                 = (0x1ULL << 38), /*!< Connectable Advertising State and Connection State (Slave Role). */
  LL_SUP_STATE_HD_DIRECT_ADV_SLV            = (0x1ULL << 39), /*!< High Duty Cycle Directed Advertising State and Connection State (Slave Role). */
  LL_SUP_STATE_LD_DIRECT_ADV_SLV            = (0x1ULL << 40), /*!< Low Duty Cycle Directed Advertising State and Connection State (Slave Role). */
  LL_SUP_STATE_INIT_SLV                     = (0x1ULL << 41)  /*!< Initiating State and Connection State (Slave Role). */
};

/*! \} */    /* LL_API_PARAM */

/*! \addtogroup LL_API_CTRL
 *  \{ */

/*! \brief      This parameter identifies the device role. */
enum
{
  LL_ROLE_MASTER                = 0,            /*!< Role is master. */
  LL_ROLE_SLAVE                 = 1             /*!< Role is slave. */
};

/*! \brief      The advertising type indicates the connectable and discoverable nature of the advertising packets transmitted by a device. */
enum
{
  LL_ADV_CONN_UNDIRECT          = 0,            /*!< Connectable undirected advertising.  Peer devices can scan and connect to this device. */
  LL_ADV_CONN_DIRECT_HIGH_DUTY  = 1,            /*!< Connectable directed advertising, high duty cycle.  Only a specified peer device can connect to this device. */
  LL_ADV_DISC_UNDIRECT          = 2,            /*!< Discoverable undirected advertising.  Peer devices can scan this device but cannot connect. */
  LL_ADV_NONCONN_UNDIRECT       = 3,            /*!< Non-connectable undirected advertising.  Peer devices cannot scan or connect to this device. */
  LL_ADV_CONN_DIRECT_LOW_DUTY   = 4,            /*!< Connectable directed advertising, low duty cycle.  Only a specified peer device can connect to this device. */
  LL_EXT_ADV_CONN_UNDIRECT      = 5,            /*!< Connectable undirected advertising.  Peer devices cannot scan but can connect to this device.*/
  LL_EXT_ADV_NONCONN_DIRECT     = 6,            /*!< Non-connectable and non-scannable directed advertising. A specified peer device cannot scan or connect to this device. */
  LL_EXT_ADV_SCAN_DIRECT        = 7,            /*!< Scannable directed advertising. A specified peer device can scan but cannot connect to this device. */
  LL_EXT_PER_ADV                = 8,            /*!< Periodic advertising where the controller transmit ADV_EXT_IND and AUX_ADV_IND PDUs along with peridic advertising events (AUX_SYNC_IND PDU and its subordinate sets). */
  LL_PER_ADV                    = 9,            /*!< Periodic advertising where the controller transmit peridic advertising events (AUX_SYNC_IND PDU and its subordinate sets). */
  LL_ADV_NONE                   = 255           /*!< For internal use only */
};

/*! \brief      The address type indicates whether an address is public or random. */
enum
{
  LL_ADDR_PUBLIC                = 0,            /*!< Public address. */
  LL_ADDR_RANDOM                = 1,            /*!< Random address. */
  LL_ADDR_RES_PUBLIC            = 2,            /*!< Resolvable private address or public address. */
  LL_ADDR_RES_RANDOM            = 3             /*!< Resolvable private address or random address. */
};

/*! \brief      Different types of random address by the 2 Most significant bits */
enum
{
  LL_ADDR_RAND_NON_RESOLVE    = 0x00,           /*!< Random non resolvable. */
  LL_ADDR_RAND_RESOLVE        = 0x40,           /*!< Random resolvable. */
  LL_ADDR_RAND_STATIC         = 0xC0            /*!< Random static. */
};


/*! \brief      Advertising channel bit. */
enum
{
  LL_ADV_CHAN_37_BIT            = (1 << 0),     /*!< Advertising channel 37. */
  LL_ADV_CHAN_38_BIT            = (1 << 1),     /*!< Advertising channel 38. */
  LL_ADV_CHAN_39_BIT            = (1 << 2),     /*!< Advertising channel 39. */
  LL_ADV_CHAN_DATA              = (1 << 3),     /*!< Data channels. */
  LL_ADV_CHAN_ALL               = 0x7,          /*!< All advertising channels. */
};

/*! \brief      Advertising filter policy. */
enum
{
  LL_ADV_FILTER_NONE            = 0,            /*!< Scan from any device. */
  LL_ADV_FILTER_SCAN_WL_BIT     = 1,            /*!< Scan from White List only. */
  LL_ADV_FILTER_CONN_WL_BIT     = 2,            /*!< Connect from While List only. */
  LL_ADV_FILTER_WL_ONLY         = 3             /*!< Scan and connect from While List only. */
};

/*! \brief      Type of scan. */
enum
{
  LL_SCAN_PASSIVE               = 0,            /*!< Passive scanning. */
  LL_SCAN_ACTIVE                = 1             /*!< Active scanning. */
};

/*! \brief      Scan filter policy. */
enum
{
  LL_SCAN_FILTER_NONE           = 0,            /*!< Accept all advertising packets. */
  LL_SCAN_FILTER_WL_ONLY        = 1,            /*!< Accept from white list only. */
  LL_SCAN_FILTER_RES            = 2,            /*!< Accept from resolvable private addresses. */
  LL_SCAN_FILTER_RES_WL_ONLY    = 3             /*!< Accept from resolvable private addresses and white list. */
};

/*! \brief      Master clock accuracy. */
enum
{
  LL_MCA_500_PPM                = 0x00,         /*!< 500 ppm */
  LL_MCA_250_PPM                = 0x01,         /*!< 250 ppm */
  LL_MCA_150_PPM                = 0x02,         /*!< 150 ppm */
  LL_MCA_100_PPM                = 0x03,         /*!< 100 ppm */
  LL_MCA_75_PPM                 = 0x04,         /*!< 75 ppm */
  LL_MCA_50_PPM                 = 0x05,         /*!< 50 ppm */
  LL_MCA_30_PPM                 = 0x06,         /*!< 30 ppm */
  LL_MCA_20_PPM                 = 0x07          /*!< 20 ppm */
};

/*! \brief      This data structure is used in functions LlCreateConn() and LlConnUpdate(). */
typedef struct
{
  uint16_t        connIntervalMin;              /*!< Minimum connection interval. */
  uint16_t        connIntervalMax;              /*!< Maximum connection interval. */
  uint16_t        connLatency;                  /*!< Connection latency. */
  uint16_t        supTimeout;                   /*!< Supervision timeout. */
  uint16_t        minCeLen;                     /*!< Minimum CE length. */
  uint16_t        maxCeLen;                     /*!< Maximum CE length. */
} LlConnSpec_t;

/*! \brief       Advertising report event types. */
enum
{
  LL_RPT_TYPE_ADV_IND             = 0x00,       /*!< Connectable undirected advertising (ADV_IND). */
  LL_RPT_TYPE_ADV_DIRECT_IND      = 0x01,       /*!< Connectable directed advertising (ADV_DIRECT_IND). */
  LL_RPT_TYPE_ADV_SCAN_IND        = 0x02,       /*!< Scannable undirected advertising (ADV_SCAN_IND). */
  LL_RPT_TYPE_ADV_NONCONN_IND     = 0x03,       /*!< Non-connectable undirected advertising (ADV_NONCONN_IND). */
  LL_RPT_TYPE_SCAN_RSP            = 0x04,       /*!< Scan Response (SCAN_RSP). */

  LL_RRT_TYPE_EXT_ADV_IND                      = 0x13,
  LL_RRT_TYPE_EXT_ADV_DIRECT_IND               = 0x15,
  LL_RRT_TYPE_EXT_ADV_SCAN_IND                 = 0x12,
  LL_RRT_TYPE_EXT_ADV_NONCONN_IND              = 0x10,
  LL_RRT_TYPE_EXT_ADV_SCAN_RSP_TO_ADV_IND      = 0x1B,
  LL_RRT_TYPE_EXT_ADV_SCAN_RSP_TO_ADV_SCAN_IND = 0x1A
};

/*! \brief      This data structure is used in functions LlSetScanParam(). */
typedef struct
{
  uint8_t         scanType;
  uint16_t        scanInterval;
  uint16_t        scanWindow;
  uint8_t         ownAddrType;
  uint8_t         scanFiltPolicy;
} LlScanParam_t;

/*! \brief      This data structure is used in functions LlCreateConn(). */
typedef struct
{
  uint16_t        scanInterval;
  uint16_t        scanWindow;
  uint8_t         filterPolicy;
  uint8_t         peerAddrType;
  uint8_t         *pPeerAddr;
  uint8_t         ownAddrType;
} LlInitParam_t;

typedef struct
{
  uint32_t        perAdvInterMin;
  uint32_t        perAdvInterMax;
  uint16_t        perAdvProp;
} LlPerAdvParam_t;

typedef struct
{
  uint8_t         filterPolicy;
  uint8_t         advSID;
  uint8_t         advAddrType;
  uint8_t         *pAdvAddr;
  uint16_t        skip;
  uint16_t        syncTimeOut;
} LlPerAdvCreateSyncCmd_t;

typedef struct
{
  uint8_t         advAddrType;
  uint8_t         *pAdvAddr;
  uint8_t         advSID;
} LlDevicePerAdvList_t;

/*! \} */    /* LL_API_CTRL */

/*! \addtogroup LL_API_TEST
 *  \{ */

/*! \brief      Test packet payload type. */
enum
{
  LL_TEST_PKT_TYPE_PRBS9        = 0x00,         /*!< Pseudo-Random bit sequence 9. */
  LL_TEST_PKT_TYPE_0F           = 0x01,         /*!< 00001111'b packet payload type. */
  LL_TEST_PKT_TYPE_55           = 0x02,         /*!< 01010101'b packet payload type. */
  LL_TEST_PKT_TYPE_PRBS15       = 0x03,         /*!< Pseudo-Random bit sequence 15. */
  LL_TEST_PKT_TYPE_FF           = 0x04,         /*!< 11111111'b packet payload type. */
  LL_TEST_PKT_TYPE_00           = 0x05,         /*!< 00000000'b packet payload type. */
  LL_TEST_PKT_TYPE_F0           = 0x06,         /*!< 11110000'b packet payload type. */
  LL_TEST_PKT_TYPE_AA           = 0x07          /*!< 10101010'b packet payload type. */
};

/*! \brief      Test report data. */
typedef struct
{
  uint16_t      numTx;          /*!< Total transmit packet count. */
  uint16_t      numRxSuccess;   /*!< Successfully received packet count. */
  uint16_t      numRxCrcError;  /*!< CRC failed packet count. */
  uint16_t      numRxMissed;    /*!< Missed received packet count. */
} llTestReport_t;

/*! \} */    /* LL_API_TEST */

/*! \addtogroup LL_API_DATA
 *  \{ */

/*! \brief      ACL callback events */
enum
{
  LL_ACL_RX_IND,                /*!< ACL data packet received */
  LL_ACL_TX_CNF                 /*!< ACL data packet transmitted */
};

/*! \} */    /* LL_API_DATA */

/*! \addtogroup LL_API_CBACK
 *  \{ */

/*! \brief      Link control callback interface events */
enum
{
  LL_ERROR_IND = 0,             /*!< Unrecoverable LL or radio error occurred. */
  /* --- Core Spec 4.0 --- */
  LL_RESET_CNF,                 /*!< Reset complete. */
  LL_ADV_REPORT_IND,            /*!< LE advertising report. */
  LL_ADV_ENABLE_CNF,            /*!< Advertising enable/disable complete. */
  LL_SCAN_ENABLE_CNF,           /*!< Scan enable/disable complete. */
  LL_CONN_IND,                  /*!< LE connection complete. */
  LL_DISCONNECT_IND,            /*!< Disconnect complete. */
  LL_CONN_UPDATE_IND,           /*!< LE connection update complete. */
  LL_CREATE_CONN_CANCEL_CNF,    /*!< LE create connection cancel status. */
  LL_READ_REMOTE_VER_INFO_CNF,  /*!< Read remote version information complete. */
  LL_READ_REMOTE_FEAT_CNF,      /*!< LE read remote features complete. */
  LL_ENC_CHANGE_IND,            /*!< Encryption change. */
  LL_ENC_KEY_REFRESH_IND,       /*!< Encryption key refresh. */
  LL_LTK_REQ_IND,               /*!< LE LTK request. */
  LL_LTK_REQ_NEG_REPLY_CNF,     /*!< LTK request negative reply status. */
  LL_LTK_REQ_REPLY_CNF,         /*!< LTK request reply status. */
  /* --- Core Spec 4.2 --- */
  LL_REM_CONN_PARAM_IND,        /*!< LE remote connection parameter change. */
  LL_AUTH_PAYLOAD_TIMEOUT_IND,  /*!< Authenticated Payload Timeout occurred. */
  LL_DATA_LEN_CHANGE_IND,       /*!< Date length change occurred. */
  LL_READ_LOCAL_P256_PUB_KEY_CMPL_IND,
  LL_GENERATE_DHKEY_CMPL_IND,
  LL_SCAN_REPORT_IND,
  LL_ENH_CONN_IND,              /*!< LE enhanced connection complete. */
  LL_DIR_ADV_REPORT_IND,        /*!< LE directed advertising report. */
  /* --- Core Spec 5.0 --- */
  LL_PHY_UPDATE_IND,
  LL_EXT_ADV_REPORT_IND,        /*!< LE extended advertising report. */
  LL_EXT_SCAN_ENABLE_CNF,
  LL_SCAN_TIMEOUT_IND,
  LL_SCAN_REQ_RCVD_IND,
  LL_EXT_ADV_ENABLE_CNF,
  LL_ADV_SET_TERM_IND,
  LL_PER_ADV_ENABLE_CNF,
  LL_PER_ADV_SYNC_ESTD_IND,
  LL_PER_ADV_SYNC_LOST_IND,
  LL_PER_ADV_REPORT_IND,
  LL_CH_SEL_ALGO_IND,
  LL_API_EVENT_COUNT
};

/*! \brief      Advertising report indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       *pData;         /*!< Data buffer. */
  uint8_t       len;            /*!< Data buffer length. */
  int8_t        rssi;           /*!< RSSI. */
  uint8_t       eventType;      /*!< Event type. */
  uint8_t       addrType;       /*!< Address type. */
  bdAddr_t      addr;           /*!< Address. */

  /* direct fields */
  uint8_t       directAddrType; /*!< Directed address type. */
  bdAddr_t      directAddr;     /*!< Directed address. */
} llAdvReportInd_t;

/*! \brief      Extended advertising report indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint16_t      eventType;      /*!< Event type. */
  uint8_t       addrType;       /*!< Address type. */
  bdAddr_t      addr;           /*!< Address. */
  uint8_t       priPhy;         /*!< Primary PHY. */
  uint8_t       secPhy;         /*!< Secondary PHY. */
  uint8_t       advSid;         /*!< Advertising SID. */
  int8_t        txPwr;          /*!< Tx Power. */
  int8_t        rssi;           /*!< RSSI. */
  uint16_t      perAdvInt;      /*!< Periodic advertising interval. */
  uint8_t       directAddrType; /*!< Directed address type. */
  bdAddr_t      directAddr;     /*!< Directed address. */

	/**
	 * Data buffer length.
	 * \todo Explain why uint16_t is used. */
	//jo Spec Version 5.0 page 1217: Data_Length is 1 Octet
  uint16_t      len;
  uint8_t       *pData;         /*!< Data buffer. */
} llExtAdvReportInd_t;

/*! \brief      Connection indication */
typedef struct
{
  wsfMsgHdr_t   hdr;              /*!< Event header. */
  uint8_t       status;           /*!< Status. */
  uint16_t      handle;           /*!< Connection handle. */
  uint8_t       role;             /*!< Role of this device. */
  uint8_t       addrType;         /*!< Peer Address type. */
  bdAddr_t      peerAddr;         /*!< Peer address. */
  uint16_t      connInterval;     /*!< Connection interval. */
  uint16_t      connLatency;      /*!< Connection latency. */
  uint16_t      supTimeout;       /*!< Supervision timeout. */
  uint8_t       clockAccuracy;    /*!< Clock accuracy. */

  /* enhanced fields */
  bdAddr_t            localRpa;   /*!< Local resolvable private address. */
  bdAddr_t            peerRpa;    /*!< Peer resolvable private address. */
} llConnInd_t;

/*! \brief      Channel Selection Algorithm indication */
typedef struct
{
  wsfMsgHdr_t   hdr;              /*!< Event header. */
  uint16_t      handle;           /*!< Connection handle. */
  uint8_t       algo;             /*!< Channel selection algorithm */
} llChSelAlgo_t;

/*! \brief      Disconnect indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
  uint8_t       reason;         /*!< Reason code. */
} llDisconnectInd_t;

/*! \brief      Connect update indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
  uint16_t      connInterval;   /*!< Connection interval. */
  uint16_t      connLatency;    /*!< Connection latency. */
  uint16_t      supTimeout;     /*!< Supervision timeout. */
} llConnUpdateInd_t;

/*! \brief      Connection parameter change indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint16_t      handle;         /*!< Connection handle. */
  uint16_t      connIntervalMin;/*!< Minimum connection interval. */
  uint16_t      connIntervalMax;/*!< Maximum connection interval. */
  uint16_t      connLatency;    /*!< Connection latency. */
  uint16_t      supTimeout;     /*!< Supervision timeout. */
} llRemConnParamInd_t;

/*! \brief      Create connection cancel confirm */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
} llCreateConnCancelCnf_t;

/*! \brief      Read remote version information confirm */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
  uint8_t       version;        /*!< Bluetooth specification version. */
  uint16_t      mfrName;        /*!< Manufacturer ID. */
  uint16_t      subversion;     /*!< Subversion. */
} llReadRemoteVerInfoCnf_t;

/*! \brief      Read remote feature confirm */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
  uint8_t       features[LL_FEAT_LEN];  /*!< Features. */
} llReadRemoteFeatCnf_t;

/*! \brief      Encryption change indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
  uint8_t       enabled;        /*!< Encryption enabled. */
} llEncChangeInd_t;

/*! \brief      Encryption key refresh indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
} llEncKeyRefreshInd_t;

/*! \brief      LTK request indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint16_t      handle;         /*!< Connection handle. */
  uint8_t       randNum[LL_RAND_LEN];   /*!< Random number. */
  uint16_t      encDiversifier; /*!< Encryption diversifier. */
} llLtkReqInd_t;

/*! \brief      LTK request reply confirm */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
} llLtkReqReplyCnf_t;

/*! \brief      LTK request negative reply */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       status;         /*!< Status. */
  uint16_t      handle;         /*!< Connection handle. */
} llLtkReqNegReplyCnf_t;

/*! \brief      Authenticated Payload Timeout Indication */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint16_t      handle;         /*!< Connection handle. */
} llAuthPayloadTimeoutInd_t;

/*! \brief      LE Data Length Change indication */
typedef struct
{
  wsfMsgHdr_t         hdr;          /*!< Event header. */
  uint16_t            handle;       /*!< Connection handle. */
  uint16_t            maxTxOctets;  /*!< Maximum TX Payload bytes. */
  uint16_t            maxTxTime;    /*!< Maximum TX Payload time. */
  uint16_t            maxRxOctets;  /*!< Maximum RX Payload bytes. */
  uint16_t            maxRxTime;    /*!< Maximum RX Payload time. */
} llDataLenChangeInd_t;

/*! \brief      HW error event */
typedef struct
{
  wsfMsgHdr_t         hdr;          /*!< Event header. */
  uint8_t             status;       /*!< Status. */
  uint16_t            handle;       /*!< Connection handle. */
  uint8_t             txPhy;        /*!< TX PHY being used for this connection. */
  uint8_t             rxPhy;        /*!< RX PHY being used for this connection. */
} llPhyUpdateInd_t;

typedef struct
{
  wsfMsgHdr_t         hdr;                  /*!< Event header. */
  uint8_t             key[2*LL_ECC_KEY_LEN];/*!< Local Public P256 Key. */
} llReadLocalP256PubKeyCnf_t;

typedef struct
{
  wsfMsgHdr_t         hdr;                  /*!< Event header. */
  uint8_t             key[LL_ECC_KEY_LEN];  /*!< Shared DH Key. */
} llGenDHKeyCnf_t;

/*! \brief      HW error event */
typedef struct
{
  wsfMsgHdr_t   hdr;            /*!< Event header. */
  uint8_t       code;           /*!< Code. */
} llHwErrorEvt_t;

/*! \brief      Union of all event types */
typedef union
{
  wsfMsgHdr_t                 hdr;                    /*!< Event header. */
  llConnInd_t                 connInd;                /*!< LE connection complete. */
  llChSelAlgo_t               chSelAlgo;              /*!< LE Channel Selection Algorithm Event */
  llDisconnectInd_t           disconnectInd;          /*!< LE disconnect complete. */
  llConnUpdateInd_t           connUpdateInd;          /*!< LE connection update complete. */
  llRemConnParamInd_t         remConnParamInd;        /*!< LE remote connection parameter request. */
  llAdvReportInd_t            advReportInd;           /*!< LE advertising report. */
  llExtAdvReportInd_t         extAdvReportInd;        /*!< LE extended advertising report. */
  llCreateConnCancelCnf_t     createConnCancelCnf;    /*!< LE create connection cancel status. */
  llReadRemoteVerInfoCnf_t    readRemoteVerInfoCnf;   /*!< Read remote version information complete. */
  llReadRemoteFeatCnf_t       readRemoteFeatCnf;      /*!< LE read remote features complete. */
  llEncChangeInd_t            encChangeInd;           /*!< Encryption change */
  llEncKeyRefreshInd_t        encKeyRefreshInd;       /*!< Encryption key refresh */
  llLtkReqInd_t               ltkReqInd;              /*!< LE LTK request. */
  llLtkReqReplyCnf_t          ltkReqReplyCnf;         /*!< LTK request reply status. */
  llLtkReqNegReplyCnf_t       ltkReqNegReplyCnf;      /*!< LTK request negative reply status. */
  llHwErrorEvt_t              hwErrorEvt;             /*!< Unrecoverable LL or radio error occurred. */
  llAuthPayloadTimeoutInd_t   authPayloadTimeoutInd;  /*!< Authenticated Payload Timeout occurred. */
  llDataLenChangeInd_t        dataLenChagneInd;       /*!< LE Data length change indication. */
  llPhyUpdateInd_t            phyUpdateInd;           /*!< PHY update complete indication. */
  llReadLocalP256PubKeyCnf_t  readLocalP256Cnf;       /*!< Read Local P256 Public Key complete. */
  llGenDHKeyCnf_t             genDHKeyCnf;            /*!< Generate DH Key complete. */
} LlEvt_t;

/*! \brief      Event callback */
typedef bool_t (*llEvtCback_t)(LlEvt_t *pEvent);

/*! \brief      ACL callback */
typedef void (*llAclCback_t)(uint16_t handle, uint8_t numBufs);

/*! \} */    /* LL_API_CBACK */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup LL_API_INIT
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as an advertising slave.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an advertising slave.
 */
/*************************************************************************************************/
void LlAdvSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as a connectable slave.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an advertising and connectable slave.
 */
/*************************************************************************************************/
void LlConnSlaveInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as a scanning master.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as a scanning master.
 */
/*************************************************************************************************/
void LlScanMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as a connectable master.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as a scanning and initiating master.
 */
/*************************************************************************************************/
void LlConnMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem with task handler.
 *
 *  \param      handlerId  WSF handler ID.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem.  It is called once upon system initialization.
 *  It must be called before any other function in the LL API is called.
 */
/*************************************************************************************************/
void LlHandlerInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \brief      LL message dispatch handler.
 *
 *  \param      event       WSF event.
 *  \param      pMsg        WSF message.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LlHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief      Reset LL subsystem.
 *
 *  \return     None.
 *
 * Reset the LL subsystem.  All active connections are closed and all radio procedures such as
 * scanning or advertising are terminated.
 */
/*************************************************************************************************/
void LlReset(void);

/*************************************************************************************************/
/*!
 *  \brief      Register LL event handler.
 *
 *  \param      evtCback        Client callback function.
 *
 *  \return     None.
 *
 *  This function is called by a client to register for LL events.
 */
/*************************************************************************************************/
void LlEvtRegister(llEvtCback_t evtCback);

/*************************************************************************************************/
/*!
 *  \brief      Register ACL handler.
 *
 *  \param      sendCompCback   Client ACL send complete callback function.
 *  \param      recvPendCback   Client ACL receive pending callback function.
 *
 *  \return     None.
 *
 *  This function is called by a client to register for ACL data.
 */
/*************************************************************************************************/
void LlAclRegister(llAclCback_t sendCompCback, llAclCback_t recvPendCback);

/*! \} */    /* LL_API_INIT */

/*! \addtogroup LL_API_PARAM
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Set Bluetooth device address
 *
 *  \param      pAddr       Bluetooth device address.
 *
 *  \return     None.
 *
 *  Set the BD address to be used by LL.
 */
/*************************************************************************************************/
void LlSetBdAddr(const uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Get Bluetooth device address
 *
 *  \param      pAddr       Bluetooth device address.
 *
 *  \return     None.
 *
 *  Get the BD address currently used by LL or all zeros if address is not set.
 */
/*************************************************************************************************/
void LlGetBdAddr(uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Set random device address.
 *
 *  \param      pAddr       Random Bluetooth device address.
 *
 *  \return     None.
 *
 *  Set the random address to be used by LL.
 */
/*************************************************************************************************/
void LlSetRandAddr(const uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Get random device address
 *
 *  \param      pAddr       Random Bluetooth device address.
 *
 *  \return     None.
 *
 *  Get the random address currently used by LL or all zeros if address is not set.
 */
/*************************************************************************************************/
void LlGetRandAddr(uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Set advertising transmit power.
 *
 *  \param      advTxPwr        Advertising transmit power level.
 *
 *  \return     None.
 *
 *  Set the advertising transmit power.
 */
/*************************************************************************************************/
void LlSetAdvTxPower(int8_t advTxPwr);

/*************************************************************************************************/
/*!
 *  \brief      Get advertising transmit power.
 *
 *  \param      advTxPwr        Advertising transmit power level.
 *
 *  \return     None.
 *
 *  Get the advertising transmit power.
 */
/*************************************************************************************************/
void LlGetAdvTxPower(int8_t *pAdvTxPwr);

/*************************************************************************************************/
/*!
 *  \brief      Get supported states.
 *
 *  \param      pStates     Supported states bitmask.
 *
 *  \return     None.
 *
 *  Return the states supported by the LL.
 */
/*************************************************************************************************/
void LlGetSupStates(uint8_t *pStates);

/*************************************************************************************************/
/*!
 *  \brief      Get features.
 *
 *  \param      pFeatures   Supported features bitmask.
 *
 *  \return     None.
 *
 *  Return the LE features supported by the LL.
 */
/*************************************************************************************************/
void LlGetFeatures(uint8_t *pFeatures);

/*************************************************************************************************/
/*!
 *  \brief      Set features.
 *
 *  \param      pFeatures     Supported features bitmask.
 *  \param      pFeaturesMask Bitmask describing features to be modified.
 *
 *  \return     Status error code.
 *
 *  Set the LE features supported by the LL.
 */
/*************************************************************************************************/
uint8_t LlSetFeatures(const uint8_t *pFeatures, const uint8_t *pMask);

/*************************************************************************************************/
/*!       
 *  \brief  Vendor-specific set power level.
 *
 *  \param  param    power level in dBm. 
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlSetTxPower(int8_t power);

/*************************************************************************************************/
/*!
 *  \brief      Get white list size.
 *
 *  \return     Total number of white list entries.
 *
 *  Read the white list capacity supported by the LL.
 */
/*************************************************************************************************/
uint8_t LlGetWhitelistSize(void);

/*************************************************************************************************/
/*!
 *  \brief      Clear all white list entries.
 *
 *  \return     Status error code.
 *
 *  Clear all white list entries stored in the LL.
 *
 *  \note       This function must only be called when advertising or scan is disabled
 *              and not initiating.
 */
/*************************************************************************************************/
uint8_t LlClearWhitelist(void);

/*************************************************************************************************/
/*!
 *  \brief      Add device to the white list.
 *
 *  \param      addrType    TRUE for random address. FALSE for public.
 *  \param      pAddr       Bluetooth device address.
 *
 *  \return     Status error code.
 *
 *  Adds the given address to the white list stored in the LL.
 *
 *  \note       This function must only be called when advertising or scan is disabled
 *              and not initiating.
 */
/*************************************************************************************************/
uint8_t LlAddDeviceToWhitelist(bool_t addrType, bdAddr_t pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Remove device from the white list.
 *
 *  \param      addrType    TRUE for random address. FALSE for public.
 *  \param      pAddr       Bluetooth device address.
 *
 *  \return     Status error code.
 *
 *  Removes the given address from the white list stored in the LL.
 *
 *  \note       This function must only be called when advertising or scan is disabled
 *              and not initiating.
 */
/*************************************************************************************************/
uint8_t LlRemoveDeviceFromWhitelist(bool_t addrType, bdAddr_t pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Get random number.
 *
 *  \param      pRandNum        Buffer to store 8 bytes random data.
 *
 *  \return     Status error code.
 *
 *  Request the LL to generate a random number.
 */
/*************************************************************************************************/
uint8_t LlGetRandNum(uint8_t *pRandNum);

/*************************************************************************************************/
/*!
 *  \brief      Get RSSI of a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      pRssi           RSSI value.
 *
 *  \return     Status error code.
 *
 *  Get the current RSSI of a connection.
 */
/*************************************************************************************************/
uint8_t LlGetRssi(uint16_t handle, int8_t *pRssi);

/*************************************************************************************************/
/*!
 *  \brief      Get connection's TX power level.
 *
 *  \param      handle          Connection handle.
 *  \param      pLevel          Transmit power level.
 *
 *  \return     Status error code.
 *
 *  Get the TX power of a connection.
 */
/*************************************************************************************************/
uint8_t LlGetTxPowerLevel(uint16_t handle, uint8_t type, int8_t *pLevel);

/*************************************************************************************************/
/*!
 *  \brief      Set connection's TX power level.
 *
 *  \param      handle          Connection handle.
 *  \param      level           Transmit power level.
 *
 *  \return     Status error code.
 *
 *  Set the TX power of a connection.
 */
/*************************************************************************************************/
uint8_t LlSetTxPowerLevel(uint16_t handle, int8_t level);

/*************************************************************************************************/
/*!
 *  \brief      Get connection's channel map.
 *
 *  \param      handle          Connection handle.
 *  \param      pChanMap        Channel map.
 *
 *  \return     Status error code.
 *
 *  Get the current channel map of a connection.
 */
/*************************************************************************************************/
uint8_t LlGetChannelMap(uint16_t handle, uint8_t *pChanMap);

/*************************************************************************************************/
/*!
 *  \brief      Set channel map.
 *
 *  \param      pChanMap        Channel map.
 *
 *  \return     Status error code.
 *
 *  Set the current channel map used by the link layer.
 */
/*************************************************************************************************/
uint8_t LlSetChannelMap(uint8_t *pChanMap);

/*************************************************************************************************/
/*!
 *  \brief      Get connection's Authenticated Payload Timeout.
 *
 *  \param      handle          Connection handle.
 *  \param      pTimeout        Timeout (in milliseconds)
 *
 *  \return     Status error code.
 *
 *  Get the Authenticated Payload Timeout of a connection.
 */
/*************************************************************************************************/
uint8_t LlReadAuthPayloadTimeout(uint16_t handle, uint16_t *pTimeout);

/*************************************************************************************************/
/*!
 *  \brief      Set connection's Authenticated Payload Timeout.
 *
 *  \param      handle          Connection handle.
 *  \param      timeout10Ms     Timeout (in 10s of milliseconds).
 *
 *  \return     Status error code.
 *
 *  Set the Authenticated Payload Timeout of a connection.
 */
/*************************************************************************************************/
uint8_t LlWriteAuthPayloadTimeout(uint16_t handle, uint16_t timeout10Ms);

/*************************************************************************************************/
/*!
 *  \brief      Set advertising parameter.
 *
 *  \param      advIntervalMin  Minimum advertising interval.
 *  \param      advIntervalMax  Maximum advertising interval.
 *  \param      advType         Advertising type.
 *  \param      ownAddrType     Address type used by this device.
 *  \param      peerAddrType    Address type of peer device.  Only used for directed advertising.
 *  \param      pPeerAddr       Address of peer device.  Only used for directed advertising.
 *  \param      advChanMap      Advertising channel map.
 *  \param      advFiltPolicy   Advertising filter policy.
 *
 *  \return     Status error code.
 *
 *  Set advertising parameters.
 *
 *  \note       This function must only be called when advertising is disabled.
 */
/*************************************************************************************************/
uint8_t LlSetAdvParam(uint16_t advIntervalMin, uint16_t advIntervalMax, uint8_t advType,
                      uint8_t ownAddrType, uint8_t peerAddrType, const uint8_t *pPeerAddr,
                      uint8_t advChanMap, uint8_t advFiltPolicy);

/*************************************************************************************************/
/*!
 *  \brief      Set advertising data.
 *
 *  \param      len     Data buffer length.
 *  \param      pData   Advertising data buffer.
 *
 *  \return     Status error code.
 *
 *  Set advertising data data.
 */
/*************************************************************************************************/
uint8_t LlSetAdvData(uint8_t len, const uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief      Set scan parameters.
 *
 *  \param      param           Scan parameters.
 *
 *  \return     Status error code.
 *
 *  Set scan parameters.
 *
 *  \note       This function must only be called when scanning is disabled.
 */
/*************************************************************************************************/
uint8_t LlSetScanParam(/*@temp@*/ LlScanParam_t *param);

/*************************************************************************************************/
/*!
 *  \brief      Set scan response data.
 *
 *  \param      len     Data buffer length.
 *  \param      pData   Scan response data buffer.
 *
 *  \return     Status error code.
 *
 *  Set scan response data.
 */
/*************************************************************************************************/
uint8_t LlSetScanRespData(uint8_t len, const uint8_t *pData);

/*! \} */    /* LL_API_PARAM */

/*! \addtogroup LL_API_CTRL
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Advertising enable.
 *
 *  \param      enable          Set to TRUE to enable advertising, FALSE to disable advertising.
 *
 *  \return     None.

 *  Enable or disable advertising.
 */
/*************************************************************************************************/
void LlAdvEnable(uint8_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Scan enable.
 *
 *  \param      enable          Set to TRUE to enable scanning, FALSE to disable scanning.
 *  \param      filterDup       Set to TRUE to filter duplicates.
 *
 *  \return     None.
 *
 *  Enable or disable scanning.  This function is only used when operating in master role.
 */
/*************************************************************************************************/
void LlScanEnable(uint8_t enable, uint8_t filterDup);

/*************************************************************************************************/
/*!
 *  \brief      Create connection.
 *
 *  \param      scanInterval    Scan interval.
 *  \param      scanWindow      Scan window.
 *  \param      filterPolicy    Scan filter policy.
 *  \param      peerAddrType    Address type used for peer device.
 *  \param      pPeerAddr       Address of peer device.
 *  \param      ownAddrType     Address type used by this device.
 *  \param      pConnSpec       Connection specification.
 *
 *  \return     Status for command status event.
 *
 *  Create a connection to the specified peer address with the specified connection parameters.
 *  This function is only when operating in master role.
 */
/*************************************************************************************************/
uint8_t LlCreateConn(LlInitParam_t *pInitParam, LlConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief      Disconnect a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      reason          Disconnect reason.
 *
 *  \return     Status error code.
 *
 *  Disconnect a connection.
 */
/*************************************************************************************************/
uint8_t LlDisconnect(uint16_t handle, uint8_t reason);

/*************************************************************************************************/
/*!
 *  \brief      Cancel a create connection operation.
 *
 *  \return     Status error code.
 *
 *  Cancel a connection before it is established.  This function is only used when operating
 *  in master role.
 */
/*************************************************************************************************/
uint8_t LlCreateConnCancel(void);

/*************************************************************************************************/
/*!
 *  \brief      Update connection parameters.
 *
 *  \param      handle          Connection handle.
 *  \param      pConnSpec       New connection specification.
 *
 *  \return     None.
 *
 *  Update the connection parameters of a connection.
 */
/*************************************************************************************************/
void LlConnUpdate(uint16_t handle, LlConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief      Remote connection parameter request reply.
 *
 *  \param      handle          Connection handle.
 *  \param      pConnSpec       New connection specification.
 *
 *  \return     Status error code.
 *
 *  Reply to a connection parameter request.
 */
/*************************************************************************************************/
uint8_t LlRemoteConnParamReqReply(uint16_t handle, LlConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief      Remote connection parameter request negative reply.
 *
 *  \param      handle          Connection handle.
 *  \param      reason          Reason code.
 *
 *  \return     None.
 *
 *  Negative reply to a connection parameter request.
 */
/*************************************************************************************************/
uint8_t LlRemoteConnParamReqNegReply(uint16_t handle, uint8_t reason);

/*************************************************************************************************/
/*!
 *  \brief      Read remote features.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     Status error code.
 *
 *  Read the link layer features of the remote device.
 */
/*************************************************************************************************/
uint8_t LlReadRemoteFeat(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief      Read remote version information.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     Status error code.
 *
 *  Read the version information of the remote device.
 */
/*************************************************************************************************/
uint8_t LlReadRemoteVerInfo(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief      Encrypt data.
 *
 *  \param      pKey            Encryption key.
 *  \param      pData           16 bytes of plain text data.
 *
 *  \return     Status error code.
 *
 *  Request the LL to encryption a block of data in place.
 */
/*************************************************************************************************/
uint8_t LlEncrypt(uint8_t *pKey, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief      Start encryption.
 *
 *  \param      handle          Connection handle.
 *  \param      pRand           Random number.
 *  \param      diversifier     Diversifier value.
 *  \param      pKey            Pointer to the encryption key.
 *
 *  \return     None.
 *
 *  Start or restart link layer encryption on a connection.  This function is only used when
 *  operating in master role.
 */
/*************************************************************************************************/
void LlStartEncryption(uint16_t handle, uint8_t *pRand, uint16_t diversifier, uint8_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief      Reply to a LTK request.
 *
 *  \param      handle          Connection handle.
 *  \param      pKey            Pointer to new key.
 *
 *  \return     None.
 *
 *  Provide the requested LTK encryption key.  This function is only used when operating in
 *  slave mode.
 */
/*************************************************************************************************/
void LlLtkReqReply(uint16_t handle, uint8_t *pKey);

/*************************************************************************************************/
/*!
 *  \brief      Negative reply to a LTK request.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     None.
 *
 *  Requested LTK encryption key not available.  This function is only used when operating in
 *  slave mode.
 */
/*************************************************************************************************/
void LlLtkReqNegReply(uint16_t handle);

/*! \} */    /* LL_API_CTRL */

/*! \addtogroup LL_API_DATA
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Set the number of buffers in the LL ACL transmit queue.
 *
 *  \param      numBufs     Number of buffers.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LlSetAclTxBufs(uint8_t numBufs);

/*************************************************************************************************/
/*!
 *  \brief      Get the number of buffers in the LL ACL transmit queue.
 *
 *  \return     Number of buffers.
 */
/*************************************************************************************************/
uint8_t LlGetAclTxBufs(void);

/*************************************************************************************************/
/*!
 *  \brief      Set the number of buffers in the LL ACL receive queue.
 *
 *  \param      numBufs     Number of buffers.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LlSetAclRxBufs(uint8_t numBufs);

/*************************************************************************************************/
/*!
 *  \brief      Get the number of buffers in the LL ACL receive queue.
 *
 *  \return     Number of buffers.
 */
/*************************************************************************************************/
uint8_t LlGetAclRxBufs(void);

/*************************************************************************************************/
/*!
 *  \brief      Get the maximum size of an entry in the LL ACL queue.
 *
 *  \return     Number of bytes.
 */
/*************************************************************************************************/
uint16_t LlGetAclMaxSize(void);

/*************************************************************************************************/
/*!
 *  \brief      Set the data length for transmission PDU
 *
 *  \param      handle          Connection handle.
 *
 *  \param      txOctets Preferred maximum number of payload octetes. 0x1B-0xFB.
 *
 *  \param      txTime Preferred maximum number of microseconds. 0x148-0x848.
 * 
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetDataLen(uint16_t handle, uint16_t txOctets, uint16_t txTime);

/*************************************************************************************************/
/*!
 *  \brief      Get the initial data length for transmission PDU for new connections
 *
 *  \param      txOctets  Initial maximum payload octets. 0x1B-0xFB.
 *
 *  \param      txTime    Initial maximum microseconds. 0x148-0x848.
 * 
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadDefaulTxDataLen(uint16_t *txOctets, uint16_t *txTime);

/*************************************************************************************************/
/*!
 *  \brief      Set the data length for transmission PDU for new connections
 *
 *  \param      txOctets  Initial maximum payload octets. 0x1B-0xFB.
 *
 *  \param      txTime    Initial maximum microseconds. 0x148-0x848.
 * 
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlWriteDefaultDataLen(uint16_t suggestedMaxTxOctets, uint16_t suggestedMaxTxTime);

/*************************************************************************************************/
/*!
 *  \brief      Read the maximum data lengths
 *
 *  \param      pMaxTxOctets  Maximum transmit payload octets.
 *
 *  \param      pMaxTxTime    Maximum transmit payload microseconds.
 *
 *  \param      pMaxRxOctets  Maximum transmit payload octets.
 *
 *  \param      pMaxRxTime    Maximum transmit payload microseconds.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadMaximumDataLen(uint16_t *pMaxTxOctets, uint16_t *pMaxTxTime, uint16_t *pMaxRxOctets, uint16_t *pMaxRxTime);

/*************************************************************************************************/
/*!
 *  \brief      Send an ACL data packet.
 *
 *  \param      pData   Data buffer
 *
 *  \return     None.
 *
 *  Send an ACL data packet. pData points to an ACL buffer formatted according to [1]; the host
 *  must set the connection handle, flags, and length fields in the buffer.
 */
/*************************************************************************************************/
void LlSendAclData(uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief      Receive an ACL data packet
 *
 *  \return     Data buffer.
 *
 *  Receive an ACL data packet.  This function returns a pointer to an ACL buffer formatted
 *  according to [1].  The host must parse the header to determine the connection handle, flags,
 *  and length fields.  If no ACL buffers are available this function returns NULL.
 *
 *  The host must deallocate the buffer by calling WsfMsgFree() and call LlRecvBufCmpl() to
 *  update LL accounting.
 */
/*************************************************************************************************/
uint8_t *LlRecvAclData(void);

/*************************************************************************************************/
/*!
 *  \brief      Receive packet processing completed.
 *
 *  \param      numBufs     Number of completed packets.
 *
 *  \return     None.
 *
 *  Indicate to the LL receives buffers are available and, if needed, release flow control.
 */
/*************************************************************************************************/
void LlRecvBufCmpl(uint8_t numBufs);

/*! \} */    /* LL_API_DATA */

/*! \addtogroup LL_API_TEST
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Set advertising event access address.
 *
 *  \param      accessAddr   Access address.
 *
 *  \return     None.
 *
 *  Set the access address used in advertising, scanning and in test modes.
 */
/*************************************************************************************************/
void LlSetAdvAccessAddr(uint32_t accessAddr);

/*************************************************************************************************/
/*!
 *  \brief      Set advertising event CRC init value.
 *
 *  \param      crcInit      CRC initial value.
 *
 *  \return     None.
 *
 *  Set the CRC initial value used in advertising, scanning and test modes.
 */
/*************************************************************************************************/
void LlSetAdvCrcInit(uint16_t crcInit);

/*************************************************************************************************/
/*!
 *  \brief      Set test mode synchronization delay.
 *
 *  \param      delay       Delay in 625-us ticks.
 *
 *  \return     None.
 *
 *  Set the synchronization delay used in test mode.
 */
/*************************************************************************************************/
void LlSetTestSyncDelay(uint32_t delay);

/*************************************************************************************************/
/*!
 *  \brief      Enter transmit test mode.
 *
 *  \param      chan        Bluetooth channel.
 *  \param      len         Length of test data.
 *  \param      pktType     Test packet payload type.
 *  \param      numPkt      Auto terminate after number of packets, 0 for infinite.
 *
 *  \return     Status error code.
 *
 *  Start the transmit test mode on the given channel.
 */
/*************************************************************************************************/
uint8_t LlTxTest(uint8_t chan, uint8_t len, uint8_t pktType, uint16_t numPkt);

/*************************************************************************************************/
/*!
 *  \brief      Enter receive test mode.
 *
 *  \param      chan        Bluetooth channel.
 *  \param      numPkt      Auto terminate after number of successful packets, 0 for infinite.
 *
 *  \return     Status error code.
 *
 *  Start the receive test mode on the given channel.
 */
/*************************************************************************************************/
uint8_t LlRxTest(uint8_t chan, uint16_t numPkt);

/*************************************************************************************************/
/*!
 *  \brief      Enter enhanced transmit test mode.
 *
 *  \param      chan        Bluetooth channel.
 *  \param      len         Length of test data.
 *  \param      pktType     Test packet payload type.
 *  \param      phy         PHY to use for the test. LL_PHY_1M for legacy LlTxTest.
 *  \param      numPkt      Auto terminate after number of packets, 0 for infinite.
 *
 *  \return     Status error code.
 *
 *  Start the transmit test mode on the given channel.
 */
/*************************************************************************************************/
uint8_t LlEnhTxTest(uint8_t chan, uint8_t len, uint8_t pktType, uint8_t phy, uint16_t numPkt);

/*************************************************************************************************/
/*!
 *  \brief      Enter enhanced receive test mode.
 *
 *  \param      chan        Bluetooth channel.
 *  \param      phy         PHY to use for the test. LL_PHY_1M for legacy LlRxTest.
 *  \param      modIndex    Modulation Index, 0 = standard, 1 = stable. 0 for legacy LlRxTest.
 *  \param      numPkt      Auto terminate after number of successful packets, 0 for infinite.
 *
 *  \return     Status error code.
 *
 *  Start the receive test mode on the given channel.
 */
/*************************************************************************************************/
uint8_t LlEnhRxTest(uint8_t chan, uint8_t phy, uint8_t modIndex, uint16_t numPkt);

/*************************************************************************************************/
/*!
 *  \brief      End test mode.
 *
 *  \param      pRpt        Report return buffer.
 *
 *  \return     Status error code.
 *
 *  End test mode and return the report.
 */
/*************************************************************************************************/
uint8_t LlEndTest(llTestReport_t *pRpt);

/*************************************************************************************************/
/*!
 *  \brief      Initiate a LE ping procedure.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     None.
 *
 *  Initiate the LE ping LLCP.
 */
/*************************************************************************************************/
void LlPing(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief      Add device to the address resolution list.
 *
 *  \param      addrType    TRUE for random address. FALSE for public.
 *  \param      pAddr       Bluetooth device address.
 *  \param      pPIRK       Peer Identity Resolution Key.
 *  \param      pLIRK       Local Identity Resolution Key.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlAddDeviceToResolvingList(bool_t addrType, const uint8_t *pAddr,  const uint8_t *pPIRK, const uint8_t *pLIRK);

/*************************************************************************************************/
/*!
 *  \brief      Remove device from the address resolution list.
 *
 *  \param      addrType    TRUE for random address. FALSE for public.
 *  \param      pAddr       Bluetooth device address.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlRemoveDeviceFromResolvingList(bool_t addrType, const uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Get the maximum number of entries in the address resolution list.
 *
 *  \param      pResListSize  Resolving list size.
 *
 *  \return     Maximum number of entries in the address resolution list.
 */
/*************************************************************************************************/
void LlReadResolvingListSize(uint8_t *pResListSize);

/*************************************************************************************************/
/*!
 *  \brief      Get the peer resolvable private address
 *
 *  \param      addrType    TRUE for random address. FALSE for public.
 *  \param      pAddr       Public Bluetooth device address.
 *  \param      pPAddr      Pointer to hold peer resolvable Bluetooth device address.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadPeerResolvableAddr(bool_t addrType, const uint8_t *pAddr, uint8_t *pPAddr);

/*************************************************************************************************/
/*!
 *  \brief      Get the local resolvable private address
 *
 *  \param      addrType    TRUE for random address. FALSE for public.
 *  \param      pAddr       Public Bluetooth device address.
 *  \param      pPAddr      Pointer to hold local resolvable Bluetooth device address.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadLocalResolvableAddr(bool_t addrType, const uint8_t *pAddr, uint8_t *pLAddr);

/*************************************************************************************************/
/*!
 *  \brief      Clear the address resolution list.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlClearResolvingList(void);

/*************************************************************************************************/
/*!
 *  \brief      Enable or disable the address resolution.
 *
 *  \param      enable      TRUE to enable, FALSE to disable.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetAddrResolutionEnable(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Set the length of time the controller will use a Resolvable Private Address.
 *
 *  \param      timeout      Number of seconds.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetResolvablePrivateAddrTimeout(uint16_t timeout);

//typedef void LlExtAdvParam_t;
/*! \brief Extended advertising parameters */
typedef struct
{
  uint16_t            advEventProp;    /*!< \brief Advertising Event Properties. */
  uint32_t            priAdvInterMin;  /*!< \brief Primary Advertising Interval Minimum. */
  uint32_t            priAdvInterMax;  /*!< \brief Primary Advertising Interval Maximum. */
  uint8_t             priAdvChanMap;   /*!< \brief Primary Advertising Channel Map. */
  uint8_t             ownAddrType;     /*!< \brief Own Address Type. */
  uint8_t             peerAddrType;    /*!< \brief Peer Address Type. */
  uint8_t             *pPeerAddr;      /*!< \brief Peer Address. */
  uint8_t             advFiltPolicy;   /*!< \brief Advertising Filter Policy. */
  int8_t              advTxPwr;        /*!< \brief Advertising Tx Power. */
  uint8_t             priAdvPhy;       /*!< \brief Primary Advertising PHY. */
  uint8_t             secAdvMaxSkip;   /*!< \brief Secondary Advertising Maximum Skip. */
  uint8_t             secAdvPhy;       /*!< \brief Secondary Advertising PHY. */
  uint8_t             advSetId;        /*!< \brief Advertising set ID. */
  uint8_t             scanReqNotifEna; /*!< \brief Scan Request Notification Enable. */
} LlExtAdvParam_t;

//typedef void LlExtAdvEnableParam_t;
/*! \brief Extended advertising enable parameters */
typedef struct
{
  uint8_t             advHandle;       /*!< \brief Advertising handle. */
  uint16_t            duration;        /*!< \brief Advertising duration in 10 ms units. */
  uint8_t             maxEaEvents;     /*!< \brief Maximum number of extended advertising events. */
} LlExtAdvEnableParam_t;

/*! \brief This data structure is used in functions LlSetExtScanParam(). */
typedef struct
{
  uint8_t       scanType;     /*!< \brief Scan type. */
  uint16_t      scanInterval; /*!< \brief Scan interval. */
  uint16_t      scanWindow;   /*!< \brief Scan window. */
} LlExtScanParamPhyParam_t;

typedef struct
{
  uint8_t         ownAddrType;
  uint8_t         scanFiltPolicy;

	/*! Scan phys. */
  bool_t        phys[LL_MAX_NUM_PHYS];

	/*! Scan perm phy parameters. */
  LlExtScanParamPhyParam_t phyParam[LL_MAX_NUM_PHYS];
} LlExtScanParam_t;

typedef void LlExtInitParam_t;
typedef void LlExtInitScanParam_t;

void LlGetVersion(uint16_t *manufacturerName, uint8_t *pHciVersion, uint16_t *pHciRevision);
void LlSetPrivacyMode(uint8_t addrType, const uint8_t *pAddr, uint8_t mode);
void LlGenerateP256KeyPair(void);
void LlGenerateDhKey(uint8_t *pPubKeyX, uint8_t *pPubKeyY);
void LlSetAdvSetRandAddr(uint8_t advHandle, const uint8_t *pAddr);

/*************************************************************************************************/
/*!
 *  \brief      Set extended advertising parameter.
 *
 *  \param      advHandle       Advertising handle.
 *  \param      pExtAdvParam    Extended advertising parameters.
 *
 *  \return     Status error code.
 *
 *  \note       This function must only be called when advertising is disabled.
 */
/*************************************************************************************************/
uint8_t LlSetExtAdvParam(uint8_t advHandle, LlExtAdvParam_t *pExtAdvParam);

/*************************************************************************************************/
/*!
 *  \brief      Set extended advertising data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      fragPref    Fragment preference.
 *  \param      len         Data buffer length.
 *  \param      pData       Advertising data buffer.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetExtAdvData(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len, const uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief      Set extended scan response data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      fragPref    Fragment preference.
 *  \param      len         Data buffer length.
 *  \param      pData       Scan response data buffer.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetExtScanRespData(uint8_t advHandle, uint8_t op, uint8_t fragPref, uint8_t len, const uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief      Set extended advertising enable command.
 *
 *  \param      enable        Set to TRUE to enable advertising, FALSE to disable advertising.
 *  \param      numSets       Number of advertising sets.
 *  \param      pEnableParam  Advertising enable parameter array.
 */
/*************************************************************************************************/
void LlExtAdvEnable(uint8_t enable, uint8_t numSets, LlExtAdvEnableParam_t *pEnableParam);

/*************************************************************************************************/
/*!
 *  \brief      Remove advertising set command.
 *
 *  \param      advHandle    Advertising handle.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlRemoveAdvSet(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief      Clear advertising sets command.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlClearAdvSets(void);

/*************************************************************************************************/
/*!
 *  \brief      Read transmit power.
 *
 *  \param      minTxPower      Minimum transmit power supported by the controller.
 *  \param      maxTxPower      Maximum transmit power supported by the controller.
 */
/*************************************************************************************************/
void LlReadTxPower(int8_t *minTxPower, int8_t *maxTxPower);

/*************************************************************************************************/
/*!
 *  \brief      Read RF path compensation parameters.
 *
 *  \param      txPathComp      RF Path compensation value used for TX Power level calculation.
 *  \param      rxPathComp      RF Path compensation value used for RSSI calculation. 
 */
/*************************************************************************************************/
void LlReadRfPathComp(int16_t *txPathComp, int16_t *rxPathComp);

/*************************************************************************************************/
/*!
 *  \brief      Write RF path compensation parameters.
 *  \param      txPathComp      RF Path compensation value used for TX Power level calculation.
 *  \param      rxPathComp      RF Path compensation value used for RSSI calculation.
 */
/*************************************************************************************************/
void LlWriteRfPathComp(int16_t txPathComp, int16_t rxPathComp);

/*************************************************************************************************/
/*!
 *  \brief      Set extended scanning parameters command.
 *
 *  \param      pScanParam      Scanning parameter array.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetExtScanParam(/*@temp@*/ LlExtScanParam_t *pScanParam);

/*************************************************************************************************/
/*!
 *  \brief      Extended scan enable command.
 *
 *  \param      enable          Set to TRUE to enable scanning, FALSE to disable scanning.
 *  \param      filterDup       Set to TRUE to filter duplicates.
 *  \param      duration        Duration.
 *  \param      period          Period.
 */
/*************************************************************************************************/
void LlExtScanEnable(uint8_t enable, uint8_t filterDup, uint16_t duration, uint16_t period);

/*************************************************************************************************/
/*!
 *  \brief      Extended create connection command.
 *
 *  \param      pInitParam      Initiating parameters.
 *  \param      pScanParam      Initiating scan parameters.
 *  \param      pConnSpec       Connection specification.
 *
 *  \return     None.
 */
/*************************************************************************************************/
uint8_t LlExtCreateConn(LlExtInitParam_t *pInitParam, LlExtInitScanParam_t *pScanParam, LlConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief      The LE_Read_PHY command is used to read the current transmitter PHY and
 *              receiver PHY on the connection identified by the Connection_Handle.
 *
 *  \param      handle          Connection handle.
 *  \param      pTxPhy          PHY used for TX.
 *  \param      pRxPhy          PHY used for RX.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadPhy(uint16_t handle, uint8_t *pTxPhy, uint8_t *pRxPhy);

/*************************************************************************************************/
/*!
 *  \brief      The LE_Set_Default_PHY command allows the Host to specify its preferred
 *              values for the transmitter PHY and receiver PHY to be used for all subsequent
 *              connections over the LE transport.
 *
 *  \param      allPhys         The ALL_PHYS parameter is a bit field that allows the Host to specify, for each
 *                              direction, whether it has no preference among the PHYs that the Controller
 *                              supports in a given direction or whether it has specified particular PHYs that it
 *                              prefers in the TX_PHYS or RX_PHYS parameter.
 *                               
 *  \param      TxPhys          The TX_PHYS parameter is a bit field that indicates the transmitter PHYs that
 *                              the Host prefers the Controller to use. If the ALL_PHYS parameter specifies
 *                              that the Host has no preference, the TX_PHYS parameter is ignored;
 *                              otherwise at least one bit shall be set to 1.
 *
 *  \param      RxPhys          The RX_PHYS parameter is a bit field that indicates the receiver PHYs that the
 *                              Host prefers the Controller to use. If the ALL_PHYS parameter specifies that
 *                              the Host has no preference, the RX_PHYS parameter is ignored; otherwise at
 *                              least one bit shall be set to 1.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetDefaultPhy(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys);

/*************************************************************************************************/
/*!
 *  \brief      The LE_Set_PHY command is used to set the PHY preferences for the
 *              connection identified by the Connection_Handle. The Controller might not be
 *              able to make the change (e.g. because the peer does not support the
 *              requested PHY) or may decide that the current PHY is preferable.
 *
 *  \param      handle          Connection handle.
 *  \param      allPhys         The ALL_PHYS parameter is a bit field that allows the Host to specify, for each
 *                              direction, whether it has no preference among the PHYs that the Controller
 *                              supports in a given direction or whether it has specified particular PHYs that it
 *                              prefers in the TX_PHYS or RX_PHYS parameter.
 *                               
 *  \param      TxPhys          The TX_PHYS parameter is a bit field that indicates the transmitter PHYs that
 *                              the Host prefers the Controller to use. If the ALL_PHYS parameter specifies
 *                              that the Host has no preference, the TX_PHYS parameter is ignored;
 *                              otherwise at least one bit shall be set to 1.
 *
 *  \param      RxPhys          The RX_PHYS parameter is a bit field that indicates the receiver PHYs that the
 *                              Host prefers the Controller to use. If the ALL_PHYS parameter specifies that
 *                              the Host has no preference, the RX_PHYS parameter is ignored; otherwise at
 *                              least one bit shall be set to 1.
 * 
 * \param       phyOptions      The PHY_options parameter is a bit field that allows the Host to specify options
 *                              for PHYs. The default value for a new connection shall be all zero bits. The
 *                              Controller may override any preferred coding for transmitting on the LE Coded
 *                              PHY.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetPhy(uint16_t handle, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions);

/*************************************************************************************************/
/*!
 *  \brief      Translate the PHY preference bit to the PHY value.
 *
 *  \param      pref            PHY preference bit.
 *
 *  \return     PHY value.
 */
/*************************************************************************************************/
uint8_t LlTranslatePhyPref(uint8_t pref);

/*************************************************************************************************/
/*!
 *  \brief      Translate the PHY preference bit to the PHY value.
 *
 *  \param      value           PHY value.
 *
 *  \return     PHY preference bit.
 */
/*************************************************************************************************/
uint8_t LlTranslatePhyValue(uint8_t value);

/*************************************************************************************************/
/*!
 *  \brief      Maximum length of data supported by the Controller for use asadvertisement data or 
 *              scan response data in an advertising event or as periodic advertisement data.
 *
 *  \param      pMaxAdvDataLen  Controller maximum advertisement (or scan response) data length.
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadMaxAdvDataLen(uint16_t *pMaxAdvDataLen);

/*************************************************************************************************/
/*!
 *  \brief      Read the maximum number of advertising sets supported by the advertising Controller.
 *
 *  \param      pNumSupAdvSets  Controller maximum number of advertising sets.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadNumSupAdvSets(uint8_t *pNumSupAdvSets);

/*************************************************************************************************/
/*!
*  \brief      Read the power used for advertising packets.
*
*  \return     Power level in dBm units.
*
*/
/*************************************************************************************************/
uint8_t LlReadAdvTxPower(int8_t *power);

void LlRecvAclDataComplete(uint8_t);

/*************************************************************************************************/
/*!
 *  \brief      Set periodic advertising parameters command.
 *
 *  \param      advHandle       Advertising handle.
 *  \param      advIntervalMin  Periodic advertising interval minimum.
 *  \param      advIntervalMax  Periodic advertising interval maximum.
 *  \param      advProps        Periodic advertising properties.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetPeriodicAdvParam(uint8_t advHandle, LlPerAdvParam_t *pPerAdvParam);

/*************************************************************************************************/
/*!
 *  \brief      Set periodic advertising data command.
 *
 *  \param      advHandle   Advertising handle.
 *  \param      op          Operation.
 *  \param      len         Data buffer length.
 *  \param      pData       Advertising data buffer.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetPeriodicAdvData(uint8_t advHandle, uint8_t op, uint8_t len, const uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief      Set periodic advertising enable command.
 *
 *  \param      enable      Set to TRUE to enable advertising, FALSE to disable advertising.
 *  \param      advHandle   Advertising handle.
 */
/*************************************************************************************************/
void LlSetPeriodicAdvEnable(uint8_t advHandle, uint8_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Periodic advertising create sync command.
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
uint8_t LlPeriodicAdvCreateSync(LlPerAdvCreateSyncCmd_t *pParam);

/*************************************************************************************************/
/*!
 *  \brief      Periodic advertising create sync cancel command.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlPeriodicAdvCreateSyncCancel(void);

/*************************************************************************************************/
/*!
 *  \brief      Periodic advertising terminate sync command.
 *
 *  \param      syncHandle      Sync handle.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlPeriodicAdvTerminateSync(uint16_t syncHandle);

uint8_t LlGetPeriodicAdvListSize(uint8_t *pListSize);

/*************************************************************************************************/
/*!
 *  \brief      Add device to periodic advertiser list command.
 *
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      advSid          Advertising SID.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlAddDeviceToPeriodicAdvList(LlDevicePerAdvList_t *pParam);

/*************************************************************************************************/
/*!
 *  \brief      Remove device from periodic advertiser list command.
 *
 *  \param      advAddrType     Advertiser address type.
 *  \param      pAdvAddr        Advertiser address.
 *  \param      advSid          Advertising SID.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlRemoveDeviceFromPeriodicAdvList(LlDevicePerAdvList_t *pParam);

/*************************************************************************************************/
/*!
 *  \brief      HCI LE clear periodic advertiser list command.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlClearPeriodicAdvList(void);

/*************************************************************************************************/
/*!
 *  \brief      The LE_Read_PHY command is used to read the current transmitter PHY and
 *              receiver PHY on the connection identified by the Connection_Handle.
 *
 *  \param      handle          Connection handle.
 *  \param      pTxPhy          PHY used for TX.
 *  \param      pRxPhy          PHY used for RX.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlReadPhy(uint16_t handle, uint8_t *pTxPhy, uint8_t *pRxPhy);

/*************************************************************************************************/
/*!
 *  \brief      The LE_Set_Default_PHY command allows the Host to specify its preferred
 *              values for the transmitter PHY and receiver PHY to be used for all subsequent
 *              connections over the LE transport.
 *
 *  \param      allPhys         The ALL_PHYS parameter is a bit field that allows the Host to specify, for each
 *                              direction, whether it has no preference among the PHYs that the Controller
 *                              supports in a given direction or whether it has specified particular PHYs that it
 *                              prefers in the TX_PHYS or RX_PHYS parameter.
 *                               
 *  \param      TxPhys          The TX_PHYS parameter is a bit field that indicates the transmitter PHYs that
 *                              the Host prefers the Controller to use. If the ALL_PHYS parameter specifies
 *                              that the Host has no preference, the TX_PHYS parameter is ignored;
 *                              otherwise at least one bit shall be set to 1.
 *
 *  \param      RxPhys          The RX_PHYS parameter is a bit field that indicates the receiver PHYs that the
 *                              Host prefers the Controller to use. If the ALL_PHYS parameter specifies that
 *                              the Host has no preference, the RX_PHYS parameter is ignored; otherwise at
 *                              least one bit shall be set to 1.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetDefaultPhy(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys);

/*************************************************************************************************/
/*!
 *  \brief      The LE_Set_PHY command is used to set the PHY preferences for the
 *              connection identified by the Connection_Handle. The Controller might not be
 *              able to make the change (e.g. because the peer does not support the
 *              requested PHY) or may decide that the current PHY is preferable.
 *
 *  \param      handle          Connection handle.
 *  \param      allPhys         The ALL_PHYS parameter is a bit field that allows the Host to specify, for each
 *                              direction, whether it has no preference among the PHYs that the Controller
 *                              supports in a given direction or whether it has specified particular PHYs that it
 *                              prefers in the TX_PHYS or RX_PHYS parameter.
 *                               
 *  \param      TxPhys          The TX_PHYS parameter is a bit field that indicates the transmitter PHYs that
 *                              the Host prefers the Controller to use. If the ALL_PHYS parameter specifies
 *                              that the Host has no preference, the TX_PHYS parameter is ignored;
 *                              otherwise at least one bit shall be set to 1.
 *
 *  \param      RxPhys          The RX_PHYS parameter is a bit field that indicates the receiver PHYs that the
 *                              Host prefers the Controller to use. If the ALL_PHYS parameter specifies that
 *                              the Host has no preference, the RX_PHYS parameter is ignored; otherwise at
 *                              least one bit shall be set to 1.
 * 
 * \param       phyOptions      The PHY_options parameter is a bit field that allows the Host to specify options
 *                              for PHYs. The default value for a new connection shall be all zero bits. The
 *                              Controller may override any preferred coding for transmitting on the LE Coded
 *                              PHY.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t LlSetPhy(uint16_t handle, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions);

/*************************************************************************************************/
/*!
 *  \brief      Function to send custom LLCP PDUs
 *
 *  \param      handle          Connection handle.
 *  \param      type            Type field for the LLCP PDU.
 *  \param      length          Length of the data to send.
 *  \param      data            Buffer holding the data to send.
 */
/*************************************************************************************************/
void LlSendPdu(uint16_t handle, uint8_t type, uint8_t length, uint8_t *data);

/*! \} */    /* LL_API_TEST */

#ifdef __cplusplus
};
#endif

#endif /* LL_API_H */
