/*************************************************************************************************/
/*!
 *  \file   main.c
 *
 *  \brief  Main module.
 *
 *          $Date: 2019-02-11 23:36:35 +0000 (Mon, 11 Feb 2019) $
 *          $Revision: 40967 $
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

#include <stdio.h>
#include <string.h>
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "ll_api.h"
#include "sch_api.h"
#include "scanner_api.h"
#include "mxc_config.h"
#include "gcr_regs.h"
#include "mcr_regs.h"
#include "app_config.h"
#include "hci_core.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#ifdef USE_EXTENDED_ADV
#define WSF_BUF_POOLS              7
#else /* USE_EXTENDED_ADV */
#define WSF_BUF_POOLS              6
#endif /* USE_EXTENDED_ADV */

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE 128

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

/*! Free memory for pool buffers (use word elements for word alignment). */
static uint32_t mainBufMem[3584/sizeof(uint32_t)+96];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
#ifdef USE_EXTENDED_ADV
  { 384,  3 },
  { 512,  1 }
#else /* USE_EXTENDED_ADV */
  { 384,  4 }
#endif /* USE_EXTENDED_ADV */
};

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInitScanner(void);

/*************************************************************************************************/
void SysTick_Handler(void)
{
    WsfTimerUpdate(1);
}

/*************************************************************************************************/
static void myTrace(const char *pStr, va_list args)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        vprintf(pStr, args);
        printf("\r\n");
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize WSF.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void WsfInit(void)
{
    WsfTimerInit();
    WsfBufInit(sizeof(mainBufMem), (uint8_t*)mainBufMem, WSF_BUF_POOLS, mainPoolDesc);
    WsfTraceRegister(myTrace);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize platform.
 *
 *  \param  msPerTick   Milliseconds per timer tick.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlatformInit(void)
{
    /* Change the pullup on the RST pin to 25K */
    MXC_MCR->ctrl = 0x202;

    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Power up the 32MHz XO */
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);  // Clear GPIO0 and GPIO1 Disable
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_BTLED | MXC_F_GCR_PERCKCN1_TRNGD );  // Clear BTLE and ICACHE0 disable

    /* setup the systick */
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

#ifndef __IAR_SYSTEMS_ICC__
    /* Added to prevent printf() and friends from using malloc() */
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);
#endif
}

/*
 * In two-chip solutions, setting the address must wait until the HCI interface is initialized.
 * This handler can also catch other Application events, but none are currently implemented.
 * See ble-profiles/sources/apps/app/common/app_ui.c for further details.
 *
 */
void SetAddress(uint8_t event)
{
    uint8_t bdAddr[6] = {0x02, 0x01, 0x44, 0x8B, 0x05, 0x00};
    
    switch (event) {
	case APP_UI_RESET_CMPL:
	    printf("Setting address -- MAC %02X:%02X:%02X:%02X:%02X:%02X\n", bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1], bdAddr[0]);
	    LlSetBdAddr((uint8_t*)&bdAddr);
		LlGetBdAddr(hciCoreCb.bdAddr);
	    break;
	default:
	    break;
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize MAC layer.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MacInit(void)
{
    wsfHandlerId_t handlerId;

    /* Initialize link layer. */
    BbInit();
    handlerId = WsfOsSetNextHandler(SchHandler);
    SchInit(handlerId);
    LlScanMasterInit();
    handlerId = WsfOsSetNextHandler(LlHandler);
    LlHandlerInit(handlerId);
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
    /* Workaround for Hard Fault caused by memory read of dmEvt_t object allocated on stack */
    volatile uint8_t workaround[100];
    workaround[0] = 0;
    workaround[0];

#ifdef USE_EXTENDED_ADV
    printf("\n\n***** MAX32665 Extended Scanner *****\n");
#else /* USE_EXTENDED_ADV */
    printf("\n\n***** MAX32665 Scanner *****\n");
#endif /* USE_EXTENDED_ADV */

    PlatformInit();
    WsfInit();
    MacInit();
    StackInitScanner();
    ScannerStart();

    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);
    
    printf("Setup Complete\n");

    while (1)
    {
        wsfOsDispatcher();
    }
}

/*****************************************************************/
void HardFault_Handler(void)
{
    printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

    // Loop forever
    while(1);
}
