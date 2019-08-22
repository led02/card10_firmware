/*******************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2019-07-12 13:08:01 -0500 (Fri, 12 Jul 2019) $
 * $Revision: 44610 $
 *
 ******************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief Controller HCI interface to the Host
 */
/*************************************************************************************************/

#include <stdio.h>
#include "mxc_config.h"
#include "uart.h"
#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "util/bstream.h"
#include "hci_api.h"
#include "hci_core.h"
#include "hci_drv.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define HCI_UART_REG            MXC_UART_GET_UART(HCI_UART)

/*************************************************************************************************/
void hciTrSerialRxIncoming(uint8_t *pBuf, uint8_t len)
{
    int error;

    error = UART_Write(HCI_UART_REG, pBuf, len);
    if(error != len) {
        printf("Error writing to HCI_UART\n");
    }
}

/*************************************************************************************************/
void hciTrSendCmd(uint8_t *pData)
{
  uint16_t   len;

  /* get length */
  len = pData[2] + HCI_CMD_HDR_LEN;

  /* dump event for protocol analysis */
  HCI_PDUMP_CMD(len, pData);

  /* transmit ACL header and data */
  if (hciDrvWrite(HCI_CMD_TYPE, len, pData) == len)
  {
    /* free buffer */
    WsfMsgFree(pData);
  }
}
