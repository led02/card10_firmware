/*************************************************************************************************/
/*!
 *  \file   wsf_cs.c
 *
 *  \brief  Software foundation OS main module.
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

#include "wsf_types.h"
#include "wsf_cs.h"
#include "wsf_assert.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief  Critical section nesting level. */
uint8_t wsfCsNesting = 0;

#if (WSF_CS_STATS == TRUE)

/*! \brief  Critical section entrance callback. */
WsfCsTimestamp_t wsfCsTimestampCback = NULL;

/*! \brief  Critical section exit callback. */
WsfCsTimebase_t wsfCsTimebaseCback = NULL;

/*! \brief      Critical section start time. */
static uint32_t wsfCsStatsStartTime = 0;

/*! \brief      Critical section start time valid. */
static bool_t wsfCsStatsStartTimeValid = FALSE;

#endif

#if (WSF_CS_STATS == TRUE)

/*************************************************************************************************/
/*!
 *  \fn     WsfCsStatsRegister
 *
 *  \brief  Register critical section statistics hooks.
 *
 *  \param  timestampCback  Callback for obtaining the current timestamp.
 *  \param  timebaseCback   Callback for converting timestamp delta into microseconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsStatsRegister(WsfCsTimestamp_t timestampCback, WsfCsTimebase_t timebaseCback)
{
  WSF_ASSERT(timestampCback != NULL);
  WSF_ASSERT(timebaseCback != NULL);

  wsfCsTimestampCback = timestampCback;
  wsfCsTimebaseCback = timebaseCback;
  wsfCsStatsWatermarkUsec = 0;
}

/*************************************************************************************************/
/*!
 *  \brief  Mark the beginning of a CS.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void wsfCsStatsEnter(void)
{
  /* N.B. Code path must not use critical sections. */

  if (wsfCsTimestampCback)
  {
    wsfCsStatsStartTimeValid = wsfCsTimestampCback(&wsfCsStatsStartTime);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Record the CS watermark.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void wsfCsStatsExit(void)
{
  /* N.B. Code path must not use critical sections. */

  if (wsfCsStatsStartTimeValid != TRUE)
  {
    return;
  }

  /* Valid wsfCsStatsStartTimeValid assumes valid WsfCsStatsRegister(). */
  WSF_ASSERT(wsfCsTimestampCback != NULL);
  WSF_ASSERT(wsfCsTimebaseCback != NULL);

  uint32_t exitTime;

  if (wsfCsTimestampCback(&exitTime))
  {
    uint32_t durUsec = wsfCsTimebaseCback(exitTime - wsfCsStatsStartTime);
    if (durUsec > wsfCsStatsWatermarkUsec)
    {
      wsfCsStatsWatermarkUsec = durUsec;
    }
  }
}

#endif

/*************************************************************************************************/
/*!
 *  \fn     WsfCsEnter
 *
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsEnter(void)
{
  if (wsfCsNesting == 0)
  {
#ifdef __IAR_SYSTEMS_ICC__
    __disable_interrupt();
#endif
#ifdef __GNUC__
    __asm volatile ("cpsid i");
#endif
#ifdef __CC_ARM
    __disable_irq();
#endif

#if (WSF_CS_STATS == TRUE)
    wsfCsStatsEnter();
#endif
  }
  wsfCsNesting++;
}

/*************************************************************************************************/
/*!
 *  \fn     WsfCsExit
 *
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfCsExit(void)
{
  WSF_ASSERT(wsfCsNesting != 0);

  wsfCsNesting--;
  if (wsfCsNesting == 0)
  {
#if (WSF_CS_STATS == TRUE)
    wsfCsStatsExit();
#endif

#ifdef __IAR_SYSTEMS_ICC__
    __enable_interrupt();
#endif
#ifdef __GNUC__
    __asm volatile ("cpsie i");
#endif
#ifdef __CC_ARM
      __enable_irq();
#endif
  }
}
