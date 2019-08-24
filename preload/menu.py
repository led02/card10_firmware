"""
Menu Script
===========
You can customize this script however you want :)  If you want to go back to
the default version, just delete this file; the firmware will recreate it on
next run.
"""
import buttons
import color
import display
import os
import utime
import ujson
import sys

BUTTON_TIMER_POPPED = -1
COLOR1, COLOR2 = (color.CHAOSBLUE_DARK, color.CHAOSBLUE)
MAXCHARS = 11


def create_folders():
    try:
        os.mkdir("/apps")
    except:
        pass


def read_metadata(app_folder):
    try:
        info_file = "/apps/%s/metadata.json" % (app_folder)
        with open(info_file) as f:
            information = f.read()
        return ujson.loads(information)
    except Exception as e:
        print("Failed to read metadata for %s" % (app_folder))
        sys.print_exception(e)
        return {
            "author": "",
            "name": app_folder,
            "descriptionr": "",
            "category": "",
            "revision": 0,
        }


def list_apps():
    """Create a list of available apps."""
    apps = []

    # add main application
    for mainFile in os.listdir("/"):
        if mainFile == "main.py":
            apps.append(
                [
                    "/main.py",
                    {
                        "author": "card10badge Team",
                        "name": "Home",
                        "description": "",
                        "category": "",
                        "revision": 0,
                    },
                ]
            )

    dirlist = [
        entry for entry in sorted(os.listdir("/apps")) if not entry.startswith(".")
    ]

    # list all hatchary style apps (not .elf and not .py)
    # with or without metadata.json
    for appFolder in dirlist:
        if not (appFolder.endswith(".py") or appFolder.endswith(".elf")):
            apps.append(["/apps/%s/__init__.py" % appFolder, read_metadata(appFolder)])

    # list simple python scripts
    for pyFile in dirlist:
        if pyFile.endswith(".py"):
            apps.append(
                [
                    "/apps/%s" % pyFile,
                    {
                        "author": "",
                        "name": pyFile,
                        "description": "",
                        "category": "",
                        "revision": 0,
                    },
                ]
            )

    # list simple elf binaries
    for elfFile in dirlist:
        if elfFile.endswith(".elf"):
            apps.append(
                [
                    "/apps/%s" % elfFile,
                    {
                        "author": "",
                        "name": elfFile,
                        "description": "",
                        "category": "",
                        "revision": 0,
                    },
                ]
            )

    return apps


def button_events(timeout=0):
    """Iterate over button presses (event-loop)."""
    yield 0
    button_pressed = False
    count = 0
    while True:
        v = buttons.read(buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT | buttons.TOP_RIGHT)
        if timeout > 0 and count > 0 and count % timeout == 0:
            yield BUTTON_TIMER_POPPED

        if timeout > 0:
            count += 1

        if v == 0:
            button_pressed = False

        if not button_pressed and v & buttons.BOTTOM_LEFT != 0:
            button_pressed = True
            yield buttons.BOTTOM_LEFT

        if not button_pressed and v & buttons.BOTTOM_RIGHT != 0:
            button_pressed = True
            yield buttons.BOTTOM_RIGHT

        if not button_pressed and v & buttons.TOP_RIGHT != 0:
            button_pressed = True
            yield buttons.TOP_RIGHT

        utime.sleep_ms(10)


def triangle(disp, x, y, left, scale=6, color=[255, 0, 0]):
    """Draw a triangle to show there's more text in this line"""
    yf = 1 if left else -1
    disp.line(x - scale * yf, int(y + scale / 2), x, y, col=color)
    disp.line(x, y, x, y + scale, col=color)
    disp.line(x, y + scale, x - scale * yf, y + int(scale / 2), col=color)


def draw_menu(disp, applist, pos, appcount, lineoffset):
    disp.clear()

    start = 0
    if pos > 0:
        start = pos - 1
    if start + 4 > appcount:
        start = appcount - 4
    if start < 0:
        start = 0

    for i, app in enumerate(applist):
        if i >= start + 4 or i >= appcount:
            break
        if i >= start:
            disp.rect(
                0,
                (i - start) * 20,
                159,
                (i - start) * 20 + 20,
                col=COLOR1 if i == pos else COLOR2,
            )

            line = app[1]["name"]
            linelength = len(line)
            off = 0

            # calc line offset for scrolling
            if i == pos and linelength > (MAXCHARS - 1) and lineoffset > 0:
                off = (
                    lineoffset
                    if lineoffset + (MAXCHARS - 1) < linelength
                    else linelength - (MAXCHARS - 1)
                )
            if lineoffset > linelength:
                off = 0

            disp.print(
                " " + line[off : (off + (MAXCHARS - 1))],
                posy=(i - start) * 20,
                bg=COLOR1 if i == pos else COLOR2,
            )
            if i == pos:
                disp.print(">", posy=(i - start) * 20, fg=color.COMMYELLOW, bg=COLOR1)

            if linelength > (MAXCHARS - 1) and off < linelength - (MAXCHARS - 1):
                triangle(disp, 153, (i - start) * 20 + 6, False, 6)
                triangle(disp, 154, (i - start) * 20 + 7, False, 4)
                triangle(disp, 155, (i - start) * 20 + 8, False, 2)
            if off > 0:
                triangle(disp, 24, (i - start) * 20 + 6, True, 6)
                triangle(disp, 23, (i - start) * 20 + 7, True, 4)
                triangle(disp, 22, (i - start) * 20 + 8, True, 2)

    disp.update()


def main():
    create_folders()
    disp = display.open()
    applist = list_apps()
    numapps = len(applist)
    current = 0
    lineoffset = 0
    timerscrollspeed = 1
    timerstartscroll = 5
    timercountpopped = 0
    for ev in button_events(10):
        if numapps == 0:
            disp.clear(color.COMMYELLOW)
            disp.print(
                " No apps ",
                posx=17,
                posy=20,
                fg=color.COMMYELLOW_DARK,
                bg=color.COMMYELLOW,
            )
            disp.print(
                "available",
                posx=17,
                posy=40,
                fg=color.COMMYELLOW_DARK,
                bg=color.COMMYELLOW,
            )
            disp.update()
            continue

        if ev == buttons.BOTTOM_RIGHT:
            # Scroll down
            current = (current + 1) % numapps
            lineoffset = 0
            timercountpopped = 0

        elif ev == buttons.BOTTOM_LEFT:
            # Scroll up
            current = (current + numapps - 1) % numapps
            lineoffset = 0
            timercountpopped = 0

        elif ev == BUTTON_TIMER_POPPED:
            timercountpopped += 1
            if (
                timercountpopped >= timerstartscroll
                and (timercountpopped - timerstartscroll) % timerscrollspeed == 0
            ):
                lineoffset += 1

        elif ev == buttons.TOP_RIGHT:
            # Select & start
            disp.clear().update()
            disp.close()
            try:
                os.exec(applist[current][0])
            except OSError as e:
                print("Loading failed: ", e)
                os.exit(1)

        draw_menu(disp, applist, current, numapps, lineoffset)


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        sys.print_exception(e)
        with display.open() as d:
            d.clear(color.COMMYELLOW)
            d.print("Menu", posx=52, posy=20, fg=color.COMMYELLOW_DARK, bg=color.COMMYELLOW)
            d.print("crashed", posx=31, posy=40, fg=color.COMMYELLOW_DARK, bg=color.COMMYELLOW)
            d.update()
            utime.sleep(2)
        os.exit(1)
