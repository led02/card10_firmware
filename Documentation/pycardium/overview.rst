.. _pycardium_overview:

Overview
========
Pycardium is what we call our MicroPython port for card10.  For the most part,
it will behave like a normal MicroPython, enhanced by a few modules to
interface with card10's hardware.  We have also included modules from Python's
standard library so you have lots of familiar functionality at hand to start
hacking!

.. note::

   Right now, MicroPython is only available via the serial console.  card10
   makes its console available as a CDC ACM device via USB or, if you have a
   debugger, on the debugging serial port.

   This means, you can only interact with Python through the REPL console for
   now. However, this will soon change, once `#10`_ and `#11`_ are implemented.

   .. _#10: https://git.card10.badge.events.ccc.de/card10/firmware/issues/10
   .. _#11: https://git.card10.badge.events.ccc.de/card10/firmware/issues/11

   In other words: It is currently not possible to execute Python code
   from .py files on the filesystem. You have to test new modules using the
   serial console.

Serial Console
--------------
If you have a debugger, you can just use the debuggers serial console.
Otherwise, card10 will open a serial usb-device on its usb-port. On most Linux
systems this device will be called ``/dev/ttyACM0`` or ``/dev/ttyACM1``.

Choose a terminal-emulator of your liking and open the above mentioned device.
Baud-rate is 115200.  Some options are:

* **screen**: ``screen /dev/ttyACM0 115200``
* **picocom**: ``picocom -b 115200 /dev/ttyACM0``

After connecting, reboot card10 and you should see the MicroPython REPL pop up.

.. todo::

   Getting Started Guide for people interested in writing Python code.
