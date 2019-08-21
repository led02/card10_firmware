.. py:module:: buttons

``buttons`` - Push Buttons
==========================
The :py:mod:`buttons` module allows you to use card10's push buttons as input
in your scripts.

**Example**:

.. code-block:: python

   import buttons

   print("Press bottom left or right button:")

   while True:
      pressed = buttons.read(
          buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT
      )

      if pressed != 0:
         break

   if pressed & buttons.BOTTOM_LEFT != 0:
      print("Left button pressed!")

   if pressed & buttons.BOTTOM_RIGHT != 0:
      print("Right button pressed!")

.. py:function:: read(mask)

   Read button status.

   :param int mask: Mask of buttons to check.  Create the mask by ORing
      :py:data:`buttons.BOTTOM_LEFT`, :py:data:`buttons.BOTTOM_RIGHT`,
      :py:data:`buttons.TOP_RIGHT`, and :py:data:`buttons.TOP_LEFT` (=
      :py:data:`buttons.RESET`).
   :returns: An integer with the bits for pressed buttons set.  Use the same
      costants as for the mask to check which buttons were pressed.

.. py:data:: BOTTOM_LEFT

   Bottom left button.

.. py:data:: BOTTOM_RIGHT

   Bottom right button.

.. py:data:: TOP_RIGHT

   Top right button.

.. py:data:: TOP_LEFT

   Top left button (Reset button).

.. py:data:: RESET

   Top left button (Reset button).
