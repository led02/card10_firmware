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
