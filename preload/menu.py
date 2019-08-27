"""
Menu Script
===========
You can customize this script however you want :)  If you want to go back to
the default version, just delete this file; the firmware will recreate it on
next run.
"""
import collections
import color
import display
import os
import simple_menu
import sys
import ujson
import utime

App = collections.namedtuple("App", ["name", "path"])


def enumerate_apps():
    """List all installed apps."""
    for f in os.listdir("/"):
        if f == "main.py":
            yield App("Home", f)

    for app in sorted(os.listdir("/apps")):
        if app.startswith("."):
            continue

        if app.endswith(".py") or app.endswith(".elf"):
            yield App(app, "/apps/" + app)
            continue

        try:
            with open("/apps/" + app + "/metadata.json") as f:
                info = ujson.load(f)

            yield App(
                info["name"], "/apps/{}/{}".format(app, info.get("bin", "__init__.py"))
            )
        except Exception as e:
            print("Could not load /apps/{}/metadata.json!".format(app))
            sys.print_exception(e)


class MainMenu(simple_menu.Menu):
    timeout = 30.0

    def entry2name(self, app):
        return app.name

    def on_select(self, app, index):
        self.disp.clear().update()
        try:
            print("Trying to load " + app.path)
            os.exec(app.path)
        except OSError as e:
            print("Loading failed: ")
            sys.print_exception(e)
            self.error("Loading", "failed")
            utime.sleep(1.0)
            os.exit(1)

    def on_timeout(self):
        try:
            f = open("main.py")
            f.close()
            os.exec("main.py")
        except OSError:
            pass


def no_apps_message():
    """Display a warning if no apps are installed."""
    with display.open() as disp:
        disp.clear(color.COMMYELLOW)
        disp.print(
            " No apps ", posx=17, posy=20, fg=color.COMMYELLOW_DARK, bg=color.COMMYELLOW
        )
        disp.print(
            "available", posx=17, posy=40, fg=color.COMMYELLOW_DARK, bg=color.COMMYELLOW
        )
        disp.update()

    while True:
        utime.sleep(0.5)


if __name__ == "__main__":
    apps = list(enumerate_apps())

    if apps == []:
        no_apps_message()

    MainMenu(apps).run()
