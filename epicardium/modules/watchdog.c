#include "modules/log.h"
#include "modules/modules.h"

#include "timers.h"
#include "mxc_sys.h"
#include "wdt.h"

static TimerHandle_t clearer_timer;
static StaticTimer_t clearer_timer_buffer;
#define CLEAR_PERIOD pdMS_TO_TICKS(2000)

static void watchdog_clearer_callback()
{
	WDT_ResetTimer(MXC_WDT0);
}

void watchdog_init()
{
	/*
	 * Don't enable the the watchdog when a debugger is connected.
	 */
	if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0) {
		return;
	}

	sys_cfg_wdt_t wdt_cfg = NULL;
	WDT_Init(MXC_WDT0, wdt_cfg);

	if (WDT_GetResetFlag(MXC_WDT0)) {
		WDT_ClearResetFlag(MXC_WDT0);
		LOG_INFO("watchdog", "Last reset was due to watchdog timeout");
	}

	WDT_Enable(MXC_WDT0, 1);
	WDT_SetResetPeriod(
		MXC_WDT0,
		WDT_PERIOD_2_28); /* Clocked by PCLK at 50MHz, reset at 2^28 ticks = 5.4 seconds */
	WDT_EnableReset(MXC_WDT0, 1);
}

void watchdog_clearer_init()
{
	WDT_ResetTimer(MXC_WDT0);

	clearer_timer = xTimerCreateStatic(
		"watchdog_clearer_timer",
		CLEAR_PERIOD,
		pdTRUE,
		NULL,
		watchdog_clearer_callback,
		&clearer_timer_buffer
	);
	xTimerStart(clearer_timer, 0);
}
