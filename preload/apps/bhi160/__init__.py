import bhi160
import display
import utime
import buttons

disp = display.open()
sensor = 0

sensors = [
    {"sensor": bhi160.BHI160Orientation(), "name": "Orientation"},
    {"sensor": bhi160.BHI160Accelerometer(), "name": "Accelerometer"},
    {"sensor": bhi160.BHI160Gyroscope(), "name": "Gyroscope"},
]

while True:
    # Read and print sample
    samples = sensors[sensor]["sensor"].read()
    if len(samples) > 0:
        disp.clear()
        sample = samples[0]

        color = [255, 0, 0]
        if sample.status == 1:
            color = [255, 128, 0]
        elif sample.status == 2:
            color = [255, 255, 0]
        elif sample.status == 3:
            color = [0, 200, 0]

        disp.print(sensors[sensor]["name"], posy=0)
        disp.print("X: %f" % sample.x, posy=20, fg=color)
        disp.print("Y: %f" % sample.y, posy=40, fg=color)
        disp.print("Z: %f" % sample.z, posy=60, fg=color)

        disp.update()

    # Read button
    v = buttons.read(buttons.BOTTOM_RIGHT)
    if v == 0:
        button_pressed = False

    if not button_pressed and v & buttons.BOTTOM_RIGHT != 0:
        button_pressed = True
        sensor = (sensor + 1) % len(sensors)

    utime.sleep(0.1)
