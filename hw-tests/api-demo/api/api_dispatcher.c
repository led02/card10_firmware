#include "api_dispatcher.h"

int api_init (sys_cfg_sema_t *sys_cfg)
{
	int ret;

	ret = SEMA_Init (sys_cfg);
	/* Set the status of the flag to a valid initial state (right before
	 * api-calls are made) */
	ApiCallSpace->returning = 1;

	return ret;
}

void api_dispatcher()
{
	while (SEMA_GetSema(API_CALL_SEMA) == E_BUSY) {}

	if (ApiCallSpace->returning == 1) {
		SEMA_FreeSema(API_CALL_SEMA);
		return;
	}

	printf("core1: Catched API CALL!\n");
	printf("%d: ",ApiCallSpace->id);
	for (int i = 0; i < 16; i++) {
		printf("0x%02x ", ((char*)ApiCallSpace->buf)[i]);
	}
	printf("\n");

	ApiCallSpace->returning = 1;

	SEMA_FreeSema(API_CALL_SEMA);
}
