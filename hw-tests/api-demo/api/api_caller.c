#include <stdlib.h>

#include "api/api_caller.h"

void* api_call_start (uint32_t id, void* args, uint32_t size)
{
	// aquire semaphore
	while (E_BUSY == SEMA_GetSema (API_CALL_SEMA)) ;

	ApiCallSpace->id = id;
	ApiCallSpace->returning = 0;
	return ApiCallSpace;
}

void* api_call_bother_dispatcher (void* buf)
{
	SEMA_FreeSema (API_CALL_SEMA);
	// set event

	return NULL;
}
