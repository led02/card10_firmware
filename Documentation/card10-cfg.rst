.. _card10_cfg:

card10.cfg
==========

Certain high-level settings can be configured using a filed named ``card10.cfg``.  It is accessed from the :ref:`usb_file_transfer` of the bootloader.  Once you are in this mode and have mounted the badge's flash device, you can either create or update a file named ``card10.cfg``.

The file is in the well-known INI-style format, with one setting per line. For instance, if there were an option called ``answer_to_life``, you could set it by writing the following line in the ``card10.cfg`` file:

.. code-block:: text

   answer_to_life = 42

Don't forget to unmount the filesystem before rebooting your badge after changing any setting.

Syntax and Types
----------------

Lines that start with a ``#`` character are ignored.

Any other line will have the overall syntax of ``option_name = option_value``, with spaces around the ``=`` character optional.

Option names are internal to card10 and described below. Each option has a defined type.

========= ===========
Type name Description
========= ===========
Boolean   A true/false value. ``1`` or ``true`` is true, ``0`` or ``false`` is false. Example: ``foo = true``.
String    An unquoted string value of maximum 20 bytes. Values longer than 20 bytes are trimmed. Example: ``foo = bar``.
Integer   A signed 32-bit integer in base 10. Example: ``foo = 42`` or ``bar = -1337``.
Float     A single-precision (32-bit) floating-point number in base 10. Example: ``foo = 13.37``.
========= ===========

Supported options
-----------------

=============== ========== ===========
Option name     Type       Description
=============== ========== ===========
``execute_elf`` Boolean    Allow running of binary :ref:`l0dables`. These files can be nefarious, so this option is off by default.
=============== ========== ===========
