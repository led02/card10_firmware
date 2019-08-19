.. py:module:: gpio

``gpio`` - GPIO Pins
==========================
The :py:mod:`gpio` module allows you to use card10's GPIO pins as input and
output in your scripts.

**Example**:

.. code-block:: python

   import gpio

   gpio.set_mode(gpio.WRISTBAND_1, gpio.mode.OUTPUT)
   gpio.write(gpio.WRISTBAND_1, True)

   gpio.set_mode(gpio.WRISTBAND_2, gpio.mode.INPUT | gpio.mode.PULL_UP)
   state = gpio.read(gpio.WRISTBAND_2)
   print("State of Wristband pin 2:", state)

.. py:function:: gpio.set_mode(pin, mode)

   Configure GPIO pin state.

   :param int pin: ID of the pin to be configured.
   :param int mode: An integer with the bits for the wanted mode set. Create your
      integer by ORing :py:data:`gpio.mode.OUTPUT`, :py:data:`gpio.mode.INPUT`,
      :py:data:`gpio.mode.PULL_UP`, :py:data:`gpio.mode.PULL_DOWN`.

.. py:function:: gpio.get_mode(pin)

   Get GPIO pin state.

   :param int pin: ID of the pin of to get the mode of.
   :returns: An integer with the configure mode bits set.

.. py:function:: gpio.write(pin, value)

   Write a value to a GPIO pin.

   :param int pin: ID of the pin of to get the mode of.
   :param bool value: New pin value.

.. py:function:: gpio.read(pin)

   Read GPIO pin value.

   :param int pin: ID of the pin of to get the mode of.
   :returns: Current value of the GPIO pin.

.. py:data:: gpio.WRISTBAND_1

   Pin ID for Wristband GPIO 1.

.. py:data:: gpio.WRISTBAND_2

   Pin ID for Wristband GPIO 2.

.. py:data:: gpio.WRISTBAND_3

   Pin ID for Wristband GPIO 3.

.. py:data:: gpio.WRISTBAND_4

   Pin ID for Wristband GPIO 4.

.. py:data:: gpio.mode.OUTPUT

   Configures a pin as output.

.. py:data:: gpio.mode.INPUT

   Configures a pin as input.

.. py:data:: gpio.mode.PULL_UP

   Enables the internal pull-up resistor of a pin.

.. py:data:: gpio.mode.PULL_DOWN

   Enables the internal pull-down resistor of a pin.
