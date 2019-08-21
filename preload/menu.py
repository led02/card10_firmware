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
import ujson
import sys

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
    except BaseException as e:
        sys.print_exception(e)
        return {'author':'', 'name':app_folder, 'description':'', 'category':'', 'revision': 0}

def list_apps():
    """Create a list of available apps."""
    apps = []

    # add main application
    for mainFile in os.listdir("/"):
        if mainFile == "main.py":
            apps.append(["/main.py", {'author':'', 'name':'Main Clock', 'description':'', 'category':'', 'revision': 0}])

    # list all hatchary style apps (not .elf and not .py)
    # with or without metadata.json
    for appFolder in sorted(os.listdir("/apps")):
        if not(appFolder.endswith(".py") or appFolder.endswith(".elf")):
            apps.append(["/apps/%s/__init__.py" % appFolder, read_metadata(appFolder)])

    # list simple python scripts
    for pyFile in sorted(os.listdir("/apps")):
        if pyFile.endswith(".py"):
            apps.append(["/apps/%s" % pyFile, {'author':'', 'name':pyFile, 'description':'', 'category':'', 'revision': 0}])

    # list simple elf binaries
    for elfFile in sorted(os.listdir("/apps")):
        if elfFile.endswith(".elf"):
            apps.append(["/apps/%s" % elfFile, {'author':'', 'name':elfFile, 'description':'', 'category':'', 'revision': 0}])

    return apps

def button_events():
    """Iterate over button presses (event-loop)."""
    yield 0
    button_pressed = False
    while True:
        v = buttons.read(buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT | buttons.TOP_RIGHT)

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


COLOR1, COLOR2 = (color.CHAOSBLUE_DARK, color.CHAOSBLUE)


def draw_menu(disp, applist, idx, offset):
    disp.clear()

    # Wrap around the app-list and draw entries from idx - 3 to idx + 4
    for y, i in enumerate(range(len(applist) + idx - 3, len(applist) + idx + 4)):
        disp.print(
            " " + applist[i % len(applist)][1]['name'] + "      ",
            posy=offset + y * 20 - 40,
            bg=COLOR1 if i % 2 == 0 else COLOR2,
        )

    disp.print(
        ">",
        posy=20,
        fg=color.COMMYELLOW,
        bg=COLOR1 if (idx + len(applist)) % 2 == 0 else COLOR2,
    )
    disp.update()


def main():
    create_folders()
    disp = display.open()
    applist = list_apps()
    numapps = len(applist)
    current = 0
    for ev in button_events():
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
            draw_menu(disp, applist, current, -8)
            current = (current + 1) % numapps
        elif ev == buttons.BOTTOM_LEFT:
            # Scroll up
            draw_menu(disp, applist, current, 8)
            current = (current + numapps - 1) % numapps
        elif ev == buttons.TOP_RIGHT:
            # Select & start
            disp.clear().update()
            disp.close()
            try:
                os.exec(applist[current][0])
            except OSError as e:
                print("Loading failed: ", e)
                os.exit(1)

        draw_menu(disp, applist, current, 0)


if __name__ == "__main__":
    main()
