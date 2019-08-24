import os
import display
import utime
import buttons
import max30001
import math
import struct

WIDTH = 160
HEIGHT = 80
OFFSET_Y = 49
ECG_RATE = 128
HISTORY_MAX = ECG_RATE * 4
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
pause_histogram = False
histogram_offset = 0
sensor = 0
disp = display.open()


def callback_ecg(datasets):
    global update_screen, history, filebuffer, write
    update_screen += len(datasets)

    # update histogram datalist
    if pause_histogram == False:
        if len(datasets) > 0:
            for value in datasets:
                history.append(value)
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
        draw_histogram()


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


def toggle_pause():
    global pause_histogram, histogram_offset, history
    if pause_histogram == True:
        pause_histogram = False
        history = []
    else:
        pause_histogram = True
    histogram_offset = 0


def draw_histogram():
    global disp, history, current_mode, bias, write, pause_screen, update_screen

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

    # offset in pause_histogram mode
    samples = len(history)
    s_start = samples - (histogram_offset + (ECG_RATE * 2))
    s_end = samples - (histogram_offset + 1)
    s_draw = s_end - (WIDTH - 1)

    # get max value and calc scale
    value_max = 0
    for i, value in enumerate(history):
        if i >= s_start and i <= s_end and abs(value) > value_max:
            value_max = abs(value)
    scale = SCALE_FACTOR / (value_max if value_max > 0 else 1)

    # draw histogram
    old = False
    x = 0
    for i, value in enumerate(history):
        if old == False:
            old = value
            x += 1
            continue
        elif i > s_start and i > s_draw and i < s_end:

            oldy = int(old * scale)
            if oldy < -SCALE_FACTOR:
                oldy = -SCALE_FACTOR
            elif oldy > SCALE_FACTOR:
                oldy = SCALE_FACTOR

            disp.line(
                x - 1, oldy + OFFSET_Y, x, int(value * scale) + OFFSET_Y, col=COLOR_LINE
            )
            old = value
            x += 1

    # draw text: mode/bias/write
    if pause_histogram == True:
        disp.print(
            "Pause"
            + (
                " -%0.1fs" % (histogram_offset / ECG_RATE)
                if histogram_offset > 0
                else ""
            ),
            posx=0,
            posy=0,
            fg=COLOR_TEXT,
        )
    else:
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
    update_screen = 0


def main():
    global pause_histogram, histogram_offset

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
        button_pressed = {"BOTTOM_LEFT": 0, "BOTTOM_RIGHT": 0, "TOP_RIGHT": 0}
        while True:
            v = buttons.read(
                buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT | buttons.TOP_RIGHT
            )

            # BUTTOM LEFT

            if button_pressed["BOTTOM_LEFT"] == 0 and v & buttons.BOTTOM_LEFT != 0:
                button_pressed["BOTTOM_LEFT"] = utime.time_ms()
                if pause_histogram == False:
                    toggle_write()
                else:
                    l = len(history)
                    histogram_offset += ECG_RATE / 2
                    if l - histogram_offset < WIDTH:
                        histogram_offset = l - WIDTH

            if button_pressed["BOTTOM_LEFT"] > 0 and v & buttons.BOTTOM_LEFT == 0:
                duration = utime.time_ms() - button_pressed["BOTTOM_LEFT"]
                button_pressed["BOTTOM_LEFT"] = 0

            # BUTTOM RIGHT

            if button_pressed["BOTTOM_RIGHT"] == 0 and v & buttons.BOTTOM_RIGHT != 0:
                button_pressed["BOTTOM_RIGHT"] = utime.time_ms()
                if pause_histogram == False:
                    toggle_bias()
                else:
                    histogram_offset -= ECG_RATE / 2
                    histogram_offset -= histogram_offset % (ECG_RATE / 2)
                    if histogram_offset < 0:
                        histogram_offset = 0

            if button_pressed["BOTTOM_RIGHT"] > 0 and v & buttons.BOTTOM_RIGHT == 0:
                duration = utime.time_ms() - button_pressed["BOTTOM_RIGHT"]
                button_pressed["BOTTOM_RIGHT"] = 0

            # TOP RIGHT

            # down, and still pressed
            if button_pressed["TOP_RIGHT"] > 0 and v & buttons.TOP_RIGHT != 0:
                duration = utime.time_ms() - button_pressed["TOP_RIGHT"]
                if duration > 1000:
                    button_pressed["TOP_RIGHT"] = -1
                    toggle_pause()

            # register down event
            elif button_pressed["TOP_RIGHT"] == 0 and v & buttons.TOP_RIGHT != 0:
                button_pressed["TOP_RIGHT"] = utime.time_ms()

            # register up event but event already called
            if button_pressed["TOP_RIGHT"] == -1 and v & buttons.TOP_RIGHT == 0:
                button_pressed["TOP_RIGHT"] = 0

            # register normal up event
            elif button_pressed["TOP_RIGHT"] > 0 and v & buttons.TOP_RIGHT == 0:
                duration = utime.time_ms() - button_pressed["TOP_RIGHT"]
                button_pressed["TOP_RIGHT"] = 0
                if pause_histogram == True:
                    toggle_pause()
                else:
                    toggle_mode()

        pass


if __name__ == "__main__":
    main()
