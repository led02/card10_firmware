import sys_display
import color

# font enumeration
FONT8 = 0
FONT12 = 1
FONT16 = 2
FONT20 = 3
FONT24 = 4


class Display:
    """
    The display class provides methods to allow the lcd display
    in card10 to be used in a safe way. All draw methods return
    the display object so that it is possible to chain calls.
    It is recommended to use a context manager as following:

    .. code-block:: python

        import display
        with display.open() as disp:
            disp.clear().update()
    """

    def __init__(self):
        """
        Opens the display. Will fail the display can't be locked
        """
        sys_display.open()

    def __enter__(self):
        return self

    def __exit__(self, _et, _ev, _t):
        self.close()

    @classmethod
    def open(cls):
        """
        Opens the display. Will fail the display can't be locked
        """
        return cls()

    @staticmethod
    def close():
        """
        Closes and unlocks the display. To be able to use it again,
        it is necessary to open and lock it again with Display.open()
        """
        sys_display.close()

    def update(self):
        """
        Updates the display based on the changes previously made by
        various draw functions
        """
        sys_display.update()

    def clear(self, col=None):
        """
        Clears the display using the color provided, or the default
        color black

        :param col: Clearing color (expects RGB triple)
        """
        col = col or color.BLACK
        sys_display.clear(col)
        return self

    def print(self, text, *, fg=None, bg=None, posx=0, posy=0, font=FONT20):
        """
        Prints a string on the display.

        :param text: Text to print
        :param fg: Foreground color (expects RGB triple)
        :param bg: Background color (expects RGB triple) or None for transparent background
        :param posx: X-Position of the first character
        :param posy: Y-Position of the first character
        :param font: 0 <= font <= 4 (currently) selects a font

        Avaiable Fonts:

         - :py:data:`display.FONT8`
         - :py:data:`display.FONT12`
         - :py:data:`display.FONT16`
         - :py:data:`display.FONT20`
         - :py:data:`display.FONT24`

         **Example:**

        .. code-block:: python

            with display.open() as d:
                d.clear()
                d.print('Hello Earth!', font=display.FONT24)
                d.update()

        .. versionchanged:: 1.11

            Added transparent background printing.
        """
        fg = fg or color.WHITE
        bg = bg or fg

        sys_display.print_adv(text, posx, posy, fg, bg, font)
        return self

    def pixel(self, x, y, *, col=None):
        """
        Draws a pixel on the display

        :param x: X coordinate
        :param y: Y coordinate
        :param col: color of the pixel (expects RGB tripple)
        """

        col = col or color.WHITE

        sys_display.pixel(x, y, col)
        return self

    def blit(self, x, y, w, h, img, bg=None, alpha=None):
        """
        Blit an image to the display

        :param x: top left X coordinate
        :param y: top left Y coordinate
        :param w: image width
        :param h: image height
        :param img: a byte buffer with pixel data (rgb565)
        :param bg: background color
        :param alpha: alpha mask for `img`
        """

        if alpha is None:
            sys_display.blit(x, y, w, h, img)
        else:
            sys_display.blit(x, y, w, h, img, bg, alpha)

        return self

    def backlight(self, brightness):
        """
        Set display backlight brightness

        :param brightness: backlight brightness 0 <= brightness <= 100
        """

        sys_display.backlight(brightness)
        return self

    def line(self, xs, ys, xe, ye, *, col=None, dotted=False, size=1):
        """
        Draws a line on the display.

        :param xs: X start coordinate
        :param ys: Y start coordinate
        :param xe: X end coordinate
        :param ye: Y end coordinate
        :param col: color of the line (expects RGB triple)
        :param dotted: whether the line should be dotted or not
           (questionable implementation: draws every other pixel white, draws
           white squares at higher pixel sizes)
        :param size: size of the individual pixels, ranges from 1 to 8
        """

        col = col or color.WHITE

        sys_display.line(xs, ys, xe, ye, col, dotted, size)
        return self

    def rect(self, xs, ys, xe, ye, *, col=None, filled=True, size=1):
        """
        Draws a rectangle on the display.

        :param xs: X start coordinate
        :param ys: Y start coordinate
        :param xe: X end coordinate
        :param ye: Y end coordinate
        :param col: color of the outline and fill (expects RGB triple)
        :param filled: whether the rectangle should be filled or not
        :param size: size of the individual pixels, ranges from 1 to 8
        """

        col = col or color.WHITE

        sys_display.rect(xs, ys, xe, ye, col, filled, size)
        return self

    def circ(self, x, y, rad, *, col=None, filled=True, size=1):
        """
        Draws a circle on the display.

        :param x: center x coordinate
        :param y: center y coordinate
        :param rad: radius
        :param col: color of the outline and fill (expects RGB triple)
        :param filled: whether the rectangle should be filled or not
        :param size: size of the individual pixels, ranges from 1 to 8
        """

        col = col or color.WHITE

        sys_display.circ(x, y, rad, col, filled, size)
        return self


open = Display.open
close = Display.close
