#include "epicardium.h"
#include "modules/log.h"
#include "api/interrupt-sender.h"

#include "FreeRTOS.h"
#include "task.h"

#include "rtc.h"

#include <stdint.h>

uint64_t monotonic_offset = 0;

uint32_t epic_rtc_get_monotonic_seconds(void)
{
	return epic_rtc_get_seconds() + monotonic_offset / 1000ULL;
}

uint64_t epic_rtc_get_monotonic_milliseconds(void)
{
	return epic_rtc_get_milliseconds() + monotonic_offset;
}

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

uint64_t epic_rtc_get_milliseconds(void)
{
	uint32_t sec, subsec;

	while (RTC_GetTime(&sec, &subsec) == E_BUSY) {
		vTaskDelay(pdMS_TO_TICKS(4));
	}

	// Without the bias of 999 (0.24 milliseconds), this decoding function is
	// numerically unstable:
	//
	// Encoding 5 milliseconds into 20 subsecs (using the encoding function in
	// epic_rtc_set_milliseconds) and decoding it without the bias of 999 yields
	// 4 milliseconds.
	//
	// The following invariants should hold when encoding / decoding from and to
	// milliseconds / subseconds:
	//
	// - 0 <= encode(ms) < 4096 for 0 <= ms < 1000
	// - decode(encode(ms)) == ms for 0 <= ms < 1000
	// - 0 <= decode(subsec) < 1000 for 0 <= subsec < 4096
	//
	// These invariants were proven experimentally.
	return (subsec * 1000ULL + 999ULL) / 4096 + sec * 1000ULL;
}

void epic_rtc_set_milliseconds(uint64_t milliseconds)
{
	uint32_t sec, subsec;
	uint64_t old_milliseconds, diff;

	old_milliseconds = epic_rtc_get_milliseconds();

	sec    = milliseconds / 1000;
	subsec = (milliseconds % 1000);
	subsec *= 4096;
	subsec /= 1000;

	while (RTC_Init(MXC_RTC, sec, subsec, NULL) == E_BUSY)
		;
	while (RTC_EnableRTCE(MXC_RTC) == E_BUSY)
		;

	diff = old_milliseconds - milliseconds;
	monotonic_offset += diff;
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

	/*
	 * Check if the timestamp lies in the past and if so, trigger
	 * immediately.
	 */
	if (epic_rtc_get_seconds() >= timestamp) {
		api_interrupt_trigger(EPIC_INT_RTC_ALARM);
		return 0;
	}

	NVIC_EnableIRQ(RTC_IRQn);

	while ((res = RTC_SetTimeofdayAlarm(MXC_RTC, timestamp)) == E_BUSY)
		;

	if (res != E_SUCCESS) {
		return -EINVAL;
	}

	return 0;
}
