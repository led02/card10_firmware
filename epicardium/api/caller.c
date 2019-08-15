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

__attribute__((noreturn)) void epic_exit(int ret)
{
	/*
	 * Call __epic_exit() and then jump to the reset routine/
	 */
	void *buffer;

	buffer         = _api_call_start(API_SYSTEM_EXIT, sizeof(int));
	*(int *)buffer = ret;
	_api_call_transact(buffer);

	API_CALL_MEM->reset_stub();

	/* unreachable */
	while (1)
		;
}

int epic_exec(char *name)
{
	/*
	 * Call __epic_exec().  If it succeeds, jump to the reset routine.
	 * Otherwise, return the error code.
	 */
	void *buffer;

	buffer           = _api_call_start(API_SYSTEM_EXEC, sizeof(char *));
	*(char **)buffer = name;
	int ret          = *(int *)_api_call_transact(buffer);

	if (ret < 0) {
		return ret;
	}

	API_CALL_MEM->reset_stub();

	/* unreachable */
	while (1)
		;
}

int api_fetch_args(char *buf, size_t cnt)
{
	if (API_CALL_MEM->id != 0) {
		/*
		 * When any call happened before the args are fetched, they are
		 * overwritten and no longer accessible.
		 */
		return (-1);
	}

	if (API_CALL_MEM->buffer[0x20] == '\0') {
		return 0;
	}

	int i;
	for (i = 0; i < cnt && API_CALL_MEM->buffer[i + 0x20] != '\0'; i++) {
		buf[i] = API_CALL_MEM->buffer[i + 0x20];
	}

	return i - 1;
}
