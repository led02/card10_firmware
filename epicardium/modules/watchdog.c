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

void watchdog_clearer_init()
{
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
