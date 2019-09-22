.. py:module:: utime

``utime`` - Time
================
The ``utime`` module loosely follows CPython's |time|_ module, but is heavily
stripped down.  Instead, it has a few time related functions which are not in
CPython but wouldn't fit anywhere else in our implementation.  Most
prominently,  this is the :py:func:`utime.alarm` function for setting an RTC
alarm.

.. |time| replace:: ``time``
.. _time: https://docs.python.org/3/library/time.html

.. py:function:: sleep(secs)

   Sleep for ``secs`` seconds.  Can take a floating-point value.

.. py:function:: sleep_ms(msecs)

   Sleep for ``msecs`` milliseconds.  Only takes integer values.

.. py:function:: sleep_us(usecs)

   Sleep for ``usecs`` microseconds.  Only takes integer values.

.. py:function:: time()

   Return the current timestamp in seconds since 2000-01-01 00:00 in
   the local timezone.

.. py:function:: time_ms()

   Return the current timestamp in milliseconds since 2000-01-01 00:00 in
   the local timezone.

.. py:function:: monotonic()

   Return a monotonically increasing timestamp.

   .. versionadded:: 1.11

.. py:function:: monotonic_ms()

   Return a monotonically increasing timestamp in milliseconds.

   .. versionadded:: 1.11

.. py:function:: set_time(secs)

   Sets the time to ``secs`` seconds since 2000-01-01 00:00 in the local
   timezone.

   .. versionchanged:: 1.4
      :py:func:`utime.set_time` previously applied a wrong timezone offset,
      thus leading to wrong results.

.. py:function:: set_unix_time(secs)

   Sets the time to ``secs`` seconds since 1970-01-01 00:00 UTC.
   This corresponds to a regular Unix timestamp which can be obtained
   by running ``date +%s`` in a command line or ``int(time.time())``
   in Python.

.. py:function:: localtime([secs])

   Return the current time as a timestruct tuple.  If ``secs`` is given, return
   its timestruct tuple instead.  Timestruct tuple looks like:

   .. code-block:: python

      (year, month, mday, hour, min, sec, wday, yday)
      #   0      1     2     3    4    5     6     7

.. py:function:: mktime(t)

   Convert timestruct tuple into a seconds time stamp.  See
   :py:func:`utime.localtime` for details about timestruct tuples.

   :returns: Seconds since 2000-01-01

.. py:function:: alarm(secs, [callback])

   Register the next RTC alarm for the timestamp ``secs``.  ``secs`` is seconds
   since 2000-01-01.

   If an optional ``callback`` is given, it will be registered for the RTC
   alarm interrupt.  This will overwrite any previous interrupt handler.  If
   ``callback`` is given, :c:func:`utime.alarm` will also enable the RTC alarm
   interrupt.

   **Example**:

   .. code-block:: python

      import utime

      def minute_timer(x):
         current = utime.time()
         print("Current: " + str(current))
         alarm = (current // 60 + 1) * 60
         utime.alarm(alarm, minute_timer)

      minute_timer(None)

   Alternatively, you can register a callback using the interrupt module and
   then call :py:func:`utime.alarm` without a ``callback`` parameter:

   .. code-block:: python

      import interrupt, utime

      def 5_second_timer(x):
         current = utime.time()
         print("Current: " + str(current))
         alarm = (current // 10) * 10 + 5
         utime.alarm(alarm)

      # This time, we need to register and enable the callback manually
      interrupt.set_callback(interrupt.RTC_ALARM, 5_second_timer)
      interrupt.enable_callback(interrupt.RTC_ALARM)

      5_second_timer(None)
