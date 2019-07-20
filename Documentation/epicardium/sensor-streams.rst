Sensor Streams
==============
Sensor drivers can make their data available to core 1 in a stream-like format.
This allows batch-reading many samples and shoud reduce pressure on the
Epicardium API this way.  Sensor streams are read on core 1 using
:c:func:`epic_stream_read`.

This page intends to document how to add this stream interface to a sensor driver.
It also serves as a reference of existing streams.  For that, take a look at the
definitions in the :c:type:`stream_descriptor` enum.

Adding a new Stream
-------------------
The list of possible sensor streams must be known at compile time.  Each stream
gets a unique ID in the :c:type:`stream_descriptor` enum.  Please do not assign
IDs manually but instead let the enum assign sequencial IDs.  :c:macro:`SD_MAX`
must always be the highest stream ID.  Additionally, please document what this
stream is for using a doc-comment so it shows up on this page.

When a sensor driver enables data collection, it should also register its
respective stream.  This is done using a :c:type:`stream_info` object.  Pass
this object to :c:func:`stream_register` to make your stream available.  Your
driver must guarantee the :c:member:`stream_info.queue` handle to be valid until
deregistration using :c:func:`stream_deregister`.

Definitions
-----------

.. c:autodoc:: epicardium/modules/stream.h
