#include <stdlib.h>

#include "api/api_caller.h"
#include "tmr_utils.h"

void *api_call_start(uint32_t id, void *args, uint32_t size)
{
	// aquire semaphore
	while (E_BUSY == SEMA_GetSema(API_CALL_SEMA))
		;

	ApiCallSpace->id        = id;
	ApiCallSpace->returning = 0;
	return ApiCallSpace->buf;
}

void *api_call_bother_dispatcher(void *buf)
{
	SEMA_FreeSema(API_CALL_SEMA);
	// TODO: set event

	while (1) {
		// aquire semaphore
		while (E_BUSY == SEMA_GetSema(API_CALL_SEMA))
			;
		if (ApiCallSpace->returning == 1) {
			break;
		}
		SEMA_FreeSema(API_CALL_SEMA);
		TMR_Delay(MXC_TMR2, MSEC(100), 0);
	}
	SEMA_FreeSema(API_CALL_SEMA);

	return NULL;
}
