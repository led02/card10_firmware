How To Flash
============
Depending on whether you have a debugger or not, you have to use a different
method of flashing:

Flash Without Debugger
----------------------

If you do not have a debugger, you have to update the firmware using our
bootloader by going into :ref:`usb_file_transfer`.

After you get your badge into :ref:`usb_file_transfer`, you can drop the firmware's
``.bin`` (from ``build/pycardium/pycardium_epicardium.bin`` in most cases) into
this flash-storage.  You **must** call the file ``card10.bin`` for the
bootloader to use it.

The bootloader will then display ``Writing.`` in red while it is actually
writing the file to external flash.  Please wait until it displays ``Ready.``
again before resetting card10 by pressing the power button again.

Flash Using Debugger
--------------------

First, setup everything as explained on the :ref:`debugger` page.  Following
that and after connecting to card10, you can flash your binary using the
``load`` command.  After loading, you need to use ``reset`` to reboot card10
using your new firmware.

.. warning::

   With the current version of the bootloader, before attempting to flash using
   the debugger, make sure there is no ``card10.bin`` stored on the device.
   If there is, the bootloader will overwrite whatever you just flashed after
   reboot every time.

.. code-block:: text

   (gdb) load
   Loading section .text, size 0x12514 lma 0x10010000
   Loading section .ARM.exidx, size 0x8 lma 0x10022514
   Loading section .data, size 0x8d8 lma 0x1002251c
   Start address 0x10012160, load size 77300
   Transfer rate: 19 KB/sec, 11042 bytes/write.
   (gdb)

.. note::

   If OpenOCD was able to connect, but GDB gives you an

   .. code-block:: text

      Error erasing flash with vFlashErase packet

   error, issue a ``reset`` command, quickly followed by a ``load`` command.

   Reason: The Epicardium puts parts of the CPU to sleep and the debugging
   interface is part of that. After a reset the bootloader starts up
   and lets OpenOCD/GDB take control again.
