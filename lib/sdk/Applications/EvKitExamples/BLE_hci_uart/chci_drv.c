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
 * $Date: 2019-06-27 10:54:45 -0700 (Thu, 27 Jun 2019) $
 * $Revision: 44319 $
 *
 ******************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief Controller HCI interface to the Host
 */
/*************************************************************************************************/

#include <stdio.h>
#include "mxc_config.h"
#include "hci_defs.h"
#include "chci_api.h"
#include "chci_tr_serial.h"
#include "uart.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define HCI_UART_REG            MXC_UART_GET_UART(HCI_UART)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Asynchronous UART request for HCI UART */
uart_req_t hci_read_req;
uint8_t hci_data;

/*************************************************************************************************/
void UART0_IRQHandler(void)
{
    UART_Handler(MXC_UART0);
}

/*************************************************************************************************/
void UART1_IRQHandler(void)
{
    UART_Handler(MXC_UART1);
}

/*************************************************************************************************/
void UART2_IRQHandler(void)
{
    UART_Handler(MXC_UART2);
}

/*************************************************************************************************/
void HCI_UART_Handler(uart_req_t* req, int error)
{
    if(error != E_NO_ERROR) {
        printf("Error with UART_ReadAsync callback %d\n", error);
    }

    // Write data to the Controller HCI
    chciTrSerialRxIncoming(&hci_data, 1);

    /* Read next byte */
    UART_ReadAsync(HCI_UART_REG, &hci_read_req);
}

/*************************************************************************************************/
void chciDrvInit(void)
{
	IRQn_Type hci_irq = MXC_UART_GET_IRQ(HCI_UART);

    /* Configure the HCI UART */
    const uart_cfg_t hci_uart_cfg = {
        .parity = UART_PARITY_DISABLE,
        .size   = UART_DATA_SIZE_8_BITS,
        .stop   = UART_STOP_1,
        .flow   = UART_FLOW_CTRL_DIS,
        .pol    = UART_FLOW_POL_DIS,
        .baud   = 115200,
        .clksel = UART_CLKSEL_SYSTEM
    };

    const sys_cfg_uart_t hci_uart_sys_cfg = {
        .map = HCI_UART_MAP,
        .flow = Disable
    };

    UART_Init(HCI_UART_REG, &hci_uart_cfg, &hci_uart_sys_cfg);

    /* Setup Interrupt */
    hci_read_req.data = &hci_data;
    hci_read_req.len = 1;
    hci_read_req.callback = HCI_UART_Handler;
    NVIC_EnableIRQ(hci_irq);

    /* Setup the asycnronous read */
    UART_ReadAsync(HCI_UART_REG, &hci_read_req);
    UART_Enable(HCI_UART_REG);
}

/*************************************************************************************************/
uint16_t chciDrvWrite(uint8_t prot, uint8_t type, uint16_t len, uint8_t *pData)
{
	uint16_t write_len = 0;
    int error;

    // Convert CHCI_TR_TYPE to HCI_TYPE
    uint8_t hciType = (type == CHCI_TR_TYPE_EVT) ? HCI_EVT_TYPE : HCI_ACL_TYPE;

    error = UART_Write(HCI_UART_REG, &hciType, 1);
    if(error == 1) {
        write_len = 1;
    } else {
        printf("Error writing to HCI_UART\n");
        return write_len;
    }

    /* Write out to the Host from the UART */
    error = UART_Write(HCI_UART_REG, pData, len);
    if(error >= 0) {
        write_len += error;
    } else {
        printf("Error writing to HCI_UART\n");
    }

    return write_len;
}
