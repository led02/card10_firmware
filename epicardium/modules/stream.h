#ifndef STREAM_H
#define STREAM_H

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"

/**
 * **Stream Descriptors**:
 *
 *    All supported streams have to have a unique ID in this list.  ``SD_MAX``
 *    must be greater than or equal to the highest defined ID.  Please keep IDs
 *    in sequential order.
 */
enum stream_descriptor {
	/** Highest descriptor must always be ``SD_MAX``. */
	SD_MAX,
};

struct stream_info {
	QueueHandle_t queue;
	size_t item_size;
	int (*poll_stream)();
};

/**
 * Register a stream so it can be read from Epicardium API.
 *
 * :param int sd: Stream Descriptor.  Must be from the above enum.
 * :param stream_info stream: Stream info.
 * :returns: ``0`` on success or a negative value on error.  Possible errors:
 *
 *    - ``-EINVAL``: Out of range sensor descriptor.
 *    - ``-EACCES``: Stream already registered.
 */
int stream_register(int sd, struct stream_info *stream);

/**
 * Deregister a stream.
 *
 * :param int sd:  Stream Descriptor.
 * :param stream_info stream: The stream which should be registered for the
 *    stream ``sd``.  If a different stream is registered, this function
 *    will return an error.
 * :returns: ``0`` on success or a negative value on error.  Possible errors:
 *
 *    - ``-EINVAL``: Out of range sensor descriptor.
 *    - ``-EACCES``: Stream ``stream`` was not registered for ``sd``.
 */
int stream_deregister(int sd, struct stream_info *stream);

/*
 * Initialize stream interface.  Called by main().
 */
int stream_init();

#endif /* STREAM_H */
