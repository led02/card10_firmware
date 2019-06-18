#include "api/common.h"

/*
 * Initialize the API system.  This function *must* be called
 * before any API action can take place.
 */
int api_dispatcher_init();

/*
 * Attempt to dispatch a call, if the caller has requested one.
 * Will return 0 if no call was dispatched and the ID of the dispatched
 * call otherwise.
 */
api_id_t api_dispatcher_poll();

/* This function is defined by the generated dispatcher code */
void __api_dispatch_call(api_id_t id, void*buffer);
