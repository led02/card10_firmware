import bhi160
import display
import utime

disp = display.open()

while True:
    with bhi160.BHI160Accelerometer(sample_rate=20) as bhi:
        utime.sleep(0.25)
        samples = bhi.read()
        if len(samples) > 0:
            disp.clear()
            sample = samples[0]

            disp.print("Accelerometer", posy=0)
            disp.print("X: %f" % sample["x"], posy=20)
            disp.print("Y: %f" % sample["y"], posy=40)
            disp.print("Z: %f" % sample["z"], posy=60)

            disp.update()

