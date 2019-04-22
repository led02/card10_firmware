/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Baseband interface file.
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
 *  \addtogroup BB_API              Baseband (BB) API
 *  \{
 *
 *  The baseband porting layer is a protocol independent BB + radio abstraction layer. It allows
 *  the simultaneous operation of protocol specific schedulers to transport packets across each
 *  medium via a single multi-protocol baseband. This interface describes operations for the
 *  following protocols:
 *
 *    - Bluetooth low energy: advertising and connections
 *    - ZigBee/802.15.4 (TBD)
 *    - BAN/802.15.6 (TBD)
 *
 *  \dot
 *  digraph
 *  {
 *    node [shape=record, width=1.0, height=0.5, fontname=Helvetica, fontsize=8];
 *
 *    ble    [label="BLE\nlink layer", style=filled, fillcolor=lightgray];
 *    zigbee [label="802.15.4\nMAC", style=filled, fillcolor=lightgray];
 *    ban    [label="802.15.6\nMAC", style=filled, fillcolor=lightgray];
 *    sch    [label="Multi-protocol\nscheduler", style=filled, fillcolor=lightgray];
 *    bb_drv [label="{ BB porting layer | BB specific driver }"];
 *    bb_hw  [label="BB hardware", style=filled, fillcolor=dimgray];
 *
 *    ble -> sch;
 *    ble -> bb_drv;
 *    zigbee -> sch;
 *    zigbee -> bb_drv;
 *    ban -> sch;
 *    ban -> bb_drv;
 *    sch -> bb_drv;
 *    bb_drv -> bb_hw [style=dashed];
 *  }
 *  \enddot
 *
 *  \defgroup BB_API_INIT           Initialization
 *  \defgroup BB_API_BOD            Baseband Operation Descriptors (BOD)
 *  \defgroup BB_API_CTRL           Control interface
 *  \defgroup BB_API_TIME           Time interface
 *  \defgroup BB_API_BLE            Bluetooth LE Protocol
 *
 *  \}
 */
/*************************************************************************************************/

#ifndef BB_API_H
#define BB_API_H

#include "wsf_types.h"
#include "cfg_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \addtogroup BB_API_BOD
 *  \{ */

/*! \brief      Protocol types */
enum
{
  BB_PROT_NONE,                     /*!< Non-protocol specific operation. */
  BB_PROT_BLE,                      /*!< Bluetooth Low Energy. */
  BB_PROT_PRBS15,                   /*!< Enable the continuous PRBS15 transmit sequence. */
};

/*! \brief      PHY codes */
enum
{
  BB_PHY_1M = 1,                        /*!< 1M PHY. */
  BB_PHY_2M = 2,                        /*!< 2M PHY. */
  BB_PHY_S8 = 3,                        /*!< Coded S8 PHY. */
  BB_PHY_S2 = 4,                        /*!< Coded S2 PHY. */
};

/*! \brief      Status codes */
enum
{
  BB_STATUS_SUCCESS,                /*!< Operation successful. */
  BB_STATUS_FAILED,                 /*!< General failure. */
  BB_STATUS_RX_TIMEOUT,             /*!< Receive packet timeout. */
  BB_STATUS_CRC_FAILED,             /*!< Receive packet with CRC verification failed. */
  BB_STATUS_DECRYPT_FAILED,         /*!< Receive packet with decryption verification failed. */
};

/*! \} */    /* BB_API_BOD */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \addtogroup BB_API_BOD
 *  \{ */

/* Forward definition of BOD. */
struct BbOpDesc_tag;

/*! \brief      Event type. */
enum
{
  BB_EVENT_BEGIN_BOD,               /*!< Beginning of BOD event. */
  BB_EVENT_END_BOD                  /*!< End of BOD event. */
};

/*! \brief      BB event callback signature. */
typedef void (*BbEventCback_t)(struct BbOpDesc_tag *pBod, uint8_t event);

/*! \brief      BOD operation callback signature. */
typedef void (*BbBodCback_t)(struct BbOpDesc_tag *pBod);

/*! \} */    /* BB_API_BOD */

/*! \addtogroup BB_API_BOD
 *  \{ */

/* Forward protocol data definitions. */
struct BbBleData_tag;

/*! \brief      Baseband operation descriptor (BOD). */
typedef struct BbOpDesc_tag
{
  struct BbOpDesc_tag *pPrev;       /*!< Previous BOD. */
  struct BbOpDesc_tag *pNext;       /*!< Next BOD. */

  uint32_t      due;                /*!< Absolute clock due time. */
  uint32_t      interval;           /*!< Interval assigned by scheduler. */

  uint32_t      minDuration;        /*!< Minimum duration of operation. */
  uint32_t      maxDuration;        /*!< Maximum duration of operation. */
  uint32_t      duration;           /*!< duration allocated by scheduler. */

  uint8_t       protId;             /*!< Protocol type. */
  uint8_t       phy;                /*!< PHY to use for this operation. */

  uint8_t       priority;           /*!< Priority of operation for scheduling. */
  uint8_t       basePriority;       /*!< Base priority of operation for scheduling. */

  BbBodCback_t  beginCback;         /*!< Beginning of BOD callback. */
  BbBodCback_t  preemptCback;       /*!< Preempt BOD callback. */
  BbBodCback_t  endCback;           /*!< End of BOD callback. */

  void          *pCtx;              /*!< Client defined context. */

  union
  {
    struct BbBleData_tag *pBle;     /*!< BLE operation data. */
  } prot;                           /*!< Protocol specific data. */
} BbOpDesc_t;

/*! \} */    /* BB_API_BOD */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup BB_API_INIT
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BB.
 *
 *  \return     None.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void BbInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Register operation completion handler.
 *
 *  \param      eventCback  Event callback.
 *
 *  \return     None.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void BbRegister(BbEventCback_t eventCback);

/*! \} */    /* BB_API_INIT */

/*! \addtogroup BB_API_CTRL
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 *
 *  Enable BB and start processing the list of BODs.
 */
/*************************************************************************************************/
void BbStart(uint8_t protId);

/*************************************************************************************************/
/*!
 *  \brief      Stop BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  Disable BB processing of BODs.
 */
/*************************************************************************************************/
void BbStop(uint8_t protId);

/*************************************************************************************************/
/*!
 *  \brief      Execute BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *
 *  \return     None.
 *
 *  Execute the protocol specific BOD handler.
 */
/*************************************************************************************************/
void BbExecuteBod(BbOpDesc_t *pBod);

/*************************************************************************************************/
/*!
 *  \brief      Preempt BOD.
 *
 *  \param      pBod    Pointer to the BOD to preempt.
 *
 *  \return     None.
 *
 *  Preempt a BOD that has been started but the due time has not yet occurred.
 */
/*************************************************************************************************/
void BbPreemptBod(BbOpDesc_t *pBod);

/*************************************************************************************************/
/*!
 *  \brief      Cancel current executing BOD.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the execution context of the
 *              current executing BOD.
 */
/*************************************************************************************************/
void BbCancelBod(void);

/*************************************************************************************************/
/*!
 *  \brief      Get the amount of time until the current operation is due.
 *
 *  \return     Tick count until the current operation is due. Returns maximum value if no
 *              operation is in progress. 
 */
/*************************************************************************************************/
uint32_t BbGetTimeUntilNextDue(void);

/*! \} */    /* BB_API_CTRL */

#ifdef __cplusplus
};
#endif

#endif /* BB_API_H */
