#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "epicardium.h"
#include "modules/log.h"
#include "modules/stream.h"

/* Internal buffer of registered streams */
static struct stream_info *stream_table[SD_MAX];

/* Lock for modifying the stream info table */
static StaticSemaphore_t stream_table_lock_data;
static SemaphoreHandle_t stream_table_lock;

int stream_init()
{
	memset(stream_table, 0x00, sizeof(stream_table));
	stream_table_lock =
		xSemaphoreCreateMutexStatic(&stream_table_lock_data);
	return 0;
}

int stream_register(int sd, struct stream_info *stream)
{
	int ret = 0;
	if (xSemaphoreTake(stream_table_lock, STREAM_MUTEX_WAIT) != pdTRUE) {
		LOG_WARN("stream", "Lock contention error");
		ret = -EBUSY;
		goto out;
	}

	if (sd < 0 || sd >= SD_MAX) {
		ret = -EINVAL;
		goto out_release;
	}

	if (stream_table[sd] != NULL) {
		/* Stream already registered */
		ret = -EACCES;
		goto out_release;
	}

	stream_table[sd] = stream;

out_release:
	xSemaphoreGive(stream_table_lock);
out:
	return ret;
}

int stream_deregister(int sd, struct stream_info *stream)
{
	int ret = 0;
	if (xSemaphoreTake(stream_table_lock, STREAM_MUTEX_WAIT) != pdTRUE) {
		LOG_WARN("stream", "Lock contention error");
		ret = -EBUSY;
		goto out;
	}

	if (sd < 0 || sd >= SD_MAX) {
		ret = -EINVAL;
		goto out_release;
	}

	if (stream_table[sd] != stream) {
		/* Stream registered by someone else */
		ret = -EACCES;
		goto out_release;
	}

	stream_table[sd] = NULL;

out_release:
	xSemaphoreGive(stream_table_lock);
out:
	return ret;
}

int epic_stream_read(int sd, void *buf, size_t count)
{
	int ret = 0;
	/*
	 * TODO: In theory, multiple reads on different streams can happen
	 * simulaneously.  I don't know what the most efficient implementation
	 * of this would look like.
	 */
	if (xSemaphoreTake(stream_table_lock, STREAM_MUTEX_WAIT) != pdTRUE) {
		LOG_WARN("stream", "Lock contention error");
		ret = -EBUSY;
		goto out;
	}

	if (sd < 0 || sd >= SD_MAX) {
		ret = -EBADF;
		goto out_release;
	}

	struct stream_info *stream = stream_table[sd];
	if (stream == NULL) {
		ret = -ENODEV;
		goto out_release;
	}

	/* Poll the stream, if a poll_stream function exists */
	if (stream->poll_stream != NULL) {
		int ret = stream->poll_stream();
		if (ret < 0) {
			goto out_release;
		}
	}

	/* Check buffer size is a multiple of the data packet size */
	if (count % stream->item_size != 0) {
		ret = -EINVAL;
		goto out_release;
	}

	size_t i;
	for (i = 0; i < count; i += stream->item_size) {
		if (!xQueueReceive(stream->queue, buf + i, 0)) {
			break;
		}
	}

	ret = i / stream->item_size;

out_release:
	xSemaphoreGive(stream_table_lock);
out:
	return ret;
}
