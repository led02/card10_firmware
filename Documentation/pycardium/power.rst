.. py:module:: Power

``power`` - PMIC power module handling
======================================
The :py:mod:`power` module allows you to read the card10's power status
in your scripts.

**Example**:

.. code-block:: python

   import power

   print(power.read_battery_voltage())

.. py:function:: read_battery_voltage()

   Read the battery voltage in V.  Please keep in mind that battery
   voltage behaves exponentially when interpreting this value.

   .. warning::

      Card10 will hard-shutdown once the voltage drops below 3.4 V

.. py:function:: read_battery_current()

   Read the battery-side current flow in A.

.. py:function:: read_chargin_voltage()

   Read the charge voltage in V.

.. py:function:: read_chargin_current()

   Read the charge current in A.

.. py:function:: read_system_voltage()

   Read the system-side voltate in V.

.. py:function:: read_thermistor_voltage()

   Read the thermistor voltage in V.

   There is a resistor network from GND over a thermistor
   (10K at room temperature) over 10K to the Thermistor Bias voltage.
   This reads the voltage between thermistor and resistor.
