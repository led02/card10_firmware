``vibra`` - Vibration Motor
===========================

.. py:function:: vibra.vibrate(millis)

   Turn on the vibration motor for a certain duration.

   **Example**:

   .. code-block:: python

      import vibra

      # Turn on vibration motor for 60 ms
      vibra.vibrate(60)

   :param int millis:  Duration for the vibration motor to be on.


.. py:function:: vibra.set(state)

   Permanently set the state of the vibration motor to either *on* or *off*.

   .. warning::

      If your code for some reason crashes between turning the motor on and
      back off again, the motor will continue running.  Consider using
      :py:func:`vibra.vibrate` instead.

   :param bool state: ``True`` to turn on, ``False`` to turn the motor off.
