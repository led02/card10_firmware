void* api_call_start (uint32_t id, void* args, size_t size)
{
	int ret;
	// aquire semaphore
	ret = SEMA_GetSema (API_CALL_SEMA);

	ApiCallSpace->id = id;
	ApiCallSpace->returning = 0;
	return ApiCallSpace;
}

