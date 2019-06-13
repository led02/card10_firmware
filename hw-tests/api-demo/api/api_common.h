#define API_CALL_SEMA 0

struct api_call
{
	uint32_t id;
	uint8_t returning;
	unsigned char *buf;
};

static struct api_call **ApiCallSpace = (void*)0x20080001;
