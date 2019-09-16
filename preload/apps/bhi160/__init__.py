import bhi160
import buttons
import color
import contextlib
import display
import itertools
import simple_menu

STATUS_COLORS = [
    color.RED,
    # Orange
    color.RED * 0.5 + color.YELLOW * 0.5,
    color.YELLOW,
    color.GREEN,
]

with contextlib.ExitStack() as cx:
    disp = cx.enter_context(display.open())

    args = {"sample_rate": 10, "sample_buffer_len": 20}

    sensor_iter = itertools.cycle(
        [
            (cx.enter_context(bhi160.BHI160Orientation(**args)), "Orientation"),
            (cx.enter_context(bhi160.BHI160Accelerometer(**args)), "Accel"),
            (cx.enter_context(bhi160.BHI160Gyroscope(**args)), "Gyro"),
            (cx.enter_context(bhi160.BHI160Magnetometer(**args)), "Magnetic"),
        ]
    )
    sensor, sensor_name = next(sensor_iter)

    for ev in simple_menu.button_events(timeout=0.1):
        # Pressing the bottom right button cycles through sensors
        if ev == buttons.BOTTOM_RIGHT:
            sensor, sensor_name = next(sensor_iter)

        samples = sensor.read()
        if not samples:
            continue

        sample = samples[-1]
        col = STATUS_COLORS[sample.status]

        disp.clear()
        disp.print("{:^11s}".format(sensor_name), posy=0, posx=3)
        disp.print("X:{: 9.4f}".format(sample.x), posy=20, fg=col)
        disp.print("Y:{: 9.4f}".format(sample.y), posy=40, fg=col)
        disp.print("Z:{: 9.4f}".format(sample.z), posy=60, fg=col)
        disp.update()
