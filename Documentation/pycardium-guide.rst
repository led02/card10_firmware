Pycardium Module Development
============================
This is a step-by-step guide on augmenting Pycardium with new modules.  These
module are either written in Python or in C:

* **Python Modules**: Python modules live in ``pycardium/modules/py`` and are
  pre-compiled and packed into Pycardium at build-time.  Python modules are
  meant to be used for creating abstractions and helpers based on lower level
  functionality.  The rationale is that it is easier to write pythonic modules
  in python instead of having to wrap all that in C code.  Though care has to
  be taken as these modules can eat quite a lot of space.

* **C Modules**: C modules allow MicroPython code to interface with the rest of
  the system or allow a much faster implementation of performance-critical
  code. For interfacing with card10, these modules are mostly meant to make use
  of the :ref:`Epicardium API <epicardium_api_overview>`.

Python Modules
--------------
Adding a new Python module
~~~~~~~~~~~~~~~~~~~~~~~~~~
Add a file in ``pycardium/modules/py``.  The name of the file dictates the name
of the module.  Next, add you file to the list in
``pycardium/modules/py/meson.build`` to make the build-system aware of it.
Rebuild, and you should see your module pop up in the list shown by

.. code-block:: python

   help("modules")

Size Consideration
~~~~~~~~~~~~~~~~~~
Python modules can get quite big.  If you want to know exact sizes, take a look at
the output of

.. code-block:: shell-session

   ls -lh build/pycardium/*@@frozen.c@*/

If your module contains some kind of big lookup-table or data-block, consider
pushing that into external flash.

.. todo::

   External flash is not yet accessible by Pycardium (ref `#11`_).

   .. _#11: https://git.card10.badge.events.ccc.de/card10/firmware/issues/11

Creating a new C module
-----------------------
Module Source
~~~~~~~~~~~~~
Create a new file in ``pycardium/modules``.  The basic structure of a C module
looks like this:

.. code-block:: c++

   #include "py/obj.h"

   /* Define a function in this module */
   static mp_obj_t mp_example_hello(mp_obj_t number)
   {
           int num = mp_obj_get_int(number);
           printf("Hello from example: %d\r\n", num);
           return mp_const_none;
   }
   /* Create an object for this function */
   static MP_DEFINE_CONST_FUN_OBJ_1(example_hello_obj, mp_example_hello);

   /* The globals table for this module */
   static const mp_rom_map_elem_t example_module_globals_table[] = {
           {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_example)},
           {MP_ROM_QSTR(MP_QSTR_hello), MP_ROM_PTR(&example_hello_obj)},
   };
   static MP_DEFINE_CONST_DICT(example_module_globals, example_module_globals_table);

   const mp_obj_module_t example_module = {
           .base    = {&mp_type_module},
           .globals = (mp_obj_dict_t*)&example_module_globals,
   };

   /* This is a special macro that will make MicroPython aware of this module */
   MP_REGISTER_MODULE(MP_QSTR_example, example_module, MODULE_EXAMPLE_ENABLED);

Build Integration
~~~~~~~~~~~~~~~~~
Next, you need to add your module to ``pycardium/meson.build``. There is a list
of module sources at the very top called ``modsrc`` where you need to add your
file (e.g. ``modules/example.c``).

QSTR Definitions
~~~~~~~~~~~~~~~~
If you now run ``ninja -C build/``, you will hit a few errors regarding missing
QSTR definitions.  With the example module above, they will look similar to
this:

.. code-block:: text

   ../pycardium/modules/example.c:15:46: error: 'MP_QSTR_example' undeclared here (not in a function)
      15 |  {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_example)},

To fix these errors, you need to add all QSTRs your module needs to
``pycardium/modules/qstrdefs.h``.  Add a section for your module where you
define all QSTRs you need:

.. code-block:: cpp

   /* example */
   Q(example)
   Q(hello)

Each ``Q(...)`` will define into a corresponding ``MP_QSTR_...``.  So
``Q(example)`` corresponds to ``MP_QSTR_example``.

Enable Module
~~~~~~~~~~~~~
The last step is to actually enable inclusion of your module into the firmware.
Do this by adding a define in ``pycardium/mpconfigport.h``:

.. code-block:: cpp

   #define MODULE_EXAMPLE_ENABLED              (1)

The name of the define is the one from the last line in the module source above.

Wrapping Epicardium API
-----------------------
Most modules will probably make use of the :ref:`Epicardium API
<epicardium_api_overview>`. Doing so does not require any extra work, you can
just call the API from your module code.  You should check the input that your
module got from MicroPython before sending data off to Epicardium.  For
example, raise a ``ValueError`` if an integer is too big to fit into the type
specified by the API.  You should also gracefully handle errors returned by API
calls.  As most API calls use *errno* codes, you can just wrap them in an
``OSError``:

.. code-block:: cpp

   int ret = epic_bma_get_accel(&values);

   if (ret < 0) {
          mp_raise_OSError(-ret);
   }

QSTRs
-----
QSTRs are so called “interned strings”. This means they are not allocated like
normal python objects but instead live in flash and are indexed. This allow
MicroPython to very efficiently use them as identifiers. According to them,
comparing two QSTR is as fast as comparing integers.

Unfortunately, the way these QSTRs are collected from the source files is quite
weird.  MicroPython comes with a few python scripts (namely `makeqstrdefs.py`_
and `makeqstrdata.py`_) that parse the C source files and search for uses of
``MP_QSTR_*``.  These are then sorted and indexed into a header file called
``qstrdefs.collected.h``. This is closely tied in with their Makefiles.

As we use our own build system, we had to somehow wrap this generation to work
for us as well. This is done using a few scripts in `lib/micropython`_.
Currently, our build system does **not** parse the module sources to search for
QSTRs.  Instead all QSTRs needed by modules need to be defined in the header
``pycardium/modules/qstrdefs.h``.

.. _makeqstrdefs.py: https://github.com/micropython/micropython/blob/master/py/makeqstrdefs.py
.. _makeqstrdata.py: https://github.com/micropython/micropython/blob/master/py/makeqstrdata.py
.. _lib/micropython: https://git.card10.badge.events.ccc.de/card10/firmware/tree/master/lib/micropython
