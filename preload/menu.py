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


def list_apps():
    """Create a list of available apps."""
    apps = sorted(os.listdir("."))

    # Filter for apps
    apps = [app for app in apps if app.endswith(".elf") or app.endswith(".py")]

    if "menu.py" in apps:
        apps.remove("menu.py")

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
            " " + applist[i % len(applist)] + "      ",
            posy=offset + y * 20 - 40,
            bg=COLOR1 if i % 2 == 0 else COLOR2,
        )

    disp.print(">", posy=20, fg=color.COMMYELLOW, bg=COLOR2 if idx % 2 == 0 else COLOR1)
    disp.update()


def main():
    disp = display.open()
    applist = list_apps()
    numapps = len(applist)
    current = 0
    for ev in button_events():
        if numapps == 0:
            disp.clear()
            disp.print("No apps", posy=0)
            disp.print("available", posy=20)
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
                os.exec(applist[current])
            except OSError as e:
                print("Loading failed: ", e)
                os.exit(1)

        draw_menu(disp, applist, current, 0)


if __name__ == "__main__":
    main()
