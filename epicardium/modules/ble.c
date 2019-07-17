#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "fit/fit_api.h"
#include "hci_vs.h"

#include "att_defs.h"
#include "util/bstream.h"
#include "att_api.h"

#include "modules.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

uint32_t SystemHeapSize=WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE/4];
uint32_t SystemHeapStart;

/* Task ID for the ble handler */
static TaskHandle_t ble_task_id = NULL;

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 512,  4 }
};

/*! \brief  Stack initialization for app. */
extern void StackInitFit(void);

/*************************************************************************************************/
void PalSysAssertTrap(void)
{
    while(1) {}
}


/*************************************************************************************************/
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        fwrite(pBuf, len, 1, stdout);
        return TRUE;
    }

    return FALSE;
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
    uint32_t bytesUsed;
    WsfTimerInit();

    SystemHeapStart = (uint32_t)&SystemHeap;
    memset(SystemHeap, 0, sizeof(SystemHeap));
    //printf("SystemHeapStart = 0x%x\n", SystemHeapStart);
    //printf("SystemHeapSize = 0x%x\n", SystemHeapSize);
    bytesUsed = WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
    printf("bytesUsed = %u\n", (unsigned int)bytesUsed);
    
    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);
}
/* TODO: We need a source of MACs */
/*
 * In two-chip solutions, setting the address must wait until the HCI interface is initialized.
 * This handler can also catch other Application events, but none are currently implemented.
 * See ble-profiles/sources/apps/app/common/app_ui.c for further details.
 *
 */
void SetAddress(uint8_t event)
{
    uint8_t bdAddr[6] = {0x02, 0x02, 0x44, 0x8B, 0x05, 0x00};
    char buf[32];

    switch (event) {
    case APP_UI_RESET_CMPL:
        fs_read_text_file("mac.txt", buf, sizeof(buf));
        printf("mac file: %s\n", buf);
        int a, b, c, d, e, f;
        if(sscanf(buf, "%x:%x:%x:%x:%x:%x", &a, &b, &c, &d, &e, &f) == 6) {
            bdAddr[0] = f;
            bdAddr[1] = e;
            bdAddr[2] = d;
            bdAddr[3] = c;
            bdAddr[4] = b;
            bdAddr[5] = a;
        }

        printf("Setting address -- MAC %02X:%02X:%02X:%02X:%02X:%02X\n", bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1], bdAddr[0]);
        HciVsSetBdAddr(bdAddr);
        break;
    default:
        break;
    }
}

static StaticTimer_t x;
TimerHandle_t timerWakeup = NULL;
int lasttick = 0;
bool schedule_needed = false;

static void vTimerCallback(xTimerHandle pxTimer)
{
    //printf("wake\n");
    int tick = xTaskGetTickCount();
    printf("WsfTimerUpdate(%d)\n", tick - lasttick);
    WsfTimerUpdate(tick - lasttick);
    lasttick = tick;
    //printf("done\n");
}

