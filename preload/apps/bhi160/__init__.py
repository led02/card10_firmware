import bhi160
import display
import utime

disp = display.open()

bhi = bhi160.BHI160Accelerometer()

while True:
    samples = bhi.read()
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

        disp.print("Accelerometer", posy=0)
        disp.print("X: %f" % sample.x, posy=20, fg=color)
        disp.print("Y: %f" % sample.y, posy=40, fg=color)
        disp.print("Z: %f" % sample.z, posy=60, fg=color)

        disp.update()

    utime.sleep(0.1)
