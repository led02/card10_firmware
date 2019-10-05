``display`` - Display
=====================

The display module let's you draw on the card10's display.
Pixels are addressed from top left to bottom right with a range of x: 0 to 159 and y: 0 to 79.

.. code-block:: text

   0,0
      +---------------------+
      |                     |
      |                     |
      |                     |
      |                     |
      +---------------------+
                           159,79

Drawing operations are clipped, pixels outside of the screen will be ignored.

.. automodule:: display
   :members:
