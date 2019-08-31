import sys_bme680
import ucollections

# Import old module for compatibility
from sys_bme680 import *  # noqa

Bme680Data = ucollections.namedtuple(
    "Bme680Data", ["temperature", "humidity", "pressure", "gas_resistance"]
)


class Bme680:
    def __init__(self):
        sys_bme680.init()

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.close()

    def get_data(self):
        return Bme680Data(*sys_bme680.get_data())

    def close(self):
        sys_bme680.deinit()

    def temperature(self):
        return self.get_data().temperature

    def humidity(self):
        return self.get_data().humidity

    def pressure(self):
        return self.get_data().pressure

    def gas_resistance(self):
        return self.get_data().gas_resistance
