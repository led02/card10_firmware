#include "max32665.h"
#include "tmr.h"
#include "api/common.h"
#include "epicardium.h"

void api_interrupt_handler_ctrl_c(api_int_id_t id)
	__attribute__((weak, alias("api_interrupt_handler_default")));
void api_interrupt_handler_bhi160(api_int_id_t id)
	__attribute__((weak, alias("api_interrupt_handler_default")));

/* Timer Interrupt used for control char notification */
void TMR5_IRQHandler(void)
{
	TMR_IntClear(MXC_TMR5);

	switch (API_CALL_MEM->int_id) {
	case API_INT_CTRL_C:
		api_interrupt_handler_ctrl_c(API_CALL_MEM->int_id);
		break;
	case API_INT_BHI160:
		api_interrupt_handler_bhi160(API_CALL_MEM->int_id);
		break;
	}

	API_CALL_MEM->int_id = 0;
}

__attribute__((weak)) void api_interrupt_handler_catch_all(api_int_id_t id)
{
}

void api_interrupt_handler_default(api_int_id_t id)
{
	api_interrupt_handler_catch_all(id);
}
