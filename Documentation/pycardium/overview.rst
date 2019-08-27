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

* **screen**: ``sudo screen /dev/ttyACM0 115200``
* **picocom**: ``sudo picocom -b 115200 /dev/ttyACM0``

After connecting, reboot reset the card10 via the power button (left upper
corner) and you should see the output of **menu.py** script (it's located in
*preload/menu.py*). You can press CTRL-C to interrupt the script and jump into
the MicroPython prompt.

To switch on the blue fairy dust you must import the led python module::

   import leds

and power it on::

   leds.set_rocket(0, 31)


REPL modes
^^^^^^^^^^

MicroPython supports a different REPL modes over the serial console. The modes
can be changed on every new line.

Normal mode
"""""""""""
This is the mode you will first see. You can switch to it by pressing CTRL-B.
If you are in a other mode you can return to this mode by pressing CTRL-B too.

Paste mode
""""""""""
You can enter the paste mode by pressing CTRL-E and you can simple copy 'n'
paste your source code into the console and it will be interpreted and executed
line by line. Every new line will be reply by the prompt with **===**.

RAW mode
""""""""
The RAW mode to be intendend for the usage with tools. By pressing CTRL-A you
will enter the RAW REPL mode. The type in code will not printed. By pressing
CTRL-D the whole entered code will be evaluated and executed. The board will
reply with **OK** and print after that the output (print commands) of the code
or give you tracebacks if an error occured.

You can use **pycard10** (tools/pycard10.py) to execute python files from your
PC directly on the card10.


