MicroPython Standard Library
============================
Pycardium contains some modules from the MicroPython standard library.  These
are:

.. py:module:: ubinascii

``ubinascii``
-------------
Refer to the official `MicroPython docs for ubinascii`_.

.. _MicroPython docs for ubinascii: http://docs.micropython.org/en/latest/library/ubinascii.html

.. py:module:: ucollections

``ucollections``
----------------

.. py:function:: namedtuple(...)

   See the official `MicroPython docs for namedtuple`_ for details.

   .. _MicroPython docs for namedtuple: http://docs.micropython.org/en/latest/library/ucollections.html#ucollections.namedtuple

.. py:module:: uerrno

``uerrno``
----------
Refer to the offical `MicroPython docs for uerrno`_.

.. _MicroPython docs for uerrno: http://docs.micropython.org/en/latest/library/uerrno.html

.. py:module:: uheapq

``uheapq``
----------
Refer to the offical `MicroPython docs for uheapq`_.

.. _MicroPython docs for uheapq: http://docs.micropython.org/en/latest/library/uheapq.html

.. py:module:: uio

``uio``
-------
Refer to the offical `MicroPython docs for uio`_.

.. _MicroPython docs for uio: http://docs.micropython.org/en/latest/library/uio.html

.. py:module:: ujson

``ujson``
---------
Refer to the offical `MicroPython docs for ujson`_.

.. _MicroPython docs for ujson: http://docs.micropython.org/en/latest/library/ujson.html

.. py:module:: urandom

``urandom``
-----------
Pseudo-random number generator.

.. py:function:: choice(seq)

   Return a random element from the non-empty sequence ``seq``.

.. py:function:: getrandbits(k)

   Returns a Python integer with ``k`` random bits.

.. py:function:: randint(a, b)

   Return a random integer ``N`` such that ``a <= N <= b``. Alias for
   :py:func:`randrange(a, b+1) <urandom.randrange>`.

.. py:function:: random()

   Return the next random floating point number in the range [0.0, 1.0).

.. py:function:: randrange(start, stop, [step])

.. py:function:: randrange(stop)
   :noindex:

   Return a randomly selected element from ``range(start, stop, step)``. This
   is equivalent to ``urandom.choice(range(start, stop, step))``, but doesn’t
   actually build a range object.

   The positional argument pattern matches that of ``range()``. Keyword
   arguments should not be used because the function may use them in unexpected
   ways.

.. py:function:: seed(n)

   Seed the pseudo-random number generator from ``n``.

   .. note::

      CPython does not provide a :py:func:`seed` function.  This is a
      difference in the MicroPython implementation.

.. py:function:: uniform(a, b)

   Return a random floating point number ``N`` such that ``a <= N <= b`` for
   ``a <= b`` and ``b <= N <= a`` for ``b < a``.

   The end-point value ``b`` may or may not be included in the range depending
   on floating-point rounding in the equation ``a + (b-a) * random()``.

.. py:module:: ure

``ure``
-------
Minimal regular expression library.  Refer to the offical `MicroPython docs for ure`_.

.. _MicroPython docs for ure: http://docs.micropython.org/en/latest/library/ure.html

.. py:module:: ustruct

``ustruct``
-----------
Refer to the offical `MicroPython docs for ustruct`_.

.. _MicroPython docs for ustruct: http://docs.micropython.org/en/latest/library/ustruct.html

``utime``
---------
``utime`` contains non-standard functions as well.  Please refer to our
dedicated :py:mod:`utime` docs.


Python Standard Library
=======================
Additionally to the MicroPython module, Pycardium contains a subset of the
CPython standard library, as implemented by `micropython-lib`_.  The following
modules are included:

.. _micropython-lib: https://github.com/micropython/micropython-lib

.. py:module:: collections

``collections``
---------------
Collections module.

.. py:module:: contextlib

``contextlib``
--------------
Contextlib module.

.. py:module:: functools

``functools``
-------------
Functools module.

.. py:module:: itertools

``itertools``
-------------
Itertools module.

.. warning::

   :py:func:`itertools.tee` is not implemented correctly.

.. py:module:: string

``string``
----------
String module.

.. py:module:: struct

``struct``
----------
Struct module.

.. py:module:: uuid

``uuid``
--------

.. py:class:: UUID(hex=None, bytes=None, int=None, version=None)

   Create a new UUID object.

   Exactly one of ``hex``, ``bytes``, or ``int`` must be given.  The
   ``version`` argument is optional; if given, the resulting UUID will have its
   variant and version set according to RFC 4122, overriding the given ``hex``,
   ``bytes``, or ``int``.

   **Examples**:

   .. code-block:: python

      UUID('{12345678-1234-5678-1234-567812345678}')
      UUID('12345678123456781234567812345678')
      UUID('urn:uuid:12345678-1234-5678-1234-567812345678')
      UUID(bytes='\x12\x34\x56\x78' * 4)
      UUID(int=0x12345678123456781234567812345678)

   .. versionadded:: 1.10

   .. py:attribute:: bytes

      UUID as ``bytes()`` object

   .. py:attribute:: node

      Node of this UUID

   .. py:attribute:: hex

      Hex-String representation of this UUID

   .. py:attribute:: version

      UUID version accordiung to RFC 4122

.. py:function:: uuid4():

   Generate a new UUID version 4 (random UUID).

   .. versionadded:: 1.10
