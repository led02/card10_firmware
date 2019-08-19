#include "modules/log.h"

#include "fs_util.h"
#include "wsf_types.h"
#include "wsf_buf.h"
#include "wsf_trace.h"
#include "ble_api.h"
#include "hci_vs.h"
#include "att_api.h"

#include "FreeRTOS.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define WSF_BUF_POOLS 6
#define WSF_BUF_SIZE 0x1048

uint32_t SystemHeapSize = WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE / 4];
uint32_t SystemHeapStart;

/* Task ID for the ble handler */
static TaskHandle_t ble_task_id = NULL;

/*! Default pool descriptor. */
/* clang-format off */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 512,  4 }
};
/* clang-format on */

static StaticTimer_t x;
static TimerHandle_t timerWakeup = NULL;
static int lasttick              = 0;

/*! \brief  Stack initialization for app. */
extern void StackInit(void);
extern void AppInit(void);
extern void bleuart_init(void);
extern void bleFileTransfer_init(void);
extern void bleCard10_init(void);

/*************************************************************************************************/
void PalSysAssertTrap(void)
{
	while (1) {
	}
}
/*************************************************************************************************/
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
	extern uint8_t wsfCsNesting;

	if (wsfCsNesting == 0) {
		fwrite(pBuf, len, 1, stdout);
		return TRUE;
	}

	return FALSE;
}
/*************************************************************************************************/
static void WsfInit(void)
{
	uint32_t bytesUsed __attribute__((unused));
	WsfTimerInit();

	SystemHeapStart = (uint32_t)&SystemHeap;
	memset(SystemHeap, 0, sizeof(SystemHeap));
	//printf("SystemHeapStart = 0x%x\n", SystemHeapStart);
	//printf("SystemHeapSize = 0x%x\n", SystemHeapSize);

	WsfTraceRegisterHandler(myTrace);
	WsfTraceEnable(TRUE);

	bytesUsed = WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
	APP_TRACE_INFO1("bytesUsed = %u", (unsigned int)bytesUsed);
}
/*************************************************************************************************/
/* TODO: We need a source of MACs */
static void setAddress(void)
{
	uint8_t bdAddr[6] = { 0x02, 0x02, 0x44, 0x8B, 0x05, 0x00 };
	char buf[32];

	fs_read_text_file("mac.txt", buf, sizeof(buf));
	APP_TRACE_INFO1("mac file contents: %s", buf);
	int a, b, c, d, e, f;
	if (sscanf(buf, "%x:%x:%x:%x:%x:%x", &a, &b, &c, &d, &e, &f) == 6) {
		bdAddr[0] = f;
		bdAddr[1] = e;
		bdAddr[2] = d;
		bdAddr[3] = c;
		bdAddr[4] = b;
		bdAddr[5] = a;
	}

	LOG_INFO(
		"ble",
		"Setting MAC address to %02X:%02X:%02X:%02X:%02X:%02X",
		bdAddr[5],
		bdAddr[4],
		bdAddr[3],
		bdAddr[2],
		bdAddr[1],
		bdAddr[0]
	);
	HciVsSetBdAddr(bdAddr);
}
/*************************************************************************************************/
static void vTimerCallback(xTimerHandle pxTimer)
{
	//printf("wake\n");
	int tick = xTaskGetTickCount();
	//printf("WsfTimerUpdate(%d)\n", tick - lasttick);
	WsfTimerUpdate(tick - lasttick);
	lasttick = tick;
	//printf("done\n");
}
/*************************************************************************************************/
static void notify(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (xPortIsInsideInterrupt()) {
		vTaskNotifyGiveFromISR(ble_task_id, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	} else {
		xTaskNotifyGive(ble_task_id);
	}
}
/*************************************************************************************************/
void WsfTimerNotify(void)
{
	//printf("WsfTimerNotify\n");
	// TODO: Can we do this without waking up the task?
	// xTimerChangePeriodFromISR exists
	notify();
}
/*************************************************************************************************/
void wsf_ble_signal_event(void)
{
	//printf("wsf_ble_signal_event\n");
	notify();
}
/*************************************************************************************************/
static void scheduleTimer(void)
{
	bool_t timerRunning;
	wsfTimerTicks_t time_to_next_expire;

	vTimerCallback(NULL);
	time_to_next_expire = WsfTimerNextExpiration(&timerRunning);

	if (timerRunning) {
		//printf("time_to_next_expire = %d\n", time_to_next_expire);
		//printf("change period\n");
		/* We need to make sure not to schedule a 0 ticks timer.
		 * Maybe it would also be enough to simply call the dispatcher
		 * in this case... */
		if (time_to_next_expire == 0) {
			time_to_next_expire = 1;
		}

		if (timerWakeup != NULL) {
			xTimerChangePeriod(
				timerWakeup,
				pdMS_TO_TICKS(time_to_next_expire),
				0
			);
			//printf("insert done\n");
		} else {
			LOG_ERR("ble", "Could not create timer");
		}
	} else {
		APP_TRACE_INFO0("No timer running");
	}
}
/*************************************************************************************************/
void vBleTask(void *pvParameters)
{
	ble_task_id = xTaskGetCurrentTaskHandle();

	/*
	 * Delay BLE startup by a bit because it locks up Epicardium otherwise.
	 */
	vTaskDelay(pdMS_TO_TICKS(500));

	WsfInit();
	StackInit();
	setAddress();

	NVIC_SetPriority(BTLE_SFD_TO_IRQn, 2);
	NVIC_SetPriority(BTLE_TX_DONE_IRQn, 2);
	NVIC_SetPriority(BTLE_RX_RCVD_IRQn, 2);
	AppInit();
	BleStart();
	AttsDynInit();

	bleuart_init();
	bleFileTransfer_init();
	bleCard10_init();

	lasttick = xTaskGetTickCount();

	timerWakeup = xTimerCreateStatic(
		"timerWakeup",    /* name */
		pdMS_TO_TICKS(1), /* period/time */
		pdFALSE,          /* auto reload */
		NULL,             /* timer ID */
		vTimerCallback,
		&x); /* callback */

	while (1) {
		ulTaskNotifyTake(pdTRUE, portTICK_PERIOD_MS * 1000);
		wsfOsDispatcher();
		scheduleTimer();
	}
}
