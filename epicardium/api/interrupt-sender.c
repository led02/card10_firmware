#include "api/interrupt-sender.h"
#include "api/common.h"
#include "tmr_utils.h"

static bool enabled[API_INT_MAX + 1];

void api_interrupt_trigger(api_int_id_t id)
{
	if (id <= API_INT_MAX) {
		if (enabled[id]) {
			while (API_CALL_MEM->int_id)
				;
			API_CALL_MEM->int_id = id;
			TMR_TO_Start(MXC_TMR5, 1, 0);
		}
	}
}

void api_interrupt_init(void)
{
	int i;
	API_CALL_MEM->int_id = 0;

	for (i = 0; i <= API_INT_MAX; i++) {
		enabled[i] = false;
	}
}

void epic_interrupt_enable(api_int_id_t int_id)
{
	if (int_id <= API_INT_MAX) {
		enabled[int_id] = true;
	}
}

void epic_interrupt_disable(api_int_id_t int_id)
{
	if (int_id <= API_INT_MAX) {
		enabled[int_id] = false;
	}
}
