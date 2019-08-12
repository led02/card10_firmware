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

.. py:function:: utime.sleep(secs)

   Sleep for ``secs`` seconds.  Can take a floating-point value.

.. py:function:: utime.sleep_ms(msecs)

   Sleep for ``msecs`` milliseconds.  Only takes integer values.

.. py:function:: utime.sleep_us(usecs)

   Sleep for ``usecs`` microseconds.  Only takes integer values.

.. py:function:: utime.time()

   Return the current timestamp in seconds since 2000-01-01

.. py:function:: utime.localtime([secs])

   Return the current time as a timestruct tuple.  If ``secs`` is given, return
   its timestruct tuple instead.  Timestruct tuple looks like:

   .. code-block:: python

      (year, month, mday, hour, min, sec, wday, yday)
      #   0      1     2     3    4    5     6     7

.. py:function:: utime.mktime(t)

   Convert timestruct tuple into a seconds time stamp.  See
   :py:func:`utime.localtime` for details about timestruct tuples.

   :returns: Seconds since 2000-01-01

.. py:function:: utime.alarm(secs, [callback])

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
