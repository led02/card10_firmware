import buttons
import color
import display
import sys
import utime

TIMEOUT = 0x100
""":py:func:`~simple_menu.button_events` timeout marker."""


def button_events(timeout=None):
    """
    Iterate over button presses (event-loop).

    This is just a helper function used internally by the menu.  But you can of
    course use it for your own scripts as well.  It works like this:

    .. code-block:: python

        import simple_menu, buttons

        for ev in simple_menu.button_events():
            if ev == buttons.BOTTOM_LEFT:
                # Left
                pass
            elif ev == buttons.BOTTOM_RIGHT:
                # Right
                pass
            elif ev == buttons.TOP_RIGHT:
                # Select
                pass

    .. versionadded:: 1.4

    :param float,optional timeout:
       Timeout after which the generator should yield in any case.  If a
       timeout is defined, the generator will periodically yield
       :py:data:`simple_menu.TIMEOUT`.

       .. versionadded:: 1.9
    """
    yield 0

    v = buttons.read(buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT | buttons.TOP_RIGHT)
    button_pressed = True if v != 0 else False

    if timeout is not None:
        timeout = int(timeout * 1000)
        next_tick = utime.time_ms() + timeout

    while True:
        if timeout is not None:
            current_time = utime.time_ms()
            if current_time >= next_tick:
                next_tick += timeout
                yield TIMEOUT

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


class _ExitMenuException(Exception):
    pass


