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

static bool event_ready = false;

bool api_dispatcher_poll_once()
{
	if (event_ready) {
		return false;
	}

	while (SEMA_GetSema(_API_SEMAPHORE) == E_BUSY) {}

	if (API_CALL_MEM->call_flag != _API_FLAG_CALLING) {
		SEMA_FreeSema(_API_SEMAPHORE);
		return false;
	}

	event_ready = true;
	return true;
}

bool api_dispatcher_poll()
{
	if (event_ready) {
		return true;
	}

	return api_dispatcher_poll_once();
}

api_id_t api_dispatcher_exec()
{
	if (!event_ready) {
		return 0;
	}

	api_id_t id = API_CALL_MEM->id;
	__api_dispatch_call(id, API_CALL_MEM->buffer);
	API_CALL_MEM->call_flag = _API_FLAG_RETURNED;

	event_ready = false;
	SEMA_FreeSema(_API_SEMAPHORE);

	/* Notify the caller that we returned */
	__SEV();
	__WFE();

	return id;
}
