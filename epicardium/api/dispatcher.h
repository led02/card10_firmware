#include "api/common.h"

/*
 * Initialize the API system.  This function *must* be called
 * before any API action can take place.
 */
int api_dispatcher_init();

/*
 * Check whether the other core requested a call.  If this function returns
 * true, the dispatcher should call api_dispatcher_exec() to actually dispatch
 * the call.  Consecutive calls to this function will return false.  Use
 * api_dispatcher_poll() if your need to recheck.
 */
bool api_dispatcher_poll_once();
bool api_dispatcher_poll();

/*
 * Attempt to dispatch a call, if one had been polled using
 * api_dispatcher_poll().  Will return 0 if no call was dispatched or the ID of
 * the dispatched call otherwise.
 */
api_id_t api_dispatcher_exec();

/*
 * Fill the API buffer with data for l0dable/pycardium startup.
 *
 * The data is a NULL-terminated string.
 */
void api_prepare_args(char *args);
