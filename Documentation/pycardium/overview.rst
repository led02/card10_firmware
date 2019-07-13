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

   This means, you can only interact with Python through a REPL for now.
   However, this will soon change, once `#10`_ and `#11`_ are implemented.

   .. _#10: https://git.card10.badge.events.ccc.de/card10/firmware/issues/10
   .. _#11: https://git.card10.badge.events.ccc.de/card10/firmware/issues/11

.. todo::

   Getting Started Guide for people interested in writing Python code.
