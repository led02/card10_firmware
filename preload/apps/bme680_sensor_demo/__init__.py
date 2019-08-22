"""
BME680 Demo
===========
This app shows the current readings of the BME680 sensor
"""
import buttons
import color
import display
import os
import utime
import bme680


def main():
    bme680.init()

    disp = display.open()
    disp.clear().update()

    while True:
        sensor_data = bme680.get_data()
        disp.clear()
        disp.print("BME680 Sensor")
        disp.print("{} C".format(str(sensor_data[0])), posy=20)
        disp.print("{} rh".format(str(sensor_data[1])), posy=40)
        disp.print("{} hPa".format(str(sensor_data[2])), posy=60)
        disp.update()
        utime.sleep(10)


if __name__ == "__main__":
    main()
