import sys_bme680
import ucollections

# Import old module for compatibility
from sys_bme680 import *  # noqa

Bme680Data = ucollections.namedtuple(
    "Bme680Data", ["temperature", "humidity", "pressure", "gas_resistance"]
)


class Bme680:
    """
    BME680 4-in-1 environmental sensor.

    **Example**:

    .. code-block:: python

        import bme680

        environment = bme680.Bme680()
        print("Current temperature: {:4.1f} °C".format(environment.temperature()))

    This class can also be used as a context-manager which will automatically
    deactivate the sensor on exit:

    .. code-block:: python

        import bme680

        with bme680.Bme680() as environment:
            print("H: {:4.1f}%".format(environment.humidity()))

        # Sensor is off again, saving power

    .. versionadded:: 1.10
    """

    def __init__(self):
        sys_bme680.init()

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.close()

    def get_data(self):
        """
        Get all sensor data at once.

        :py:meth:`~bme680.Bme680.get_data` returns a namedtuple with the
        following fields:

        - ``temperature``: Temperature in *°C*
        - ``humidity``: Relative humidity
        - ``pressure``: Barometric pressure in *hPa*
        - ``gas_resistance``: Gas resistance in *Ω*

        **Example**:

        .. code-block:: python

            import bme680

            with bme680.Bme680() as environment:
                data = environment.get_data()

                print("T: {}".format(data.temperature))
                print("H: {}".format(data.humidity))
        """
        return Bme680Data(*sys_bme680.get_data())

    def close(self):
        """
        Stop/deinit the BME680.

        If you no longer need measurements, you should call this function to
        save power.
        """
        sys_bme680.deinit()

    def temperature(self):
        """
        Measure current temperature in *°C*.

        **Example**:

        .. code-block:: python

            environment = bme680.Bme680()
            print(str(environment.temperature()))
        """
        return self.get_data().temperature

    def humidity(self):
        """
        Measure current relative humidity.

        **Example**:

        .. code-block:: python

            environment = bme680.Bme680()
            print(str(environment.humidity()))
        """
        return self.get_data().humidity

    def pressure(self):
        """
        Measure current barometric pressure in *hPa*.

        **Example**:

        .. code-block:: python

            environment = bme680.Bme680()
            print(str(environment.pressure()))
        """
        return self.get_data().pressure

    def gas_resistance(self):
        """
        Measure current gas resistance in *Ω*.

        **Example**:

        .. code-block:: python

            environment = bme680.Bme680()
            print(str(environment.gas_resistance()))
        """
        return self.get_data().gas_resistance