class Menu:
    """
    A simple menu for card10.

    This menu class is supposed to be inherited from to create a menu as shown
    in the example above.

    To instanciate the menu, pass a list of entries to the constructor:

    .. code-block:: python

        m = Menu(os.listdir("."))
        m.run()

    Then, call :py:meth:`~simple_menu.Menu.run` to start the event loop.

    .. versionadded:: 1.4
    """

    color_1 = color.CHAOSBLUE
    """Background color A."""
    color_2 = color.CHAOSBLUE_DARK
    """Background color B."""
    color_text = color.WHITE
    """Text color."""
    color_sel = color.COMMYELLOW
    """Color of the selector."""

    scroll_speed = 0.5
    """
    Time to wait before scrolling to the right.

    .. versionadded:: 1.9
    """

    timeout = None
    """
    Optional timeout for inactivity.  Once this timeout is reached,
    :py:meth:`~simple_menu.Menu.on_timeout` will be called.

    .. versionadded:: 1.9
    """

    def on_scroll(self, item, index):
        """
        Hook when the selector scrolls to a new item.

        This hook is run everytime a scroll-up or scroll-down is performed.
        Overwrite this function in your own menus if you want to do some action
        every time a new item is scrolled onto.

        :param item: The item which the selector now points to.
        :param int index: Index into the ``entries`` list of the ``item``.
        """
        pass

    def on_select(self, item, index):
        """
        Hook when an item as selected.

        The given ``index`` was selected with a SELECT button press.  Overwrite
        this function in your menu to perform an action on select.

        :param item: The item which was selected.
        :param int index: Index into the ``entries`` list of the ``item``.
        """
        pass

    def on_timeout(self):
        """
        The inactivity timeout has been triggered.  See
        :py:attr:`simple_menu.Menu.timeout`.

        .. versionadded:: 1.9
        """
        self.exit()

    def exit(self):
        """
        Exit the event-loop.  This should be called from inside an ``on_*`` hook.

        .. versionadded:: 1.9
        .. versionchanged:: 1.11

            Fixed this function not working properly.
        """
        raise _ExitMenuException()

    def __init__(self, entries):
        if len(entries) == 0:
            raise ValueError("at least one entry is required")

        self.entries = entries
        self.idx = 0
        self.select_time = utime.time_ms()
        self.disp = display.open()

    def entry2name(self, value):
        """
        Convert an entry object to a string representation.

        Overwrite this functio if your menu items are not plain strings.

        **Example**:

        .. code-block:: python

            class MyMenu(simple_menu.Menu):
                def entry2name(self, value):
                    return value[0]

            MyMenu(
                [("a", 123), ("b", 321)]
            ).run()
        """
        return str(value)

    def draw_entry(self, value, index, offset):
        """
        Draw a single entry.

        This is an internal function; you can override it for customized behavior.

        :param value: The value for this entry.  Use this to identify
            different entries.
        :param int index: A unique index per entry. Stable for a certain entry,
            but **not** an index into ``entries``.
        :param int offset: Y-offset for this entry.
        """
        string = self.entry2name(value)

        if offset != 20 or len(string) < 10:
            string = " " + string + " " * 9
        else:
            # Slowly scroll entry to the side
            time_offset = (utime.time_ms() - self.select_time) // int(
                self.scroll_speed * 1000
            )
            time_offset = time_offset % (len(string) - 7) - 1
            time_offset = min(len(string) - 10, max(0, time_offset))
            string = " " + string[time_offset:]

        self.disp.print(
            string,
            posy=offset,
            fg=self.color_text,
            bg=self.color_1 if index % 2 == 0 else self.color_2,
        )

    def draw_menu(self, offset=0):
        """
        Draw the menu.

        You'll probably never need to call this yourself; it is called
        automatially in the event loop (:py:meth:`~simple_menu.Menu.run`).
        """
        self.disp.clear()

        # Wrap around the list and draw entries from idx - 3 to idx + 4
        for y, i in enumerate(
            range(len(self.entries) + self.idx - 3, len(self.entries) + self.idx + 4)
        ):
            self.draw_entry(
                self.entries[i % len(self.entries)], i, offset + y * 20 - 40
            )

        self.disp.line(4, 22, 11, 29, col=self.color_sel, size=2)
        self.disp.line(4, 36, 11, 29, col=self.color_sel, size=2)

        self.disp.update()

    def error(self, line1, line2=""):
        """
        Display an error message.

        :param str line1: First line of the error message.
        :param str line2: Second line of the error message.

        .. versionadded:: 1.9
        """
        self.disp.clear(color.COMMYELLOW)

        offset = max(0, (160 - len(line1) * 14) // 2)
        self.disp.print(
            line1, posx=offset, posy=20, fg=color.COMMYELLOW_DARK, bg=color.COMMYELLOW
        )

        offset = max(0, (160 - len(line2) * 14) // 2)
        self.disp.print(
            line2, posx=offset, posy=40, fg=color.COMMYELLOW_DARK, bg=color.COMMYELLOW
        )

        self.disp.update()

    def run(self):
        """Start the event-loop."""
        try:
            timeout = self.scroll_speed
            if self.timeout is not None and self.timeout < self.scroll_speed:
                timeout = self.timeout

            for ev in button_events(timeout):
                if ev == buttons.BOTTOM_RIGHT:
                    self.select_time = utime.time_ms()
                    self.draw_menu(-8)
                    self.idx = (self.idx + 1) % len(self.entries)
                    try:
                        self.on_scroll(self.entries[self.idx], self.idx)
                    except _ExitMenuException:
                        raise
                    except Exception as e:
                        print("Exception during menu.on_scroll():")
                        sys.print_exception(e)
                elif ev == buttons.BOTTOM_LEFT:
                    self.select_time = utime.time_ms()
                    self.draw_menu(8)
                    self.idx = (self.idx + len(self.entries) - 1) % len(self.entries)
                    try:
                        self.on_scroll(self.entries[self.idx], self.idx)
                    except _ExitMenuException:
                        raise
                    except Exception as e:
                        print("Exception during menu.on_scroll():")
                        sys.print_exception(e)
                elif ev == buttons.TOP_RIGHT:
                    try:
                        self.on_select(self.entries[self.idx], self.idx)
                        self.select_time = utime.time_ms()
                    except _ExitMenuException:
                        raise
                    except Exception as e:
                        print("Menu crashed!")
                        sys.print_exception(e)
                        self.error("Menu", "crashed")
                        utime.sleep(1.0)

                self.draw_menu()

                if self.timeout is not None and (
                    utime.time_ms() - self.select_time
                ) > int(self.timeout * 1000):
                    self.on_timeout()
        except _ExitMenuException:
            pass
