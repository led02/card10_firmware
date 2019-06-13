#include "api_dispatcher.h"

int
api_init (sys_cfg_sema_t *sys_cfg)
{
	int ret;

	ret = SEMA_Init (sys_cfg);
	/* Set the status of the flag to a valid initial state (right before
	 * api-calls are made) */
	ApiCallSpace->returning = 1;

	return ret;
}

