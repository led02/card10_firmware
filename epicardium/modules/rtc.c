#include "epicardium.h"
#include "modules/log.h"
#include "api/interrupt-sender.h"

#include "FreeRTOS.h"
#include "task.h"

#include "rtc.h"

#include <stdint.h>

uint32_t epic_rtc_get_seconds(void)
{
	uint32_t sec, subsec;

	/*
	 * TODO:  Find out what causes the weird behavior of this function.  The
	 *        time needed for this call seems to depend on the frequency at
	 *        which it is called.
	 */
	while (RTC_GetTime(&sec, &subsec) == E_BUSY) {
		vTaskDelay(pdMS_TO_TICKS(4));
	}

	return sec;
}

void RTC_IRQHandler(void)
{
	int flags = RTC_GetFlags();

	if (flags & MXC_F_RTC_CTRL_ALDF) {
		RTC_ClearFlags(MXC_F_RTC_CTRL_ALDF);
		api_interrupt_trigger(EPIC_INT_RTC_ALARM);
	} else {
		LOG_WARN("rtc", "Unknown IRQ caught!");
		/* Disable IRQ so it does not retrigger */
		NVIC_DisableIRQ(RTC_IRQn);
	}
}

int epic_rtc_schedule_alarm(uint32_t timestamp)
{
	int res;

	NVIC_EnableIRQ(RTC_IRQn);

	while ((res = RTC_SetTimeofdayAlarm(MXC_RTC, timestamp)) == E_BUSY)
		;

	if (res != E_SUCCESS) {
		return -EINVAL;
	}

	return 0;
}
