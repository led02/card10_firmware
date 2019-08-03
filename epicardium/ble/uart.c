#include "modules/modules.h"

#include "wsf_types.h"
#include "util/bstream.h"
#include "att_api.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define UART_START_HDL                    0x800                /*!< \brief Service start handle. */
#define UART_END_HDL                      (UART_MAX_HDL - 1)  /*!< \brief Service end handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief UART Service Handles */
enum
{
  UART_SVC_HDL = UART_START_HDL,       /*!< \brief UART service declaration */
  UART_RX_CH_HDL,                      /*!< \brief UART rx characteristic */
  UART_RX_HDL,                         /*!< \brief UART rx value */
  UART_TX_CH_HDL,                      /*!< \brief UART tx characteristic */
  UART_TX_HDL,                         /*!< \brief UART tx value */
  UART_TX_CH_CCC_HDL,                  /*!< \brief UART tx CCCD */
  UART_MAX_HDL                         /*!< \brief Maximum handle. */
};
/**@}*/

/* clang-format off */
static const uint8_t UARTSvc[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x01,0x00,0x40,0x6E};

static const uint8_t uartRxCh[] = {ATT_PROP_WRITE, UINT16_TO_BYTES(UART_RX_HDL), 0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E};
const uint8_t attUartRxChUuid[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5, 0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E};

static const uint8_t uartTxCh[] = {ATT_PROP_READ | ATT_PROP_NOTIFY, UINT16_TO_BYTES(UART_TX_HDL), 0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E};
const uint8_t attUartTxChUuid[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5, 0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E};
/* clang-format on */



static void *SvcUARTAddGroupDyn(void)
{
  void *pSHdl;
  uint8_t initCcc[] = {UINT16_TO_BYTES(0x0000)};

  /* Create the service */
  pSHdl = AttsDynCreateGroup(UART_START_HDL, UART_END_HDL);

  if (pSHdl != NULL)
  {
    /* Primary service */
    AttsDynAddAttrConst(pSHdl, attPrimSvcUuid, UARTSvc, sizeof(UARTSvc), 0, ATTS_PERMIT_READ);


    /* UART rx characteristic */
    AttsDynAddAttrConst(pSHdl, attChUuid, uartRxCh, sizeof(uartRxCh), 0, ATTS_PERMIT_READ);
    // XXX: attUartRxChUuid is 16 bytes but nothing says so....
    /* UART rx value */
    // XXX: not sure if max value of 128 is fine...
    AttsDynAddAttr(pSHdl, attUartRxChUuid, NULL, 0, 128,
                   ATTS_SET_WRITE_CBACK | ATTS_SET_VARIABLE_LEN, ATTS_PERMIT_WRITE);


    /* UART tx characteristic */
    AttsDynAddAttrConst(pSHdl, attChUuid, uartTxCh, sizeof(uartTxCh), 0, ATTS_PERMIT_READ);
    /* UART tx value */
    /* TODO: do we need ATTS_SET_READ_CBACK ? */
    AttsDynAddAttr(pSHdl, attUartTxChUuid, NULL, 0, sizeof(uint8_t),
                   ATTS_SET_READ_CBACK, ATTS_PERMIT_READ);
    /* UART tx CCC descriptor */
    AttsDynAddAttr(pSHdl, attCliChCfgUuid, initCcc, sizeof(uint16_t), sizeof(uint16_t),
                   ATTS_SET_CCC, ATTS_PERMIT_READ | ATTS_PERMIT_WRITE);
  }

  return pSHdl;
}

dmConnId_t active_connection = 0;

static uint8_t UARTReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                     uint16_t offset, attsAttr_t *pAttr)
{
  printf("read callback\n");
  return ATT_SUCCESS;
}

static uint8_t UARTWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                          uint16_t offset, uint16_t len, uint8_t *pValue,
                          attsAttr_t *pAttr)
{
  active_connection = connId;

  //printf("UARTWriteCback %d: ", len);
  int i;
  for(i=0; i<len; i++) {
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

uint8_t ble_uart_tx_buf[129];
uint8_t ble_uart_buf_tx_fill;
int ble_uart_lasttick = 0;

void ble_uart_write(uint8_t *pValue, uint8_t len)
{
  int i;
  for(i=0; i<len; i++) {
    if(pValue[i] >= 0x20 && pValue[i] < 0x7f) {
        ble_uart_tx_buf[ble_uart_buf_tx_fill] = pValue[i];
        ble_uart_buf_tx_fill++;
    } else if(pValue[i] == '\r' || pValue[i] == '\n') {
        if(ble_uart_buf_tx_fill > 0) {
            AttsSetAttr(UART_TX_HDL, ble_uart_buf_tx_fill, ble_uart_tx_buf);
            if(active_connection) {
                int x = xTaskGetTickCount() - ble_uart_lasttick;
                if(x < 100) {
                    // Ugly hack if we already send something recently.
                    // TODO: figure out how fast we can send or use indications
                    vTaskDelay(100 - x);
                }
                //printf("notify: ");
                //int j;
                //for(j=0;j<ble_uart_buf_tx_fill;j++) {
                //    printf("%02x ", ble_uart_tx_buf[j]);
                //}
                //printf("\n");
                AttsHandleValueNtf(active_connection, UART_TX_HDL, ble_uart_buf_tx_fill, ble_uart_tx_buf);
                ble_uart_lasttick = xTaskGetTickCount();
            }
            ble_uart_buf_tx_fill = 0;
        }
    }
  }
}

void bleuart_init(void)
{
    /* Add the UART service dynamically */
    void *pSHdl;
    pSHdl = SvcUARTAddGroupDyn();
    AttsDynRegister(pSHdl, UARTReadCback, UARTWriteCback);
    //AttsDynRegister(pSHdl, NULL, UARTWriteCback);
}

