#include <stdlib.h>
#include "sema.h"
#include "api/caller.h"

#define MXC_ASSERT_ENABLE
#include "mxc_assert.h"

void *_api_call_start(api_id_t id, uintptr_t size)
{
	while (SEMA_GetSema(_API_SEMAPHORE) == E_BUSY) {
	}

	if (API_CALL_MEM->call_flag != _API_FLAG_IDLE) {
		/*
		 * The only way this can happen is if a call was issued from an
		 * interrupt hander while another call is still happening.  This
		 * has to be prevented at all cost!
		 */
		mxc_assert(
			"API recalled during ongoing call!",
			__FILE__,
			__LINE__
		);
	}

	API_CALL_MEM->id = id;
	return API_CALL_MEM->buffer;
}

void *_api_call_transact(void *buffer)
{
	API_CALL_MEM->call_flag = _API_FLAG_CALLING;
	SEMA_FreeSema(_API_SEMAPHORE);

	/* Notify the dispather of the new call */
	__SEV();
	__WFE();

	while (1) {
		/* Wait for the dispather to return */
		__WFE();

		while (SEMA_GetSema(_API_SEMAPHORE) == E_BUSY) {
		}
		if (API_CALL_MEM->call_flag == _API_FLAG_RETURNED) {
			break;
		}
		SEMA_FreeSema(_API_SEMAPHORE);
	}

	API_CALL_MEM->call_flag = _API_FLAG_IDLE;
	SEMA_FreeSema(_API_SEMAPHORE);

	return API_CALL_MEM->buffer;
}
