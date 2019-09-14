import os
import display
import leds
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

leds.dim_top(1)
COLORS = [((23 + (15 * i)) % 360, 1.0, 1.0) for i in range(11)]


# variables for high-pass filter
moving_average = 0
alpha = 2
beta = 3


def update_history(datasets):
    global history, moving_average, alpha, beta
    for val in datasets:
        history.append(val - moving_average)
        moving_average = (alpha * moving_average + beta * val) / (alpha + beta)

    # trim old elements
    history = history[-HISTORY_MAX:]


# variables for pulse detection
pulse = -1
samples_since_last_pulse = 0
q_threshold = -1
r_threshold = 1
q_spike = -ECG_RATE


def neighbours(n, lst):
    """
    neighbours(2, "ABCDE") = ("AB", "BC", "CD", "DE")
    neighbours(3, "ABCDE") = ("ABC", "BCD", "CDE")
    """

    for i in range(len(lst) - (n - 1)):
        yield lst[i : i + n]


def detect_pulse(num_new_samples):
    global history, pulse, samples_since_last_pulse, q_threshold, r_threshold, q_spike

    # look at 3 consecutive samples, starting 2 samples before the samples that were just added, e.g.:
    # existing samples: "ABCD"
    # new samples: "EF" => "ABCDEF"
    # consider ["CDE", "DEF"]
    # new samples: "GHI" => "ABCDEFGHI"
    # consider ["EFG", "FGH", "GHI"]
    for [prev, cur, next_] in neighbours(3, history[-(num_new_samples + 2) :]):
        samples_since_last_pulse += 1

        if prev > cur < next_ and cur < q_threshold:
            q_spike = samples_since_last_pulse
            # we expect the next q-spike to be at least 60% as high as this one
            q_threshold = (cur * 3) // 5
        elif (
            prev < cur > next_
            and cur > r_threshold
            and samples_since_last_pulse - q_spike < ECG_RATE // 10
        ):
            # the full QRS complex is < 0.1s long, so the q and r spike in particular cannot be more than ECG_RATE//10 samples apart
            pulse = 60 * ECG_RATE // samples_since_last_pulse
            samples_since_last_pulse = 0
            q_spike = -ECG_RATE
            if pulse < 30 or pulse > 210:
                pulse = -1
            # we expect the next r-spike to be at least 60% as high as this one
            r_threshold = (cur * 3) // 5
        elif samples_since_last_pulse > 2 * ECG_RATE:
            q_threshold = -1
            r_threshold = 1
            pulse = -1


def callback_ecg(datasets):
    global update_screen, history, filebuffer, write
    update_screen += len(datasets)

    # update histogram datalist
    if not pause_histogram:
        update_history(datasets)
        detect_pulse(len(datasets))

    # buffer for writes
    if write > 0:
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
    filename = "/ecg-{:04d}-{:02d}-{:02d}_{:02d}{:02d}{:02d}.log".format(*lt)

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


def open_sensor():
    global sensor
    sensor = max30001.MAX30001(
        usb=(current_mode == MODE_USB),
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
    bias = not bias
    open_sensor()


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


def toggle_pause():
    global pause_histogram, histogram_offset, history
    if pause_histogram:
        pause_histogram = False
        history = []
    else:
        pause_histogram = True
    histogram_offset = 0


def draw_leds(val):
    # val should be in [0, 11]
    for i in range(11):
        leds.prep_hsv(10 - i, COLORS[10 - i] if i < val else (0, 0, 0))
    leds.update()


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
    window_end = int(len(history) - histogram_offset)
    s_start = max(0, window_end - (ECG_RATE * 2))
    s_end = max(0, window_end)
    s_draw = max(0, s_end - WIDTH)

    # get max value and calc scale
    value_max = max(abs(x) for x in history[s_start:s_end])
    scale = SCALE_FACTOR / (value_max if value_max > 0 else 1)

    # draw histogram
    # values need to be inverted so high values are drawn with low pixel coordinates (at the top of the screen)
    draw_points = (int(-x * scale + OFFSET_Y) for x in history[s_draw:s_end])

    prev = next(draw_points)
    for x, value in enumerate(draw_points):
        disp.line(x, prev, x + 1, value, col=COLOR_LINE)
        prev = value

    # draw text: mode/bias/write
    if pause_histogram:
        disp.print(
            "Pause"
            + (
                " -{:0.1f}s".format(histogram_offset / ECG_RATE)
                if histogram_offset > 0
                else ""
            ),
            posx=0,
            posy=0,
            fg=COLOR_TEXT,
        )
    else:
        draw_leds((max(history[-5:]) * scale + SCALE_FACTOR) * 11 / (SCALE_FACTOR * 2))
        if pulse < 0:
            disp.print(
                current_mode + ("+Bias" if bias else ""),
                posx=0,
                posy=0,
                fg=(
                    COLOR_MODE_FINGER if current_mode == MODE_FINGER else COLOR_MODE_USB
                ),
            )
        else:
            disp.print(
                "BPM: {}".format(pulse),
                posx=0,
                posy=0,
                fg=(
                    COLOR_MODE_FINGER if current_mode == MODE_FINGER else COLOR_MODE_USB
                ),
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

            # BOTTOM LEFT

            if button_pressed["BOTTOM_LEFT"] == 0 and v & buttons.BOTTOM_LEFT != 0:
                button_pressed["BOTTOM_LEFT"] = utime.time_ms()
                if not pause_histogram:
                    toggle_write()
                else:
                    l = len(history)
                    histogram_offset += ECG_RATE / 2
                    if l - histogram_offset < WIDTH:
                        histogram_offset = l - WIDTH

            if button_pressed["BOTTOM_LEFT"] > 0 and v & buttons.BOTTOM_LEFT == 0:
                duration = utime.time_ms() - button_pressed["BOTTOM_LEFT"]
                button_pressed["BOTTOM_LEFT"] = 0

            # BOTTOM RIGHT

            if button_pressed["BOTTOM_RIGHT"] == 0 and v & buttons.BOTTOM_RIGHT != 0:
                button_pressed["BOTTOM_RIGHT"] = utime.time_ms()
                if not pause_histogram:
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
                if pause_histogram:
                    toggle_pause()
                else:
                    toggle_mode()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt as e:
        sensor.close()
        raise e
