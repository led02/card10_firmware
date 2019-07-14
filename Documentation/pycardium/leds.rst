``leds`` - LEDs
===============

.. py:function:: leds.set(led, color)

   Set one of the card10's RGB LEDs to a certain color.

   Example which sets all LEDs on the top to red:::

      import leds, color

      for i in range(11):
         leds.set(i, color.RED)

   :param led:  Which led to set.  0-10 are the leds on the top and 11-14 are the 4 "ambient" leds.
   :param color:  What color to set the led to.  Should be a :py:class:`color.Color` but any list/tuple
      with 3 elements will work just as well.

.. py:data:: leds.BOTTOM_RIGHT

   Index of the LED in the bottom right of card10.

.. py:data:: leds.BOTTOM_LEFT

   Index of the LED in the bottom left of card10.

.. py:data:: leds.TOP_RIGHT

   Index of the LED in the top right of card10.

.. py:data:: leds.TOP_LEFT

   Index of the LED in the top left of card10.
