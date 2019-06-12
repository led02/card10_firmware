/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Multi-protocol scheduler interface file.
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

#ifndef SCH_API_H
#define SCH_API_H

#include "wsf_types.h"
#include "wsf_os.h"
#include "bb_api.h"
#include "cfg_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Minimum time between operations in BB ticks. */
#define SCH_INTER_OP_GAP            (2 * SCH_SETUP_DELAY)

/*! \brief      Time needed for operation post-processing. */
#define SCH_OP_PROC_TICKS           130

#define SCH_NUM_OFFSETS             6

/*! \brief      Maximum highest priority of a BOD. */
#define SCH_MAX_BOD_PRIORITY        0xFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef struct
{
  uint32_t interval;
  uint32_t periodicity;
  uint32_t offset[SCH_NUM_OFFSETS];
  uint32_t offsetCount;
} SchParams_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void SchInit(wsfHandlerId_t handlerId);
void SchHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/* Operation scheduling */
bool_t SchScheduleOperation(BbOpDesc_t *pBod, uint32_t minInterval, uint32_t maxInterval);
uint32_t SchSelectInterval(BbOpDesc_t *pBod, uint32_t minInterval, uint32_t maxInterval, uint32_t *periodicity);
bool_t SchGetParams(BbOpDesc_t *pBod, uint32_t minInterval, uint32_t maxInterval, SchParams_t *pSchParams);
bool_t SchCheckOffsets(BbOpDesc_t *pBod, SchParams_t *pSchParams);
uint32_t SchGetBandwidth(BbOpDesc_t *pBod, uint32_t bodInterval);
void SchSetNextDue(BbOpDesc_t *pBod);
void SchSetNextDueAfter(BbOpDesc_t *pBod, uint32_t earliestStart);

/* List maintenance */
void SchInsertNextAvailable(BbOpDesc_t *pBod);
void SchInsertNextAvailableAfter(BbOpDesc_t *pBod, uint32_t earliestStart);
void SchInsertNextAvailableInterval(BbOpDesc_t *pBod);
bool_t SchInsertAtDueTime(BbOpDesc_t *pBod);
bool_t SchInsertLateAsPossible(BbOpDesc_t *pBod, uint32_t min, uint32_t max);
bool_t SchSearchRemove(BbOpDesc_t *pBod);
void SchRemove(BbOpDesc_t *pBod);

/*************************************************************************************************/
/*!
 *  \brief      Compare two times
 *
 *  \param      time1   First time
 *  \param      time2   Second time
 *
 *  \return     TRUE if time1 is earlier than time2
 */
/*************************************************************************************************/
static inline bool_t schIsEarlier(uint32_t time1, uint32_t time2)
{
  return ((int)(time2 - time1) > 0);
}

/*************************************************************************************************/
/*!
 *  \brief      Increase the priority of a BOD.
 *
 *  \param      pBod    Pointer to the BOD.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static inline void schPriorityIncr(BbOpDesc_t *pBod)
{
  if (pBod->priority < 0xFF) {
    pBod->priority++;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Reset the priority of a BOD to its base priority.
 *
 *  \param      pBod    Pointer to the BOD.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static inline void schPriorityReset(BbOpDesc_t *pBod)
{
  pBod->priority = pBod->basePriority;
}

#ifdef __cplusplus
};
#endif

#endif /* SCH_API_H */
