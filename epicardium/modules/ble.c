#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "fit/fit_api.h"
#include "hci_vs.h"

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

    switch (event) {
    case APP_UI_RESET_CMPL:
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

static void ble_init(void)
{
    WsfInit();
    StackInitFit();
    NVIC_SetPriority(BTLE_SFD_TO_IRQn, 2);
    NVIC_SetPriority(BTLE_TX_DONE_IRQn, 2);
    NVIC_SetPriority(BTLE_RX_RCVD_IRQn, 2);
    FitStart();

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
