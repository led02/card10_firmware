How To Build
============
If you just want to write MicroPython code for card10, you probably **won't**
need to build the firmware yourself.  This page is for people who want to work
on the underlying firmware itself.

Dependencies
------------
* **gcc**, **binutils** & **newlib** for ``arm-none-eabi``:  The packages have
  slightly different names on different distros.

  - Ubuntu: ``apt install gcc-arm-none-eabi binutils-arm-none-eabi libnewlib-arm-none-eabi``
  - Arch: ``pacman -S arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-newlib``
  - Alternative: Download `ARM's GNU toolchain`_.  **TODO**
* **python3**:  For meson and various scripts needed for building.
* **ninja**: Needed for meson.
  - Ubuntu: ``apt install ninja-build``
  - Arch: ``pacman -S ninja``
* **meson** (>0.43.0):  Unfortunately most distros only have very old versions
  of meson in their repositories.  Instead, you'll probably save yourself a lot
  of headaches by installing meson from ``pip3 install --user meson``.
* **python3-crc16**: Install with ``pip3 install --user crc16``.

.. _ARM's GNU toolchain: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

Building
--------
Build using the following two commands:

.. code-block:: shell-session

   $ ./bootstrap.sh
   $ ninja -C build/

``bootstrap.sh`` initializes git submodules and runs *meson*.  Afterwards you
can build with *ninja*.

.. note::

   Our build-system contains a few workarounds around short-comings in meson.
   These workarounds might break on some setups which we did not yet test.  If
   this is the case for you, please open an issue in our `issue tracker`_!

.. _issue tracker: https://git.card10.badge.events.ccc.de/card10/firmware/issues

If ninja succeeds, the binaries are in ``build/``.  They are available in two
formats:  As an ``.elf`` which can be flashed using a debugger and as a
``.bin`` which can be loaded using the provided bootloader.  Here is a list of
the binaries:

- ``build/bootloader/bootloader.elf``: Our bootloader.  It should already be on
  your card10.  The bootloader can only be flashed using a debugger.
- ``build/pycardium/pycardium_epicardium.bin``: The entire firmware in one ``.bin``.
- ``build/epicardium/epicardium.elf``: The core 0 part of the firmware, called Epicardium.
- ``build/pycardium/pycardium.elf``: Our MicroPython port, the core 1 part of the firmware.

In order to do a rebuild you can issue a clean command to ninja via

.. code-block:: shell-session

  $ ninja -C build/ -t clean