static void notify(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xPortIsInsideInterrupt()) {
	    vTaskNotifyGiveFromISR(ble_task_id, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else {
	     xTaskNotifyGive(ble_task_id);
    }
}

void WsfTimerNotify(void)
{
    //printf("WsfTimerNotify\n");
    // TODO: Can we do this without waking up the task?
    // xTimerChangePeriodFromISR exists
    notify();
}

void wsf_ble_signal_event(void)
{
    //printf("wsf_ble_signal_event\n");
    notify();
}

static void scheduleTimer(void)
{
    bool_t          timerRunning;
    wsfTimerTicks_t time_to_next_expire;

    time_to_next_expire = WsfTimerNextExpiration(&timerRunning);

    if(timerRunning) {
        //printf("time_to_next_expire = %d\n", time_to_next_expire);
        //printf("change period\n");
        if(timerWakeup != NULL) {
            xTimerChangePeriod(timerWakeup, pdMS_TO_TICKS(time_to_next_expire), 0);
            //printf("insert done\n");
        } else {
            printf("could not create timer\n");
        }
    } else {
        printf("No timer running\n");
    }
}

#define UART_START_HDL                    0x800                /*!< \brief Service start handle. */
#define UART_END_HDL                      (BATT_MAX_HDL - 1)  /*!< \brief Service end handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief UART Service Handles */
enum
{
  UART_SVC_HDL = UART_START_HDL,        /*!< \brief UART service declaration */
  UART_RX_CH_HDL,                      /*!< \brief UART rx characteristic */
  UART_RX_HDL,                         /*!< \brief UART rx value */
  UART_TX_CH_HDL,                      /*!< \brief UART tx characteristic */
  UART_TX_HDL,                         /*!< \brief UART tx value */
  UART_TX_CH_CCC_HDL,                  /*!< \brief UART tx CCCD */
  BATT_MAX_HDL                          /*!< \brief Maximum handle. */
};
/**@}*/

static const uint8_t UARTSvc[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x01,0x00,0x40,0x6E};

static const uint8_t uartRxCh[] = {ATT_PROP_WRITE, UINT16_TO_BYTES(UART_RX_HDL), 0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E};
const uint8_t attUartRxChUuid[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5, 0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E};

static const uint8_t uartTxCh[] = {ATT_PROP_READ | ATT_PROP_NOTIFY, UINT16_TO_BYTES(UART_TX_HDL), 0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E};
const uint8_t attUartTxChUuid[] = {0x9E,0xCA,0xDC,0x24,0x0E,0xE5, 0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E};
/* Battery level client characteristic configuration */



void *SvcUARTAddGroupDyn(void)
{
  void *pSHdl;
  uint8_t initCcc[] = {UINT16_TO_BYTES(0x0000)};
  uint8_t initUARTVal[] = {0x20};

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
    AttsDynAddAttr(pSHdl, attUartTxChUuid, initUARTVal, sizeof(uint8_t), sizeof(uint8_t),
                   ATTS_SET_READ_CBACK, ATTS_PERMIT_READ);
    /* UART tx CCC descriptor */
    AttsDynAddAttr(pSHdl, attCliChCfgUuid, initCcc, sizeof(uint16_t), sizeof(uint16_t),
                   ATTS_SET_CCC, ATTS_PERMIT_READ | ATTS_PERMIT_WRITE);
  }

  return pSHdl;
}

dmConnId_t active_connection = 0;

uint8_t UARTReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                     uint16_t offset, attsAttr_t *pAttr)
{
  //AppHwBattRead(pAttr->pValue);
  printf("read callback\n");
  return ATT_SUCCESS;
}

uint8_t UARTWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
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
                    // TODO: figure out how these notifications are acknowledged
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


static void ble_init(void)
{
    WsfInit();
    StackInitFit();
    NVIC_SetPriority(BTLE_SFD_TO_IRQn, 2);
    NVIC_SetPriority(BTLE_TX_DONE_IRQn, 2);
    NVIC_SetPriority(BTLE_RX_RCVD_IRQn, 2);
    FitStart();

    /* Add the UART service dynamically */
    AttsDynInit();
    void *pSHdl;
    pSHdl = SvcUARTAddGroupDyn();
    AttsDynRegister(pSHdl, UARTReadCback, UARTWriteCback);
    //AttsDynRegister(pSHdl, NULL, UARTWriteCback);


    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);

    lasttick = xTaskGetTickCount();

    timerWakeup = xTimerCreateStatic(
        "timerWakeup", /* name */
        pdMS_TO_TICKS(1), /* period/time */
        pdFALSE, /* auto reload */
        NULL, /* timer ID */
        vTimerCallback, &x); /* callback */
}

void vBleTask(void*pvParameters)
{
	ble_task_id = xTaskGetCurrentTaskHandle();
    ble_init();

    while (1){
		ulTaskNotifyTake(pdTRUE, portTICK_PERIOD_MS * 1000);
        wsfOsDispatcher();
        scheduleTimer();
    }
}
