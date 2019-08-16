Bluetooth Card10 Service
========================

.. warning::
    The specification is still work in progress

The Card10 Service implemented a direct Hardware access of the card10.

BLE Service
-----------

The current draft uses following service specification:

- Service

  UUID: ``42230200-2342-2342-2342-234223422342``

- light sensor characteristic:

  UUID: ``422302f0-2342-2342-2342-234223422342``
  read

- Vibra characteristic:

  UUID: ``4223020f-2342-2342-2342-234223422342``
  write

- Rockets characteristic:

  UUID: ``42230210-2342-2342-2342-234223422342``
  write

light sensor characteristic
---------------------------------

The light sensor characteristic makes it possible to read the current value of the light sensor by receiving a ``uint16``.
The range of this sensor is between 0 (``0x0``) and 400 (``0x9001``).

- reading of ``0x0e00`` means **14**

vibra characteristic
---------------------------------

The vibra characteristic makes it possible to let the card10 for given millisecound in a ``uint16`` vibrate.

- One secound: ``0xe803``

Rockets characteristic
---------------------------------

The Rockets characteristic makes it possible to address every three rockets.
Just write there three byte array, one for evey rocket.

Dataformat:

======= ======= =======
   0       1       2
------- ------- -------
Rocket0 Rocket1 Rocket2
======= ======= =======

- Enable only Rocket0:  ``0xff0000``
- Enable all rockets with 50% brightness: ``0x7f7f7f``
