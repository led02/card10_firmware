/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      MAC system configuration.
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
 */
/*************************************************************************************************/

#ifndef CFG_MAC_H
#define CFG_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** BLE LL ***/

#ifndef LL_MAX_CONN
#define LL_MAX_CONN                     5                       /*!< Maximum number of connections. */
#endif

#ifndef LL_NUM_BUFS
#define LL_NUM_BUFS                     4                       /*!< Default number of buffers. */
#endif

#ifndef LL_NUM_ADV_FILT
#define LL_NUM_ADV_FILT                 4                       /*!< Table size for advertising filter. */
#endif

#ifndef LL_ADV_FILT_TIMEOUT
#define LL_ADV_FILT_TIMEOUT             0x8000                  /*!< Advertising filter timeout in 0.625 ms units. */
#endif

#ifndef LL_NUM_WL_ENTRIES
#define LL_NUM_WL_ENTRIES               8                       /*!< Maximum number of white list entries, 0xFF is highest setting */
#endif

#ifndef LL_NUM_ADDR_RES_LIST_ENTRIES
#define LL_NUM_ADDR_RES_LIST_ENTRIES    8                       /*!< Number of entries in the Address Resolution List */
#endif

#ifndef LL_API_PARAM_CHECK
#define LL_API_PARAM_CHECK              1                       /*!< Check API parameters. */
#endif

#ifndef LL_COMP_ID
#define LL_COMP_ID                      0x058B                  /*!< Company ID (Maxim Integrated Products). */
#endif

#ifndef LL_IMPL_REV
#define LL_IMPL_REV                     0x0000                  /*!< Implementation revision number. */
#endif

#ifndef LL_ENABLE_VS
#define LL_ENABLE_VS                    1                       /*!< Enable vendor specific command processing. */
#endif

#ifndef LL_ENABLE_WW
#define LL_ENABLE_WW                    1                       /*!< Window widening enable. Disable for diagnostics only. */
#endif

#ifndef LL_ENABLE_ADV_DLY
#define LL_ENABLE_ADV_DLY               1                       /*!< Enable advertising delay. Disable for diagnostics only. */
#endif

#ifndef LL_ENABLE_SLV_LATENCY
#define LL_ENABLE_SLV_LATENCY           1                       /*!< Enable slave latency. Disable for diagnostics only. */
#endif

#ifndef LL_NUM_ADV_SETS
#define LL_NUM_ADV_SETS                 1                       /*!< Number of advertising sets supported by the Controller. */
#endif

#ifndef LL_AUX_FRAME_SPACE
#define LL_AUX_FRAME_SPACE              (3 * 300)               /*!< AUX Frame Space, where T_MAFS is 300 usec*/
#endif

#ifndef LL_ADV_SCAN_RSP_DATA_LEN
#define LL_ADV_SCAN_RSP_DATA_LEN        1000                    /*!< Advertising (or scan response) data length supported by the Controller. */
#endif

#ifndef LL_FRAG_PREFERED_DATA_LEN
#define LL_FRAG_PREFERED_DATA_LEN       31                      /*!< Advertising (or scan response) data length when Controller prefer to fragment all Host advertising data. */
#endif

#ifndef LL_NO_FRAG_PREFERED_DATA_LEN
#define LL_NO_FRAG_PREFERED_DATA_LEN    191                     /*!< Advertising (or scan response) data length when Controller does not prefer to fragment all Host advertising data. */
                                                                /*!< 254 - 63 = 191 (Payload: 1-255 octets -> ExtendedHeaderLength(6bits) + AdvMode(2 bits) + Extended Header(0-63 octets) + AdvData(0-254 octets)) */
#endif

#ifndef LL_MAX_TX_TIME
#define LL_MAX_TX_TIME                  LL_MAX_DATA_TIME_ABS_MAX /*!< Maximum number of microseconds the device will transmit a PDU */
#endif

#ifndef LL_MAX_TX_OCTETS
#define LL_MAX_TX_OCTETS                LL_MAX_DATA_LEN_ABS_MAX /*!< Maximum number of bytes the device will transmit in a PDU */
#endif

#ifndef LL_MAX_RX_TIME
#define LL_MAX_RX_TIME                  LL_MAX_DATA_TIME_ABS_MAX /*!< Maximum number of microseconds the device will receive a PDU */
#endif

#ifndef LL_MAX_RX_OCTETS
#define LL_MAX_RX_OCTETS                LL_MAX_DATA_LEN_ABS_MAX /*!< Maximum number of bytes the device will receive in a PDU */
#endif

#ifndef LL_DEF_TX_TIME
#define LL_DEF_TX_TIME                  0x148                   /*!< Specified default maximum number of microseconds the device will transmit a PDU */
#endif

#ifndef LL_DEF_TX_OCTETS
#define LL_DEF_TX_OCTETS                0x1B                    /*!< Specified default maximum number of bytes the device will transmit in a PDU */
#endif

#ifndef LL_MIN_TX_POWER
#define LL_MIN_TX_POWER                 -10                     /*!< Minimum supported TX power level, in dBm */
#endif

#ifndef LL_MAX_TX_POWER
#define LL_MAX_TX_POWER                 4                       /*!< Maximum supported TX power level, in dBm */
#endif

/*** Scheduler ***/

#ifndef SCH_SETUP_DELAY
#define SCH_SETUP_DELAY                 100     /*!< Setup delay for initial operation in BB ticks. */
#endif

/*** Baseband ***/

#ifndef BB_FIXED_PKT_LEN
#define BB_FIXED_PKT_LEN                0       /*!< Fixed packet length required for BB receive buffers (0 to disable). */
#endif

#ifndef BB_INTER_ADV_DELAY
#define BB_INTER_ADV_DELAY              320     /*! Delay between advertising channel operations. (320 = 200 us) */
#endif

#ifndef BB_SLEEP_CLK_ACCURACY
#define BB_SLEEP_CLK_ACCURACY           200     /*! Sleep clock accuracy in PPM units. */
#endif

#ifdef __cplusplus
};
#endif

#endif /* CFG_MAC_H */
