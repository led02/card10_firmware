/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer HCI subsystem API.
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

#ifndef LHCI_API_H
#define LHCI_API_H

#include "wsf_types.h"
#include "wsf_os.h"
#include "cfg_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      HCI header */
typedef struct
{
  uint16_t      opCode;                 /*!< OpCode command field. */
  uint8_t       len;                    /*!< Parameter length. */
} LhciHdr_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LhciAdvSlaveInit(void);
void LhciScanMasterInit(void);
void LhciDataInit(void);
void LhciHandlerInit(wsfHandlerId_t handlerId);
void LhciHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/* Data exchange */
void LhciCmdRecv(uint8_t *pBuf);
void LhciAclRecv(uint8_t *pBuf);

/* Command processing */
uint8_t LhciPackEvtHdr(uint8_t *pBuf, uint8_t evtCode, uint8_t paramLen);
uint8_t LhciPackCmdStatusEvt(uint8_t *pBuf, uint8_t status, uint16_t opCode);
uint8_t LhciPackCmdCompleteEvt(uint8_t *pBuf, uint16_t opCode);
uint8_t LhciPackCmdCompleteEvtStatus(uint8_t *pBuf, uint8_t status);
void LhciSendEvent(uint8_t *pBuf);

#ifdef __cplusplus
};
#endif

#endif /* LHCI_API_H */
