#include "modules/modules.h"

#include "wsf_types.h"
#include "util/bstream.h"
#include "att_api.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define UART_START_HDL 0x800            /*!< \brief Service start handle. */
#define UART_END_HDL (UART_MAX_HDL - 1) /*!< \brief Service end handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief UART Service Handles */
enum { UART_SVC_HDL = UART_START_HDL, /*!< \brief UART service declaration */
       UART_RX_CH_HDL,                /*!< \brief UART rx characteristic */
       UART_RX_HDL,                   /*!< \brief UART rx value */
       UART_TX_CH_HDL,                /*!< \brief UART tx characteristic */
       UART_TX_HDL,                   /*!< \brief UART tx value */
       UART_TX_CH_CCC_HDL,            /*!< \brief UART tx CCCD */
       UART_MAX_HDL                   /*!< \brief Maximum handle. */
};
/**@}*/

/* clang-format off */
static const uint8_t UARTSvc[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x01,0x00,0x40,0x6E};
static const uint16_t UARTSvc_len = sizeof(UARTSvc);

static const uint8_t uartRxCh[] = {ATT_PROP_WRITE, UINT16_TO_BYTES(UART_RX_HDL), 0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E};
static const uint16_t uartRxCh_len = sizeof(uartRxCh);
static const uint8_t attUartRxChUuid[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5, 0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E};

static const uint8_t uartTxCh[] = {ATT_PROP_READ | ATT_PROP_NOTIFY, UINT16_TO_BYTES(UART_TX_HDL), 0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E};
static const uint16_t uartTxCh_len = sizeof(uartTxCh);
static const uint8_t attUartTxChUuid[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5, 0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E};

static uint8_t uartTxCh_buf[128];
static uint16_t uartTxCh_buf_len = sizeof(uartTxCh_buf);
/* clang-format on */

/* Attribute list for uriCfg group */
static const attsAttr_t uartAttrCfgList[] = {
	/* Primary service */
	{
		.pUuid       = attPrimSvcUuid,
		.pValue      = (uint8_t *)UARTSvc,
		.pLen        = (uint16_t *)&UARTSvc_len,
		.maxLen      = sizeof(UARTSvc),
		.settings    = 0,
		.permissions = ATTS_PERMIT_READ,
	},
	/* UART rx characteristic */
	{
		.pUuid       = attChUuid,
		.pValue      = (uint8_t *)uartRxCh,
		.pLen        = (uint16_t *)&uartRxCh_len,
		.maxLen      = sizeof(uartRxCh),
		.settings    = 0,
		.permissions = ATTS_PERMIT_READ,
	},
	/* UART rx value */
	{
		.pUuid       = attUartRxChUuid,
		.pValue      = NULL,
		.pLen        = NULL,
		.maxLen      = 128,
		.settings    = ATTS_SET_WRITE_CBACK | ATTS_SET_VARIABLE_LEN,
		.permissions = ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
			       ATTS_PERMIT_WRITE_AUTH,
	},
	/* UART tx characteristic */
	{
		.pUuid       = attChUuid,
		.pValue      = (uint8_t *)uartTxCh,
		.pLen        = (uint16_t *)&uartTxCh_len,
		.maxLen      = sizeof(uartTxCh),
		.settings    = 0,
		.permissions = ATTS_PERMIT_READ,
	},
	/* UART tx value */
	/* TODO: do we need ATTS_SET_READ_CBACK ? */
	{
		.pUuid       = attUartTxChUuid,
		.pValue      = uartTxCh_buf,
		.pLen        = &uartTxCh_buf_len,
		.maxLen      = sizeof(uartTxCh_buf),
		.settings    = ATTS_SET_READ_CBACK,
		.permissions = ATTS_PERMIT_READ | ATTS_PERMIT_READ_ENC |
			       ATTS_PERMIT_READ_AUTH,
	},
	/* UART tx CCC descriptor */
	{
		.pUuid       = attCliChCfgUuid,
		.pValue      = NULL,
		.pLen        = NULL,
		.maxLen      = 0,
		.settings    = ATTS_SET_CCC,
		.permissions = ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
			       ATTS_PERMIT_WRITE_AUTH | ATTS_PERMIT_READ |
			       ATTS_PERMIT_READ_ENC | ATTS_PERMIT_READ_AUTH,
	},
};

dmConnId_t active_connection = 0;

static uint8_t UARTReadCback(
	dmConnId_t connId,
	uint16_t handle,
	uint8_t operation,
	uint16_t offset,
	attsAttr_t *pAttr
) {
	printf("read callback\n");
	return ATT_SUCCESS;
}

static uint8_t UARTWriteCback(
	dmConnId_t connId,
	uint16_t handle,
	uint8_t operation,
	uint16_t offset,
	uint16_t len,
	uint8_t *pValue,
	attsAttr_t *pAttr
) {
	active_connection = connId;

	//printf("UARTWriteCback %d: ", len);
	int i;
	for (i = 0; i < len; i++) {
		//printf("%c", pValue[i]);
		serial_enqueue_char(pValue[i]);
	}
	serial_enqueue_char('\r');
	//printf("\n");

#if 0
  AttsSetAttr(UART_TX_HDL, len, pValue);
  AttsHandleValueNtf(connId, UART_TX_HDL, len, pValue);
#endif

	return ATT_SUCCESS;
}

static uint8_t ble_uart_tx_buf[128];
static uint8_t ble_uart_buf_tx_fill;
static int ble_uart_lasttick = 0;

void ble_uart_write(uint8_t *pValue, uint8_t len)
{
	for (int i = 0; i < len; i++) {
		if (pValue[i] >= 0x20 && pValue[i] < 0x7f) {
			ble_uart_tx_buf[ble_uart_buf_tx_fill] = pValue[i];
			ble_uart_buf_tx_fill++;
		}

		if (ble_uart_buf_tx_fill == 128 || pValue[i] == '\r' ||
		    pValue[i] == '\n') {
			if (ble_uart_buf_tx_fill > 0) {
				AttsSetAttr(
					UART_TX_HDL,
					ble_uart_buf_tx_fill,
					ble_uart_tx_buf
				);
				if (active_connection) {
					int x = xTaskGetTickCount() -
						ble_uart_lasttick;
					if (x < 100) {
						/*
						 * TODO: Ugly hack if we already
						 *     send something recently.
						 *     Figure out how fast we
						 *     can send or use indications.
						 */
						vTaskDelay(100 - x);
					}
					AttsHandleValueNtf(
						active_connection,
						UART_TX_HDL,
						ble_uart_buf_tx_fill,
						ble_uart_tx_buf
					);
					ble_uart_lasttick = xTaskGetTickCount();
				}
				ble_uart_buf_tx_fill = 0;
			}
		}
	}
}

static attsGroup_t uartCfgGroup = {
	.pAttr       = (attsAttr_t *)uartAttrCfgList,
	.readCback   = UARTReadCback,
	.writeCback  = UARTWriteCback,
	.startHandle = UART_START_HDL,
	.endHandle   = UART_END_HDL,
};

void bleuart_init(void)
{
	/* Add the UART service */
	AttsAddGroup(&uartCfgGroup);
}
