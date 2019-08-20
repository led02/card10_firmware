.. py:module:: personal_state

``personal_state`` - Personal State
===================================
The :py:mod:`personal_state` module allows you to set and get the card10 users personal state from your script. The personal state is displayed on the top-left LED on the bottom of the harmonics board. While the personal state is set the LED can't be controlled by the :py:mod:`leds` module.

**Example**:

.. code-block:: python

   import personal_state

   # Enable the "camp" state only while the app is running.
   personal_state.set(personal_state.CAMP, False)

   # Enable the "chaos" state and keep it after the app exits.
   personal_state.set(personal_state.CHAOS, True)

   # Query the currently configured state and if it's persistent.
   state, persistent = personal_state.get()

   # Clear the currently configured state
   personal_state.clear()

.. py:function:: personal_state.set(state, persistent)

   Set the users personal state.

   :param int state: ID of the personal state to set. Must be one of :py:data:`personal_state.NO_CONTACT`, :py:data:`personal_state.CHAOS`, :py:data:`personal_state.COMMUNICATION`, :py:data:`personal_state.CAMP`.
   :param int persistent: Controls whether the personal state is persistent. A persistent state is not reset when the pycardium application is changed or restarted. In persistent mode the personal state LED is not controllable by the pycardium application.

.. py:function:: personal_state.clear()

   Clears a previously set personal state.
   
   If no personal state was set this function does nothing. It does not matter
   if a set state is marked as persistent or not.

.. py:function:: personal_state.get()

   Get the users personal state.
   
   :returns: A tuple containing the currently set state and a boolean indicating if it's persistent or not.

.. py:data:: personal_state.NO_STATE

   State ID reported when no personal state is set.

.. py:data:: personal_state.NO_CONTACT

   State ID for the "No Contact" personal state.

.. py:data:: personal_state.CHAOS

   State ID for the "Chaos" personal state.

.. py:data:: personal_state.COMMUNICATION

   State ID for the "Communicatoin" personal state.
   
.. py:data:: personal_state.CAMP

   State ID for the "Camp" personal state.
