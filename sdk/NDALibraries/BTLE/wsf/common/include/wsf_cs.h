/*************************************************************************************************/
/*!
 *  \file   wsf_cs.h
 *
 *  \brief  Critical section macros.
 *
 *  Copyright (c) 2009-2017 ARM Ltd. All Rights Reserved.
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
#ifndef WSF_CS_H
#define WSF_CS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef WSF_CS_STATS
/*! \brief  Use CS statistics hooks. */
#define WSF_CS_STATS    FALSE
#endif

/*************************************************************************************************/
/*!
 *  \def    WSF_CS_INIT
 *
 *  \brief  Initialize critical section.  This macro may define a variable.
 *
 *  \param  cs    Critical section variable to be defined.
 */
/*************************************************************************************************/
#define WSF_CS_INIT(cs)

/*************************************************************************************************/
/*!
 *  \def    WSF_CS_ENTER
 *
 *  \brief  Enter a critical section.
 *
 *  \param  cs    Critical section variable.
 */
/*************************************************************************************************/
#define WSF_CS_ENTER(cs)        WsfCsEnter()

/*************************************************************************************************/
/*!
 *  \def    WSF_CS_EXIT
 *
 *  \brief  Exit a critical section.
 *
 *  \param  cs    Critical section variable.
 */
/*************************************************************************************************/
#define WSF_CS_EXIT(cs)        WsfCsExit()

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief  Timestamp call signature. */
typedef bool_t (*WsfCsTimestamp_t)(uint32_t *);

/*! \brief  Timestamp delta and time base converter call signature. */
typedef uint32_t (*WsfCsTimebase_t)(uint32_t);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern uint16_t wsfCsStatsWatermarkUsec;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void WsfCsStatsRegister(WsfCsTimestamp_t timestampCback, WsfCsTimebase_t timebaseCback);
void WsfCsEnter(void);
void WsfCsExit(void);

#ifdef __cplusplus
};
#endif

#endif /* WSF_CS_H */
