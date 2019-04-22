/*************************************************************************************************/
/*!
 *  \file   wsf_os.h
 *
 *  \brief  Software foundation OS API.
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
#ifndef WSF_OS_H
#define WSF_OS_H

#include "wsf_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Configuration
**************************************************************************************************/

/* OS Diagnostics */
#ifndef WSF_OS_DIAG
#define WSF_OS_DIAG                             FALSE
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Derive task from handler ID */
#define WSF_TASK_FROM_ID(handlerID)       (((handlerID) >> 4) & 0x0F)

/* Derive handler from handler ID */
#define WSF_HANDLER_FROM_ID(handlerID)    ((handlerID) & 0x0F)

/* Invalid Task Identifier */
#define WSF_INVALID_TASK_ID                     0xFF

#if WSF_OS_DIAG == TRUE
#define WSF_OS_GET_ACTIVE_HANDLER_ID()          WsfActiveHandler
#else
#define WSF_OS_GET_ACTIVE_HANDLER_ID()          WSF_INVALID_TASK_ID
#endif /* WSF_OS_DIAG */

/* Task events */
#define WSF_MSG_QUEUE_EVENT   0x01        /* Message queued for event handler */
#define WSF_TIMER_EVENT       0x02        /* Timer expired for event handler */
#define WSF_HANDLER_EVENT     0x04        /* Event set for event handler */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Event handler ID data type */
typedef uint8_t  wsfHandlerId_t;

/* Event handler event mask data type */
typedef uint8_t  wsfEventMask_t;

/* Task ID data type */
typedef wsfHandlerId_t  wsfTaskId_t;

/* Task event mask data type */
typedef uint8_t wsfTaskEvent_t;

/**************************************************************************************************
  External Variables
**************************************************************************************************/

/* Diagnostic Task Identifier */
extern wsfHandlerId_t WsfActiveHandler;

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Common message structure passed to event handler */
typedef struct
{
  uint16_t        param;          /*! General purpose parameter passed to event handler */
  uint8_t         event;          /*! General purpose event value passed to event handler */
  uint8_t         status;         /*! General purpose status value passed to event handler */
} wsfMsgHdr_t;

/**************************************************************************************************
  Callback Function Types
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     wsfEventHandler_t
 *
 *  \brief  Event handler callback function.
 *
 *  \param  event    Mask of events set for the event handler.
 *  \param  pMsg     Pointer to message for the event handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*wsfEventHandler_t)(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfSetEvent
 *
 *  \brief  Set an event for an event handler.
 *
 *  \param  handlerId   Handler ID.
 *  \param  event       Event or events to set.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSetEvent(wsfHandlerId_t handlerId, wsfEventMask_t event);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskLock
 *
 *  \brief  Lock task scheduling.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTaskLock(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskUnlock
 *
 *  \brief  Unlock task scheduling.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTaskUnlock(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskSetReady
 *
 *  \brief  Set the task used by the given handler as ready to run.
 *
 *  \param  handlerId   Event handler ID.
 *  \param  event       Task event mask.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTaskSetReady(wsfHandlerId_t handlerId, wsfTaskEvent_t event);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskMsgQueue
 *
 *  \brief  Return the task message queue used by the given handler.
 *
 *  \param  handlerId   Event handler ID.
 *
 *  \return Task message queue.
 */
/*************************************************************************************************/
wsfQueue_t *WsfTaskMsgQueue(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \fn     WsfOsSetNextHandler
 *
 *  \brief  Set the next WSF handler function in the WSF OS handler array.  This function
 *          should only be called as part of the OS initialization procedure.
 *
 *  \param  handler    WSF handler function.
 *
 *  \return WSF handler ID for this handler.
 */
/*************************************************************************************************/
wsfHandlerId_t WsfOsSetNextHandler(wsfEventHandler_t handler);

/*************************************************************************************************/
/*!
 *  \fn     wsfOsReadyToSleep
 *
 *  \brief  Check if WSF is ready to sleep.
 *
 *  \param  None.
 *
 *  \return Return TRUE if there are no pending WSF task events set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t wsfOsReadyToSleep(void);

/*************************************************************************************************/
/*!
 *  \fn     wsfOsDispatcher
 *
 *  \brief  Event dispatched.  Designed to be called repeatedly from infinite loop.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void wsfOsDispatcher(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfOsShutdown
 *
 *  \brief  Shutdown OS.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfOsShutdown(void);

#ifdef __cplusplus
};
#endif

#endif /* WSF_OS_H */
