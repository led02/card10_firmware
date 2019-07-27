#include "rtc.h"

#include <stdint.h>

uint32_t epic_rtc_get_seconds(void)
{
	return RTC_GetSecond();
}
