/*************************************************************************************************/
/*!
 *  \file   wsf_trace.c
 *
 *  \brief  Trace message implementation.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *  Arm Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Arm Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "wsf_cs.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef WSF_RING_BUF_SIZE
/*! \brief      Size of token ring buffer (multiple of 2^N). */
#define WSF_RING_BUF_SIZE               32
#endif

/*! \brief      Ring buffer flow control condition detected. */
#define WSF_TOKEN_FLAG_FLOW_CTRL        (1 << 28)

/**************************************************************************************************
  Data types
**************************************************************************************************/

/*! \brief      Trace control block. */
struct
{
  struct
  {
    uint32_t token;             /*!< Token. */
    uint32_t param;             /*!< Parameter. */
  } ringBuf[WSF_RING_BUF_SIZE]; /*!< Tokenized tracing ring buffer. */

  volatile uint32_t prodIdx;    /*!< Ring buffer producer index. */
  volatile uint32_t consIdx;    /*!< Ring buffer consumer index. */

  WsfTokenHandler_t tokenCback; /*!< Token event callback. */

  bool_t ringBufEmpty;          /*!< Ring buffer state. */

  bool_t enabled;               /*!< Tracing state. */

  wsfTraceCback_t cback;        /*!< Platform trace callback */
} wsfTraceCb;

#if WSF_TRACE_ENABLED == TRUE

/*************************************************************************************************/
/*!
 *  \brief  Print a trace message.
 *
 *  \param  pStr      Message format string
 *  \param  ...       Additional arguments, printf-style
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTrace(const char *pStr, ...)
{
  va_list           args;

  va_start(args, pStr);
  if (wsfTraceCb.cback)
  {
    (*wsfTraceCb.cback)(pStr, args);
  }
  va_end(args);
}

#endif

/*************************************************************************************************/
/*!
 *  \brief  Register platform trace callback function.
 *
 *  \param  cback    Callback function
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTraceRegister(wsfTraceCback_t cback)
{
  wsfTraceCb.cback = cback;
}

/*************************************************************************************************/
/*!
 *  \brief  Output tokenized message.
 *
 *  \param  tok       Token
 *  \param  var       Variable
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfToken(uint32_t tok, uint32_t var)
{
  static uint32_t flags = 0;

  if (!wsfTraceCb.enabled)
  {
    return;
  }

  WSF_CS_INIT(cs);
  WSF_CS_ENTER(cs);

  uint32_t prodIdx = (wsfTraceCb.prodIdx + 1) & (WSF_RING_BUF_SIZE - 1);

  if (prodIdx != wsfTraceCb.consIdx)
  {
    wsfTraceCb.ringBuf[wsfTraceCb.prodIdx].token = tok | flags;
    wsfTraceCb.ringBuf[wsfTraceCb.prodIdx].param = var;
    wsfTraceCb.prodIdx = prodIdx;
    flags = 0;
  }
  else
  {
    flags = WSF_TOKEN_FLAG_FLOW_CTRL;
  }

  WSF_CS_EXIT(cs);
}

/*************************************************************************************************/
/*!
 *  \brief  Enable trace messages.
 *
 *  \param  enable    TRUE to enable, FALSE to disable
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTraceEnable(bool_t enable)
{
#if WSF_TOKEN_ENABLED == TRUE
  wsfTraceCb.enabled = enable;
#endif
}

/*************************************************************************************************/
/*!
 *  \brief  Service the trace ring buffer.
 *
 *  \return TRUE if trace messages pending, FALSE otherwise.
 *
 *  This routine is called in the main loop for a "push" type trace systems.
 */
/*************************************************************************************************/
bool_t WsfTokenService(void)
{
  if (wsfTraceCb.tokenCback == NULL)
  {
    return FALSE;
  }

  if (wsfTraceCb.consIdx != wsfTraceCb.prodIdx)
  {
    wsfTraceCb.tokenCback((uint8_t *)&wsfTraceCb.ringBuf[wsfTraceCb.consIdx], sizeof(wsfTraceCb.ringBuf[0]));
    wsfTraceCb.consIdx = (wsfTraceCb.consIdx + 1) & (WSF_RING_BUF_SIZE - 1);
    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Regsiter token handler.
 *
 *  \param  tokenCback  Token event handler.
 *
 *  \return None.
 *
 *  This routine registers a token callback. This callback is called when the next token event
 *  is ready to be written to the I/O.
 */
/*************************************************************************************************/
void WsfTokenRegisterHandler(WsfTokenHandler_t tokenCback)
{
  wsfTraceCb.tokenCback = tokenCback;
}
