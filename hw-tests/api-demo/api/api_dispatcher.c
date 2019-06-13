#include "api_common.h"

int
api_init (sys_cfg_sema_t *sys_cfg)
{
	int ret;

	ret = SEMA_Init (sys_cfg);
	ApiCallSpace->returning = 1;
}

