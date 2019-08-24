import buttons
import color
import display


def button_events():
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
    """
    yield 0
    v = buttons.read(buttons.BOTTOM_LEFT | buttons.BOTTOM_RIGHT | buttons.TOP_RIGHT)
    button_pressed = True if v != 0 else False
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

    def __init__(self, entries):
        if len(entries) == 0:
            raise ValueError("at least one entry is required")

        self.entries = entries
        self.idx = 0
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
        self.disp.print(
            " " + self.entry2name(value) + " " * 9,
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

    def run(self):
        """Start the event-loop."""
        for ev in button_events():
            if ev == buttons.BOTTOM_RIGHT:
                self.draw_menu(-8)
                self.idx = (self.idx + 1) % len(self.entries)
                self.on_scroll(self.entries[self.idx], self.idx)
            elif ev == buttons.BOTTOM_LEFT:
                self.draw_menu(8)
                self.idx = (self.idx + len(self.entries) - 1) % len(self.entries)
                self.on_scroll(self.entries[self.idx], self.idx)
            elif ev == buttons.TOP_RIGHT:
                self.on_select(self.entries[self.idx], self.idx)

            self.draw_menu()
