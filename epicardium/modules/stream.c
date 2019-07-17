#include <string.h>

#include "epicardium.h"
#include "stream.h"

static struct stream_info *stream_table[SD_MAX];

int stream_init()
{
	memset(stream_table, 0x00, sizeof(stream_table));
	return 0;
}

int stream_register(int sd, struct stream_info *stream)
{
	if (sd < 0 || sd >= SD_MAX) {
		return -EINVAL;
	}

	if (stream_table[sd] != NULL) {
		/* Stream already registered */
		return -EACCES;
	}

	stream_table[sd] = stream;
	return 0;
}

int stream_deregister(int sd, struct stream_info *stream)
{
	if (sd < 0 || sd >= SD_MAX) {
		return -EINVAL;
	}

	if (stream_table[sd] != stream) {
		/* Stream registered by someone else */
		return -EACCES;
	}

	stream_table[sd] = NULL;
	return 0;
}

int epic_stream_read(int sd, void *buf, size_t count)
{
	if (sd < 0 || sd >= SD_MAX) {
		return -EBADF;
	}

	struct stream_info *stream = stream_table[sd];
	if (stream == NULL) {
		return -ENODEV;
	}

	/* Poll the stream */
	int ret = stream->poll_stream();
	if (ret < 0) {
		return ret;
	}

	/* Check buffer sizing */
	if (count % stream->item_size != 0) {
		return -EINVAL;
	}

	size_t i;
	for (i = 0; i < count; i += stream->item_size) {
		if (!xQueueReceive(stream->queue, buf + i, 10)) {
			break;
		}
	}

	return i / stream->item_size;
}
