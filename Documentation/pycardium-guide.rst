.. _pycardium_guide:

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

Functions for MicroPython
-------------------------
As shown in the example above, you can create functions that can be called from
MicroPython code.  These functions always have one of the following signatures.
To create a MicroPython object for a function, you need the macro call shown
after each signature.  Please place these calls directly after the function
body as shown above.

.. code-block:: cpp

   /* Function with 0 arguments */
   mp_obj_t mp_example_fun0(void);
   static MP_DEFINE_CONST_FUN_OBJ_0(example_fun0_obj, mp_example_fun0);

   /* Function with 1 argument */
   mp_obj_t mp_example_fun1(mp_obj_t arg0_in);
   static MP_DEFINE_CONST_FUN_OBJ_1(example_fun1_obj, mp_example_fun0);

   /* Function with 2 arguments */
   mp_obj_t mp_example_fun2(mp_obj_t arg0_in, mp_obj_t arg1_in);
   static MP_DEFINE_CONST_FUN_OBJ_2(example_fun2_obj, mp_example_fun0);

   /* Function with 3 arguments */
   mp_obj_t mp_example_fun3(mp_obj_t arg0_in, mp_obj_t arg1_in, mp_obj_t arg2_in);
   static MP_DEFINE_CONST_FUN_OBJ_3(example_fun3_obj, mp_example_fun0);

   /* Function with 4 or more arguments */
   mp_obj_t mp_example_fun4(size_t n_args, mp_obj_t *args);
   static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(example_fun4_obj, 4, 4, mp_example_fun4);

For functions with 4 or more arguments, you need to use the
``MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN`` macro and instead of directly accessing
the arguments, you get an array.  The macro gets two numbers (they are the same
in the example above):  The minimum and maximum number of arguments.

MicroPython Objects
-------------------
**TL;DR**: Look at |obj.h|_.  It contains most functions needed to create,
access, and modify MicroPython objects.

.. |obj.h| replace:: ``lib/micropython/micropython/py/obj.h``
.. _obj.h: https://github.com/micropython/micropython/blob/master/py/obj.h

For C modules to interface with MicroPython, you need to be able to interface
with MicroPython objects.  The generic type of an object is ``mp_obj_t``.  As
you can see in the example above, this is also what a function gets as
arguments and returns back to MicroPython.  You can cast ``mp_obj_t`` to
concrete object types if you made sure it is actually the correct type.

Booleans
~~~~~~~~
``True`` and ``False`` are *const* objects, called ``mp_const_true`` and
``mp_const_false`` respectively.  A usual equality check can be used:

.. code-block:: cpp

   #include "py/obj.h"
   #include "py/runtime.h"

   if (bool_obj == mp_const_true) {
       /* is a boolean true */
   } else if (bool_obj == mp_const_false) {
       /* is a boolean false */
   } else {
       mp_raise_TypeError("arg 0 is not a boolean");
   }

Integers
~~~~~~~~
As long as your integers stay within **31**-bit limits, integers are stored very
efficiently and can be accessed and created like this:

.. code-block:: cpp

   #include "py/obj.h"

   /* Create a new integer which is < 2^31 */
   mp_obj_t int_obj = MP_OBJ_NEW_SMALL_INT(0xc0ffee);

   /* Check if an integer is small and if so, extract it */
   if (mp_obj_is_small_int(int_obj)) {
           int int_value = MP_OBJ_SMALL_INT_VALUE(int_obj);
   }

For bigger integers or if you are uncertain about the limits, use the following
functions:

.. code-block:: cpp

   #include "py/obj.h"

   /* Create new integer objects in various sizes and signedness */
   mp_obj_t int0_obj = mp_obj_new_int((mp_int_t)value);
   mp_obj_t int1_obj = mp_obj_new_int_from_uint((mp_uint_t)value);
   mp_obj_t int2_obj = mp_obj_new_int_from_ll((long long)value);
   mp_obj_t int3_obj = mp_obj_new_int_from_ull((unsigned long long)value);

   /* Check if a value is an integer */
   if (mp_obj_is_integer(int_obj)) {

           /* Get an integer */
           int int0 = mp_obj_get_int(int_obj);
           int int0 = mp_obj_get_int_truncated(int_obj);
   }

   int value;
   if (!mp_obj_get_int_maybe(int_obj, &value)) {
      /* Not an integer! */
   }

To get really big integers you have to use

.. code-block:: cpp

   #include "py/obj.h"
   #include "py/objint.h"

   long long value;
   mp_obj_int_to_bytes_impl(int_obj, 8, (byte*)&value);

Strings
~~~~~~~
As in CPython, MicroPython also has multiple string representations:  There is
``str``, ``bytes``, and ``bytearray`` but also the above mentions ``QSTR``\ s.
Ideally, code should work with as many of these as possible and to ensure this,
please use these generic functions:

.. code-block:: cpp

   #include "py/obj.h"

   /* Create a new string object */
   char buf[] = "Hello MicroPython!";
   mp_obj_t str_obj = mp_obj_new_str(buf, sizeof(buf));

   /* Check if an object is a string */
   if (mp_obj_is_str(str_obj)) {
           /* Either str or QSTR */
   }
   if (mp_obj_is_str_or_bytes) {
           /* Either str, QSTR, or bytes */
   }

   /*
    * Get a char array from a string object.
    * CAUTION! This string is not necessarily null terminated!
    */
   char *str_data;
   size_t str_len;
   str_data = mp_obj_str_get_data(str_obj, &str_len);

Lists & Tuples
~~~~~~~~~~~~~~
While lists and tuples can be accessed using type specific functions, you
should refrain from doing so:  Use generic indexing functions instead, as they
also allow types derived from lists or tuples or types with custom indexing
implementations (i.e. duck-typing).

.. code-block:: cpp

   #include "py/obj.h"
   #include "py/runtime.h"

   /* Get the length of a list-like object */
   mp_int_t len = mp_obj_get_int(mp_obj_len(list_obj));

   /* Get element at a specific index */
   mp_obj_t elem5 = mp_obj_subscr(
           list_obj,
           MP_OBJ_NEW_SMALL_INT(5),
           MP_OBJ_SENTINEL
   );

To create a list or tuple:

.. code-block:: cpp

   mp_obj_t values[] = {
           MP_OBJ_NEW_SMALL_INT(0xde),
           MP_OBJ_NEW_SMALL_INT(0xad),
           MP_OBJ_NEW_SMALL_INT(0xbe),
           MP_OBJ_NEW_SMALL_INT(0xef),
   };

   /* Create a tuple */
   mp_obj_t tuple = mp_obj_new_tuple(4, values);

   /* Create a list */
   mp_obj_t list = mp_obj_new_list(4, values);
