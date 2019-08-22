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

- Time update characteristic:

  UUID: ``42230201-2342-2342-2342-234223422342``
  write no response

- Vibra characteristic:

  UUID: ``4223020f-2342-2342-2342-234223422342``
  write no response

- Rockets characteristic:

  UUID: ``42230210-2342-2342-2342-234223422342``
  write no response

- Background LED Bottom Left characteristic:

  UUID: ``42230211-2342-2342-2342-234223422342``
  write no response

- Background LED Bottom Right characteristic:

  UUID: ``42230212-2342-2342-2342-234223422342``
  write no response

- Background LED Top Right characteristic:

  UUID: ``42230213-2342-2342-2342-234223422342``
  write no response

- Background LED Top Left characteristic:

  UUID: ``42230214-2342-2342-2342-234223422342``
  write no reponse

- LEDS dim bottom characteristic:

  UUID: ``42230215-2342-2342-2342-234223422342``
  write

- LEDs dim top characteristic:

  UUID: ``42230216-2342-2342-2342-234223422342``
  write

- LEDs powersafe characteristic:

  UUID: ``42230217-2342-2342-2342-234223422342``
  write no response

- Flashlight characteristic:

  UUID: ``42230218-2342-2342-2342-234223422342``
  write no response

- LEDs above characteristic:

  UUID: ``42230220-2342-2342-2342-234223422342``
  write no reponse

- Light sensor characteristic:

  UUID: ``422302f0-2342-2342-2342-234223422342``
  read no response

Time update characteristic
---------------------------------

The time update characteristic makes it possible to set the current time given in milliseconds after 1.1.1970 in the UTC timezone. The value is represented as a big endian ``uint64``

- Thu Aug 15 19:40:45 UTC 2019 : ``0x0 0x0 0x1 0x6c 0x96 0xcb 0xf8 0xcc``

Vibra characteristic
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

Background LED <Position> characteristic
----------------------------------------

The Background LEDs <Position> characteristic makes it possible to address the bottom LEDs by position.
Just write there three ``uint8`` for the rgb color.

Dataformat:

===== ======= =======
  0      1       2
----- ------- -------
 red   green   blue
===== ======= =======

- set led blue: ``0x0000ff``
- disabled:  ``0x000000``

LEDs dim <Position> characteristic
----------------------------------

The LEDs dim <Position> characteristic makes it possible to dim LEDs by position.
Just write a ``uint8`` between ``1`` and ``8``.

LEDs powersafe characteristic
---------------------------------

This characteristic makes it possible to set the LEDs in powersafe mode.
Even when set to zero, the RGB LEDs still individually consume ~1mA.
Powersave intelligently switches the supply power in groups.
This introduces delays in the magnitude of ~10µs, so it can be disabled for high speed applications such as POV

- enabled:   ``0x01``
- disabled:  ``0x00``

Flashlight characteristic
---------------------------------

This characteristic makes it possible to activate the flashlight.

- enabled:   ``0x01``
- disabled:  ``0x00``

LEDs above characteristic
---------------------------------
This characteristic set every 11 leds on the top module at once.
By defining 11x rgb from left to right. You need also to set exchange a bigger MTU to use this feature.

- set a rainbow beginnig with red on the right edge: ``0xff0000ff8b00e8ff005dff0000ff2e00ffb900b9ff002eff5d00ffe800ffff008b``

Light sensor characteristic
---------------------------------

The light sensor characteristic makes it possible to read the current value of the light sensor by receiving a ``uint16``.
The range of this sensor is between 0 (``0x0``) and 400 (``0x9001``).

- reading of ``0x0e00`` means **14**
