#include <stdint.h>
#include "api/common.h"

/*
 * Initiate an API call.  This function is used internally by code
 * generated from the API header.
 *
 * Args:
 *   - id: ID of the call to be initiated
 *   - size: Size of the arguments buffer
 *
 * Returns:
 *   - A pointer to the argument buffer which the caller is supposed
 *     to fill.  NULL if an error occured or no buffer of the requested
 *     size is available.
 */
void *_api_call_start(api_id_t id, uintptr_t size);

/*
 * Actually do the API call that was previously initiated using
 * _api_call_start().
 *
 * Args:
 *   - buffer: Pointer to the buffer that was returned by _api_call_start().
 *
 * Returns:
 *   - Pointer to a buffer containing the return value
 */
void *_api_call_transact(void *buffer);

/*
 * Fetch arguments from the API buffer.  This function will only work properly
 * directly after startup of core 1.  If api_fetch_args() is called after other
 * calls have already happened, it will return -1.
 *
 * Otherwise it will return the length of data which was read.
 */
int api_fetch_args(char *buf, size_t cnt);
