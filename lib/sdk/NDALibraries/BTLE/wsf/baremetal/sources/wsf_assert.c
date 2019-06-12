/*************************************************************************************************/
/*!
 *  \file   wsf_assert.c
 *
 *  \brief  Assert implementation.
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
#include "wsf_assert.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief  Assertion callback. */
static void (*wsfAssertCback)(void) = NULL;

/*************************************************************************************************/
/*!
 *  \def    WsfAssert
 *
 *  \brief  Perform an assert action.
 *
 *  \param  pFile   Name of file originating assert.
 *  \param  line    Line number of assert statement.
 *
 *  \return None.
 */
/*************************************************************************************************/
#if WSF_TOKEN_ENABLED == TRUE
void WsfAssert(uint16_t modId, uint16_t line)
#else
void WsfAssert(const char *pFile, uint16_t line)
#endif
{
#if WSF_TOKEN_ENABLED == TRUE
  WSF_TRACE_ERR2("Assertion detected on %s:%u", modId, line);
#else
  WSF_TRACE_ERR2("Assertion detected on %s:%u", pFile, line);
#endif

  if (wsfAssertCback)
  {
    wsfAssertCback();
  }
  else
  {
    while (TRUE);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Register assert handler.
 *
 *  \param  cback   Callback called upon assert condition.
 */
/*************************************************************************************************/
void WsfAssertRegister(void (*cback)(void))
{
  wsfAssertCback = cback;
}
