/*************************************************************************************************/
/*!
 *  \file   main.c
 *
 *  \brief  Main file for watch application.
 *
 *  Copyright (c) 2015-2018 Arm Ltd. All Rights Reserved.
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
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "wsf_assert.h"
#include "wsf_buf.h"
#include "wsf_cs.h"
#include "wsf_timer.h"

#include "sec_api.h"
#include "hci_handler.h"
#include "dm_handler.h"
#include "l2c_handler.h"
#include "att_handler.h"
#include "smp_handler.h"
#include "l2c_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "app_api.h"
#include "hci_core.h"
#include "sec_api.h"

#ifdef USE_EXACTLE
#include "ll_init_api.h"
#endif

#include "platform_api.h"
#include "platform_ble_api.h"

#include "watch/watch_api.h"
#include "app_ui.h"

#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief  Pool runtime configuration. */
static wsfBufPoolDesc_t mainPoolDesc[] =
{
  { 16,              8 },
  { 32,              4 },
  { 192,             8 },
  { 256,             8 }
};

#ifdef USE_EXACTLE
  static LlRtCfg_t mainLlRtCfg;
#endif

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInitWatch(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize WSF.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void mainWsfInit(void)
{
#ifdef USE_EXACTLE
  /* +12 for message headroom, + 2 event header, +255 maximum parameter length. */
  const uint16_t maxRptBufSize = 12 + 2 + 255;

  /* +12 for message headroom, +4 for header. */
  const uint16_t aclBufSize = 12 + mainLlRtCfg.maxAclLen + 4 + BB_DATA_PDU_TAILROOM;

  /* Adjust buffer allocation based on platform configuration. */
  mainPoolDesc[2].len = maxRptBufSize;
  mainPoolDesc[2].num = mainLlRtCfg.maxAdvReports;
  mainPoolDesc[3].len = aclBufSize;
  mainPoolDesc[3].num = mainLlRtCfg.numTxBufs + mainLlRtCfg.numRxBufs;
#endif

  const uint8_t numPools = sizeof(mainPoolDesc) / sizeof(mainPoolDesc[0]);

  uint16_t memUsed;
  memUsed = WsfBufInit(PlatformGetHeapAvailable(), PlatformGetHeapStart(), numPools, mainPoolDesc);
  PlatformReserveHeap(memUsed);

  WsfTimerInit();
  WsfTraceEnable(TRUE);
  #if (WSF_TOKEN_ENABLED == TRUE)
    WsfTokenRegisterHandler(LhciVsEncodeTraceMsgEvtPkt);
  #endif
}

/*************************************************************************************************/
/*!
*  \fn     main
*
*  \brief  Entry point for demo software.
*
*  \param  None.
*
*  \return None.
*/
/*************************************************************************************************/
int main(void)
{
#ifdef USE_EXACTLE
  /* Configurations must be persistent. */
  static BbRtCfg_t mainBbRtCfg;

  PlatformInitControllerHardware();
  PlatformLoadBbConfig((PlatformBbCfg_t *)&mainBbRtCfg);
  LlGetDefaultRunTimeCfg(&mainLlRtCfg);
  PlatformLoadLlConfig((PlatformLlCfg_t *)&mainLlRtCfg.maxAdvSets);
#endif

  PlatformInitHostHardware();
  PlatformInit();

  mainWsfInit();

#ifdef USE_EXACTLE
  LlInitRtCfg_t llCfg =
  {
    .pBbRtCfg     = &mainBbRtCfg,
    .wlSizeCfg    = 4,
    .rlSizeCfg    = 4,
    .plSizeCfg    = 4,
    .pLlRtCfg     = &mainLlRtCfg,
    .pFreeMem     = PlatformGetHeapStart(),
    .freeMemAvail = PlatformGetHeapAvailable()
  };

  uint32_t memUsed;
  memUsed = LlInitStdInit(&llCfg);
  PlatformReserveHeap(memUsed);

  bdAddr_t bdAddr;
  PlatformLoadBdAddress(bdAddr);
  LlSetBdAddr((uint8_t *)&bdAddr);
  LlMathSetSeed((uint32_t *)&bdAddr);
#else
  PlatformInitDualChip();
#endif

  StackInitWatch();
  WatchStart();

  bool_t tracePending = FALSE;

  while (TRUE)
  {
    PlatformTimeUpdate();

    wsfOsDispatcher();

#if WSF_TOKEN_ENABLED == TRUE
    /* Allow only a single token to be processed at a time. */
    tracePending = WsfTokenService();
#endif

    if (!tracePending)
    {
      PlatformTimeSleep();
    }
  }
}
