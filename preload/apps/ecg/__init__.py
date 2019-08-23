import os
import display
import utime
import buttons
import max30001
import math
import struct

WIDTH = 160
HEIGHT = 80
OFFSET = 50
ECG_RATE = 128
HISTORY_MAX = ECG_RATE * 2
DRAW_AFTER_SAMPLES = 5
SCALE_FACTOR = 30
MODE_USB = "USB"
MODE_FINGER = "Finger"
FILEBUFFERBLOCK = 4096
COLOR_BACKGROUND = [0, 0, 0]
COLOR_LINE = [255, 255, 255]
COLOR_TEXT = [255, 255, 255]
COLOR_MODE_FINGER = [0, 255, 0]
COLOR_MODE_USB = [0, 0, 255]
COLOR_WRITE_FG = [255, 255, 255]
COLOR_WRITE_BG = [255, 0, 0]

current_mode = MODE_FINGER
history = []
filebuffer = bytearray()
write = 0
bias = True
update_screen = 0
pause_screen = 0
sensor = 0
disp = display.open()


def callback_ecg(datasets):
    global update_screen, history, filebuffer, write
    if len(datasets) > 0:
        for value in datasets:
            history.append(value)
        update_screen += len(datasets)
    if len(history) > HISTORY_MAX:
        r = len(history) - HISTORY_MAX
        for i in range(r):
            history.pop(0)

    # buffer for writes
    if write > 0:
        if len(datasets) > 0:
            for value in datasets:
                filebuffer.extend(struct.pack("h", value))
                if len(filebuffer) >= FILEBUFFERBLOCK:
                    write_filebuffer()

    # don't update on every callback
    if update_screen >= DRAW_AFTER_SAMPLES:
        # print("history: %i, filebuffer: %i" % (len(history), len(filebuffer)))
        draw_histogram()
        update_screen = 0


def write_filebuffer():
    global write, filebuffer
    # write to file
    chars = ""
    lt = utime.localtime(write)
    filename = "/ecg-%04d-%02d-%02d_%02d%02d%02d.log" % (
        lt[0],
        lt[1],
        lt[2],
        lt[3],
        lt[4],
        lt[5],
    )

    # write stuff to disk
    # print("Write %i bytes to %s" % (len(filebuffer), filename))
    try:
        f = open(filename, "ab")
        f.write(filebuffer)
        f.close()
    except OSError as e:
        print("Please check the file or filesystem", e)
        write = 0
        pause_screen = -1
        disp.clear(COLOR_BACKGROUND)
        disp.print("IO Error", posy=0, fg=COLOR_TEXT)
        disp.print("Please check", posy=20, fg=COLOR_TEXT)
        disp.print("your", posy=40, fg=COLOR_TEXT)
        disp.print("filesystem", posy=60, fg=COLOR_TEXT)
        disp.update()
        close_sensor()
    except:
        print("Unexpected error, stop writeing logfile")
        write = 0

    filebuffer = bytearray()
    return


def open_sensor():
    global sensor
    sensor = max30001.MAX30001(
        usb=(False if current_mode == MODE_FINGER else True),
        bias=bias,
        sample_rate=ECG_RATE,
        callback=callback_ecg,
    )


def close_sensor():
    global sensor
    sensor.close()


def toggle_mode():
    global current_mode
    close_sensor()
    current_mode = MODE_USB if current_mode == MODE_FINGER else MODE_FINGER
    open_sensor()


def toggle_bias():
    global bias
    close_sensor()
    bias = False if bias == True else True
    open_sensor()
    return


def toggle_write():
    global write, disp, pause_screen
    pause_screen = utime.time_ms() + 1000
    disp.clear(COLOR_BACKGROUND)
    if write > 0:
        write_filebuffer()
        write = 0
        disp.print("Stop", posx=50, posy=20, fg=COLOR_TEXT)
        disp.print("logging", posx=30, posy=40, fg=COLOR_TEXT)
    else:
        filebuffer = bytearray()
        write = utime.time()
        disp.print("Start", posx=45, posy=20, fg=COLOR_TEXT)
        disp.print("logging", posx=30, posy=40, fg=COLOR_TEXT)

    disp.update()
    return


def draw_histogram():
    global disp, history, current_mode, bias, write, pause_screen

    # skip rendering due to message beeing shown
    if pause_screen == -1:
        return
    elif pause_screen > 0:
        t = utime.time_ms()
        if t > pause_screen:
            pause_screen = 0
        else:
            return

    disp.clear(COLOR_BACKGROUND)

    # get max value and calc scale
    value_max = 0
    for value in history:
        if abs(value) > value_max:
            value_max = abs(value)
    scale = SCALE_FACTOR / (value_max if value_max > 0 else 1)

    # draw histogram
    old = False
    x = 0
    samples = len(history)
    for i, value in enumerate(history):
        if old == False:
            old = (x, int(value * scale) + OFFSET)
            x += 1
            continue
        elif i > samples - WIDTH:
            disp.line(old[0], old[1], x, int(value * scale) + OFFSET, col=COLOR_LINE)
            old = (x, int(value * scale) + OFFSET)
            x += 1

    # draw text: mode/bias/write
    disp.print(
        current_mode + ("+Bias" if bias else ""),
        posx=0,
        posy=0,
        fg=(COLOR_MODE_FINGER if current_mode == MODE_FINGER else COLOR_MODE_USB),
    )

    # announce writing ecg log
    if write > 0:
        t = utime.time()
        if write > 0 and t % 2 == 0:
            disp.print("LOG", posx=0, posy=60, fg=COLOR_WRITE_FG, bg=COLOR_WRITE_BG)

    disp.update()


def main():
    # show button layout
    disp.clear(COLOR_BACKGROUND)
    disp.print("  BUTTONS  ", posx=0, posy=0, fg=COLOR_TEXT)
    disp.print("Finger/USB>", posx=0, posy=20, fg=COLOR_MODE_FINGER)
    disp.print("     Bias >", posx=0, posy=40, fg=COLOR_MODE_USB)
    disp.print("< Write Log", posx=0, posy=60, fg=COLOR_WRITE_BG)
    disp.update()
    utime.sleep(3)

    # start ecg
    open_sensor()
    while True:
        button_pressed = False
        while True:
            v = buttons.read(
                buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT | buttons.TOP_RIGHT
            )
            if v == 0:
                button_pressed = False

            if not button_pressed and v & buttons.BOTTOM_LEFT != 0:
                button_pressed = True
                toggle_write()

            elif not button_pressed and v & buttons.BOTTOM_RIGHT != 0:
                button_pressed = True
                toggle_bias()

            elif not button_pressed and v & buttons.TOP_RIGHT != 0:
                button_pressed = True
                toggle_mode()

        pass


if __name__ == "__main__":
    main()
