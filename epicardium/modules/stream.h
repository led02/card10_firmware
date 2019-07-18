#ifndef STREAM_H
#define STREAM_H

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"

#define STREAM_QUEUE_WAIT pdMS_TO_TICKS(10)

/**
 * **Stream Descriptors**:
 *
 *    All supported streams have to have a unique ID in this list.  :c:macro:`SD_MAX`
 *    must be greater than or equal to the highest defined ID.  Please keep IDs in
 *    sequential order.
 */
enum stream_descriptor {
	/** Highest descriptor must always be ``SD_MAX``. */
	SD_MAX,
};

/**
 * Stream Information Object.
 *
 * This struct contains the information necessary for :c:func:`epic_stream_read`
 * to read from a sensor's stream.  This consists of:
 */
struct stream_info {
	/**
	 * A FreeRTOS queue handle.
	 *
	 * Management of this queue is the sensor drivers responsibility.
	 */
	QueueHandle_t queue;
	/** The size of one data packet (= queue element). */
	size_t item_size;
	/**
	 * An optional function to call before performing the read.
	 *
	 * ``poll_stream()`` is intended for sensors who passively collect data.
	 * A sensor driver might for example retrieve the latest samples in this
	 * function instead of actively polling in a task loop.
	 */
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
