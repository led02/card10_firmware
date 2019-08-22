import os
import display
import utime
import buttons

CONFIG_NAME = "ble.txt"
MAC_NAME = "mac.txt"
ACTIVE_STRING = "active=true"
INACTIVE_STRING = "active=false"


def init():
    if CONFIG_NAME not in os.listdir("."):
        with open(CONFIG_NAME, "w") as f:
            f.write(INACTIVE_STRING)


def load_mac():
    if MAC_NAME in os.listdir("."):
        with open(MAC_NAME) as f:
            return f.read().strip()


def triangle(disp, x, y, left):
    yf = 1 if left else -1
    scale = 6
    disp.line(x - scale * yf, int(y + scale / 2), x, y)
    disp.line(x, y, x, y + scale)
    disp.line(x, y + scale, x - scale * yf, y + int(scale / 2))


def toggle():
    content = INACTIVE_STRING if is_active() else ACTIVE_STRING
    with open(CONFIG_NAME, "w") as f:
        f.write(content)

    disp.clear()
    disp.print("resetting", posy=0, fg=[0, 255, 255])
    disp.print("to toggle", posy=20, fg=[0, 255, 255])
    disp.print("BLE state", posy=40, fg=[0, 255, 255])
    disp.update()
    os.reset()


def is_active():
    with open(CONFIG_NAME, "r") as f:
        state = f.readlines()[0]
        if len(state) < len(ACTIVE_STRING):
            return False
        state = state[0 : len(ACTIVE_STRING)]
        return state == ACTIVE_STRING


def headline():
    disp.print("BLE", posy=0, fg=[0, 255, 255])
    if is_active():
        disp.print("active", posy=20, fg=[0, 255, 0])
        mac = load_mac()
        if mac is not None:
            disp.print(mac[9:], posy=60, fg=[0, 0, 255])
    else:
        disp.print("inactive", posy=20, fg=[255, 0, 0])


def selector():
    triangle(disp, 148, 46, False)
    disp.print("toggle", posx=25, posy=40, fg=[255, 255, 255])


disp = display.open()
button_pressed = True
init()

while True:
    disp.clear()
    headline()
    v = buttons.read(buttons.TOP_RIGHT)
    if v == 0:
        button_pressed = False

    if not button_pressed and v & buttons.TOP_RIGHT != 0:
        button_pressed = True
        toggle()

    selector()
    disp.update()
    utime.sleep(0.1)
