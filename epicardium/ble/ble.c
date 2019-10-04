#include "epicardium.h"
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

#define FACTOR 2
#define WSF_BUF_POOLS 6
#define WSF_BUF_SIZE (0x1048 * FACTOR)

uint32_t SystemHeapSize = WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE / 4];
uint32_t SystemHeapStart;

/* Task ID for the ble handler */
static TaskHandle_t ble_task_id = NULL;

/*! Default pool descriptor. */
/* clang-format off */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8*FACTOR },
  {  32,  4*FACTOR },
  {  64,  4*FACTOR },
  { 128,  4*FACTOR },
  { 256,  4*FACTOR },
  { 512,  4*FACTOR }
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
extern void BbBleDrvSetTxPower(int8_t power);

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
	uint8_t bdAddr[6] = { 0xCA, 0x4D, 0x10, 0x00, 0x00, 0x00 };
	char buf[32];

	int result = fs_read_text_file("mac.txt", buf, sizeof(buf));

	if (result == -1) {
		APP_TRACE_INFO0("mac.txt not found, generating random MAC");
		epic_trng_read(bdAddr + 3, 3);
		sprintf(buf,
			"%02x:%02x:%02x:%02x:%02x:%02x\n",
			bdAddr[0],
			bdAddr[1],
			bdAddr[2],
			bdAddr[3],
			bdAddr[4],
			bdAddr[5]);
		fs_write_file("mac.txt", buf, strlen(buf));
	} else {
		APP_TRACE_INFO1("mac file contents: %s", buf);
	}

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
	NVIC->STIR = RSV11_IRQn;
}
/*************************************************************************************************/
void wsf_ble_signal_event(void)
{
	//printf("wsf_ble_signal_event\n");
	NVIC->STIR = RSV11_IRQn;
}
/*************************************************************************************************/
void RSV11_IRQHandler(void)
{
	notify();
}
/*************************************************************************************************/
#define BLEMAXCFGBYTES 100
bool ble_shall_start(void)
{
	int bleConfigFile = epic_file_open("ble.txt", "r");
	if (bleConfigFile < 0) {
		LOG_INFO("ble", "can not open ble.txt -> BLE is not started");
		epic_file_close(bleConfigFile);
		return false;
	}

	char cfgBuf[BLEMAXCFGBYTES + 1];
	int readNum = epic_file_read(bleConfigFile, cfgBuf, BLEMAXCFGBYTES);
	epic_file_close(bleConfigFile);
	if (readNum < 0) {
		LOG_WARN("ble", "can not read ble.txt -> BLE is not started");
		return false;
	}
	cfgBuf[readNum] = '\0';

	char bleActiveStr[]              = "active=true";
	cfgBuf[sizeof(bleActiveStr) - 1] = '\0';

	if (strcmp(cfgBuf, "active=true") != 0) {
		LOG_INFO("ble", "BLE is disabled.");
		return false;
	} else {
		LOG_INFO("ble", "BLE is enabled.");
		return true;
	}
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

	/* We are going to execute FreeRTOS functions from callbacks
	 * coming from this interrupt. Its priority needs to be
	 * reduced to allow this. */
	NVIC_SetPriority(RSV11_IRQn, 2);
	NVIC_EnableIRQ(RSV11_IRQn);

	WsfInit();
	taskENTER_CRITICAL();
	/* Critical section to prevent a loop in iq_capture2 / meas_freq in
	 * /home/maxim/Documents/src/BLE/mcbusw/Hardware/Micro/ME14/Firmware/trunk/NDALibraries/BTLE/phy/dbb/prot/ble/pan2g5/afe/max32665/board_config.c:275
	 * if BHI160 and -Ddebug_prints=true is enabled*/
	StackInit();
	taskEXIT_CRITICAL();
	BbBleDrvSetTxPower(0);
	setAddress();

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
