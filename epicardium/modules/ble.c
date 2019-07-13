#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "ll_api.h"
#include "sch_api.h"
#include "fit/fit_api.h"
#include "mxc_config.h"
#include "gcr_regs.h"
#include "mcr_regs.h"
#include "hci_core.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>

/* Task ID for the ble handler */
static TaskHandle_t ble_task_id = NULL;

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS              6

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
  { 384,  4 }
};


static void PlatformInit(void)
{
    /* Change the pullup on the RST pin to 25K */
    /* TODO: Is this really needed? */
    MXC_MCR->ctrl = 0x202;

    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Power up the 32MHz XO */
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;

    /* Enable peripheral clocks */
    /* TODO: Is this really needed? */
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);  // Clear GPIO0 and GPIO1 Disable
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_BTLED | MXC_F_GCR_PERCKCN1_TRNGD );  // Clear BTLE and ICACHE0 disable
}

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
/* TODO: WTF? */
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
extern int8_t tx_rfpower_on;
void MacInit(void)
{
    wsfHandlerId_t handlerId;

    /* Initialize link layer. */
    BbInit();
    handlerId = WsfOsSetNextHandler(SchHandler);
    SchInit(handlerId);
    LlAdvSlaveInit();
    LlConnSlaveInit();
    handlerId = WsfOsSetNextHandler(LlHandler);
    LlHandlerInit(handlerId);
}




static StaticTimer_t x;
TimerHandle_t timerWakeup;
int lasttick = 0;

static void vTimerCallback(xTimerHandle pxTimer)
{
    bool_t          timerRunning;
    wsfTimerTicks_t time_to_next_expire;
    do {
        int tick = xTaskGetTickCount();
        WsfTimerUpdate(tick - lasttick);
        lasttick = tick;
        time_to_next_expire = WsfTimerNextExpiration(&timerRunning);
    } while (timerRunning && time_to_next_expire == 0);


    if(timerRunning) {
        printf("time_to_next_expire = %d\n", time_to_next_expire);
        timerWakeup = xTimerCreateStatic(
            "timerWakeup", /* name */
            pdMS_TO_TICKS(time_to_next_expire), /* period/time */
            pdFALSE, /* auto reload */
            NULL, /* timer ID */
            vTimerCallback, &x); /* callback */

        if(timerWakeup != NULL) {
            xTimerStart(timerWakeup, 0);
        } else {
            printf("could not create timer\n");
        }
    } else {
        printf("No timer running\n");
    }
}


static void ble_init(void)
{
    PlatformInit();
    WsfInit();
    MacInit();
    //StackInitFit();
    //FitStart();

    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);

    lasttick = xTaskGetTickCount();
    vTimerCallback(NULL);
}


void vBleTask(void*pvParameters)
{
	ble_task_id = xTaskGetCurrentTaskHandle();

    ble_init();
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

    while (1){
        // TODO: this need some timing and sleeping
        wsfOsDispatcher();
        vTimerCallback(NULL);
        vTaskDelay( xDelay );
    }
}
