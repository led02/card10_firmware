import ucollections

_ColorTuple = ucollections.namedtuple("Color", ["red", "green", "blue"])


class Color(_ColorTuple):
    """
    A color in 24-bit RGB.

    You can create a new color from red, green, and blue::

        yellow = Color(255, 255, 0)

    You can get the HTML representation for a color using::

        html_repr = str(Color(128, 128, 128))
        assert html_repr == "#808080"

    .. py:attribute:: red

        Red component of this color.

    .. py:attribute:: green

        Green component of this color.

    .. py:attribute:: blue

        Blue component of this color.
    """

    @classmethod
    def from_hex(cls, color):
        """
        Create a color from a hexadecimal number::

            # Magenta
            Color.from_hex(0xff00ff)
        """
        red = (color & 0xff0000) >> 16
        green = (color & 0x00ff00) >> 8
        blue = (color & 0x0000ff)
        return cls(red, green, blue)

    def __str__(self):
        # Return the color in hex
        return "#{:02x}{:02x}{:02x}".format(
            self.red, self.green, self.blue
        )


Color.BLACK   = Color.from_hex(0x000000)
Color.WHITE   = Color.from_hex(0xffffff)
Color.RED     = Color.from_hex(0xff0000)
Color.GREEN   = Color.from_hex(0x00ff00)
Color.YELLOW  = Color.from_hex(0xffff00)
Color.BLUE    = Color.from_hex(0x0000ff)
Color.MAGENTA = Color.from_hex(0xff00ff)
Color.CYAN    = Color.from_hex(0x00ffff)


# Add the colors and constructors to the module for convenience
# This allows you to do the following:
#
#    import colors
#
#    colors.BLACK
#    colors.from_hex(0xc6c6c6)
globals().update(
    {
        n: getattr(Color, n)
        for n in dir(Color)
        if n.startswith("from") or n.isupper()
    }
)
