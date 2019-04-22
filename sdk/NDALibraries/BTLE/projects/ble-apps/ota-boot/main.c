/*************************************************************************************************/
/*!
 *  \file   main.c
 *
 *  \brief  Main file for OTA bootloader.
 *
 *  Copyright (c) 2015-2017 ARM Ltd. All Rights Reserved.
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
#include "ota-relocate-obj.h"
#include "cordio_sdk_version.h"

#include "wsf_buf.h"
#include "wsf_math.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include "util/crc32.h"

#include "cordio_tc2.h"
#include "cordio_tc2_defs.h"

#include "board.h"
#include "board_flash.h"

#include "platform.h"

#include "ota_api.h"
#include "ota_common.h"

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize the platform.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void mainPlatformInit(void)
{
  BoardHwInit();
}

/*************************************************************************************************/
/*!
 *  \brief      Entry point for demo software.
 *
 *  \param      None.
 *
 *  \return     None.
 */
/*************************************************************************************************/
int main(void)
{
  /* ---------- init ---------- */

  WsfTraceEnable(TRUE);
  BoardWsfInit();
  mainPlatformInit();

  /* ---------- OTA load ---------- */

  OtaInit();
  if (OtaNewFileAvailable())
  {
    APP_TRACE_INFO0("ota: copying image from data to run");
    if (!OtaCopyDownImage())
    {
      APP_TRACE_INFO0("ota: image copy failed");
      if (!OtaCheckExistingFileCrc())
      {
        APP_TRACE_INFO0("ota: existing file CRC failed");
        goto Error;
      }
    }
  }
  else
  {
    if (!OtaCheckExistingFileCrc())
    {
      APP_TRACE_INFO0("ota: existing file CRC failed");
      goto Error;
    }
  }

  /* ---------- boot ---------- */

  /* Load image; will not return. */
  CordioTc2ImgHdr_t hdr;

  BoardFlashWake();
  BoardFlashRead(OTA_ROM_SEG_RUN, (uint8_t *)&hdr, sizeof(hdr));
  BoardFlashSleep();

  if (hdr.cookie != CORDIO_TC2_HEADER_COOKIE)
  {
    APP_TRACE_INFO2("img hdr: expected %08X, found %08X", CORDIO_TC2_HEADER_COOKIE, hdr.cookie);
    goto Error;
  }
  if (hdr.type != CORDIO_TC2_IMG_TYPE_HOST_FW)
  {
    APP_TRACE_INFO2("img type: expected %02X, found %02X", CORDIO_TC2_IMG_TYPE_HOST_FW, hdr.type);
    goto Error;
  }
  APP_TRACE_INFO2("img load: length %u, addr %08X", hdr.dataLen & CORDIO_TC2_DATA_LEN_MASK, OTA_ROM_SEG_RUN + hdr.hdrLen);

  ota_relocate_fnc(0x20000000, OTA_ROM_SEG_RUN + hdr.hdrLen, hdr.dataLen & CORDIO_TC2_DATA_LEN_MASK);

  /* ---------- error --------- */

  /* No valid image; blink LED for error. */

Error:
  while (TRUE)
  {

  }
}
