How To Flash
============
Depending on whether you have a debugger or not, you have to use a different
method of flashing:

Flash Without Debugger
----------------------
If you do not have a debugger, you have to update the firmware using our
bootloader.  To do so, you need to reboot card10 while keeping the buttom on
the bottom right pressed.  Rebooting is done by either short pressing the power
button (top left) while you have a working firmware, or turning the card10 off
completely (by pressing the power button for 8 seconds) and then starting it again.

.. image:: static/bootloader-buttons.png

If you did everything correctly, the bootloader will display:

.. code-block:: text

   Bootloader
   Jul 12 2019
   USB activated.
    Waiting.

On your host, you should now see an 8MB flash-device appear.  You can now drop
the firmware's ``.bin`` (from ``build/pycardium/pycardium_epicardium.bin`` in
most cases) into this flash-storage.  You **must** call the file ``card10.bin``
for the bootloader to use it.

Afterwards **eject** the flash device and reboot card10.  You should now see
your new firmware boot up!

.. warning::

   **You must EJECT the flash device!**  ``umount`` & ``sync`` is **not**
   enough and will result in the bootloader not loading the new binary.

Flash Using Debugger
--------------------

.. warning::

   With the current version of the bootloader, before attempting to flash using
   the debugger, make sure there is no ``card10.bin`` stored on the device.
   If there is, the bootloader will overwrite whatever you just flashed after
   reboot every time.

First, setup everything as explained on the :ref:`debugger` page.  Following
that and after connecting to card10, you can flash your binary using the
``load`` command.  After loading, you need to use ``reset`` to reboot card10
using your new firmware.


.. info::
   If OpenOCD was able to connect, but GDB gives you an
   ``Error erasing flash with vFlashErase packet`` error, issue a ``reset``
   command, quickly followed by a ``load`` command.

   Reason: The Epicardium puts parts of the CPU to sleep and the debugging
   interface is part of that. After a reset the bootloader starts up
   and lets OpenOCD/GDB take control again.

.. code-block:: text

   (gdb) load
   Loading section .text, size 0x12514 lma 0x10010000
   Loading section .ARM.exidx, size 0x8 lma 0x10022514
   Loading section .data, size 0x8d8 lma 0x1002251c
   Start address 0x10012160, load size 77300
   Transfer rate: 19 KB/sec, 11042 bytes/write.
   (gdb)
