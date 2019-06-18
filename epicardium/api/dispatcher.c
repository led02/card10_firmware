#include <stdlib.h>
#include "sema.h"
#include "api/dispatcher.h"

int api_dispatcher_init()
{
	int ret;

	ret = SEMA_Init(NULL);
	API_CALL_MEM->call_flag = _API_FLAG_IDLE;

	/*
	 * Enable TX events for both cores.
	 * TODO: Is this the right place?
	 */
	MXC_GCR->evten |= 0x24;

	return ret;
}

api_id_t api_dispatcher_poll()
{
	api_id_t id = 0;
	while (SEMA_GetSema(_API_SEMAPHORE) == E_BUSY) {}

	if (API_CALL_MEM->call_flag != _API_FLAG_CALLING) {
		SEMA_FreeSema(_API_SEMAPHORE);
		return 0;
	}

	id = API_CALL_MEM->id;
	__api_dispatch_call(id, API_CALL_MEM->buffer);
	API_CALL_MEM->call_flag = _API_FLAG_RETURNED;

	SEMA_FreeSema(_API_SEMAPHORE);

	/* Notify the caller that we returned */
	__SEV();
	__WFE();

	return id;
}
