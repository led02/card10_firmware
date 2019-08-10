#include "api_dispatcher.h"

int api_init(sys_cfg_sema_t *sys_cfg)
{
	int ret;

	ret = SEMA_Init(sys_cfg);
	/* Set the status of the flag to a valid initial state (right before
	 * api-calls are made) */
	ApiCallSpace->returning = 1;

	return ret;
}

/* Generated function */
void __api_dispatch_call(uint32_t id, void *buffer);

void api_dispatcher()
{
	while (SEMA_GetSema(API_CALL_SEMA) == E_BUSY) {
	}

	if (ApiCallSpace->returning == 1) {
		SEMA_FreeSema(API_CALL_SEMA);
		return;
	}

	printf("core1: Catched API CALL!\n");
	__api_dispatch_call(ApiCallSpace->id, ApiCallSpace->buf);

	ApiCallSpace->returning = 1;

	SEMA_FreeSema(API_CALL_SEMA);
}
