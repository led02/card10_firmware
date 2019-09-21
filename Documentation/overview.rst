.. _firmware_overview:

Overview
========
To make the most of card10's dual-core processor, its firmware will have been
divided into two parts: The "main" firmware running on core 0 which will have
been called *Epicardium* and the "user-code" running on core 1.  In most cases
this will have been *Pycardium*, our MicroPython port.

.. image:: ./static/overview.svg

Epicardium
----------
Epicardium is based on `FreeRTOS <https://www.freertos.org/>`_.  There are a
number of tasks that will have been keeping card10 running.  These are:

+-------------------+-------------------------------+----------+-------------------------------------------+
| Name              | ID Global                     | Priority | Description                               |
+===================+===============================+==========+===========================================+
| `vPmicTask`_      | ``pmic_task_id`` (static)     | +4       | Power Management (and Reset Button)       |
+-------------------+-------------------------------+----------+-------------------------------------------+
| `vLifecycleTask`_ | ``lifecycle_task`` (static)   | +3       | Control of the payload running on core 1. |
+-------------------+-------------------------------+----------+-------------------------------------------+
| `vBleTask`_       | ``ble_task_id`` (static)      | +3       | Bluetooth Low Energy Stack                |
+-------------------+-------------------------------+----------+-------------------------------------------+
| `vSerialTask`_    | ``serial_task_id``            | +3       | Serial Output via UART/CDC-ACM/BLE        |
+-------------------+-------------------------------+----------+-------------------------------------------+
| `vApiDispatcher`_ | ``dispatcher_task_id``        | +2       | Epicardium API dispatcher                 |
+-------------------+-------------------------------+----------+-------------------------------------------+
| `vLedTask`_       | -/-                           | +1       | LED Animations                            |
+-------------------+-------------------------------+----------+-------------------------------------------+
| `vMAX30001Task`_  | ``max30001_task_id`` (static) | +1       | `MAX30001`_ ECG driver                    |
+-------------------+-------------------------------+----------+-------------------------------------------+
| `vBhi160Task`_    | ``bhi160_task_id`` (static)   | +1       | `BHI160`_ sensor fusion driver            |
+-------------------+-------------------------------+----------+-------------------------------------------+

.. _vPmicTask: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/modules/pmic.c#L281
.. _vLifecycleTask: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/modules/lifecycle.c#L361
.. _vBleTask: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/ble/ble.c#L237
.. _vSerialTask: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/modules/serial.c#L289
.. _vApiDispatcher: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/modules/dispatcher.c#L25
.. _vLedTask: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/modules/personal_state.c#L58
.. _vMAX30001Task: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/modules/max30001.c#L378
.. _vBhi160Task: https://git.card10.badge.events.ccc.de/card10/firmware/blob/master/epicardium/modules/bhi.c#L419

.. _MAX30001: https://www.maximintegrated.com/en/products/analog/data-converters/analog-front-end-ics/MAX30001.html
.. _BHI160: https://www.bosch-sensortec.com/bst/products/all_products/bhi160

Epicardium API
--------------
Epicardium exposes lots of functionality via the *Epicardium API*.  The
technical details of this API can be found in this :ref:`overview
<epicardium_api_overview>`.  If you are interested in adding new API calls,
you should probably read the :ref:`epicardium_api_guide` guide.

Pycardium
---------
Pycardium is our MicroPython fork.  Its purpose is to make it as easy as
possible to interact with card10.  If you are interested in working on
Pycardium, take a look at the :ref:`pycardium_guide` guide.

L0dables
--------
Next to Pycardium, other bare-metal code can also run on core 1.  For example,
a `Rustcardium`_ or C-cardium.  These l0dables must be compiled using our special
linker script and should link against the api-caller library so they can
interface with the :ref:`epicardium_api`.
Note: this feature is disabled by default and has to be enabled at build time.
To do this, run ``bootstrap.sh`` with the option ``-Djailbreak_card10=true``
and rebuild the firmware as described in :ref:`how_to_build`.

.. _Rustcardium: https://git.card10.badge.events.ccc.de/astro/rust-card10

.. todo::

   Provide more details how this works

Program Flow Diagram
--------------------
The following diagram is a rough overview of the program flow in this fimware:

.. image:: ./static/firmware-flow.svg
