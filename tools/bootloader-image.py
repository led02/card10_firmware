#!/usr/bin/env python3
import argparse
import os
from PIL import Image


def main() -> None:
    parser = argparse.ArgumentParser(
        description="""\
\"Compress\" an image for the boot-splash.
The boot-splash is a two-color image."""
    )

    parser.add_argument("image", help="Boot-splash image")
    parser.add_argument("-n", "--name", help="Name of the data-block")
    parser.add_argument("output", help="Output file name")

    args = parser.parse_args()

    im = Image.open(args.image)

    assert im.size[0] == 160, "Image must be 160 pixels wide"
    assert im.size[1] == 80, "Image must be 80 pixels high)"

    if args.name is not None:
        name = args.name
    else:
        name = os.path.splitext(os.path.basename(args.image))[0].replace("-", "_")

    with open(args.output, "w") as f:
        tmp = """\
#include <stdint.h>

/*
 * This is the splash-screen image, compressed using a very primitive algorithm:
 *
 * Each byte encodes up to 127 pixels in either white or black.  The most
 * significant bit determines the color, the remaining 7 bits determine the
 * amount.
 */
const uint8_t {name}[] = {{
"""
        f.write(tmp.format(name=name))

        total = 0
        start = 0
        previous_white = False
        for i in range(160 * 80):
            x = i % 160
            y = i // 160
            white = im.getpixel((x, y))[0] > 0

            if white != previous_white or i - start == 127:
                length = i - start
                assert length < 128, "Internal error"
                value = (length & 0x7F) | (0x80 if previous_white else 0x00)

                tmp = """\
    /* {length} pixels in {color} */
    0x{value:x},
"""
                f.write(
                    tmp.format(
                        length=length,
                        color="white" if previous_white else "black",
                        value=value,
                    )
                )

                previous_white = white
                start = i
                total += length

        tmp = """\
}};
"""
        f.write(tmp.format())

        assert total < (160 * 80), "Internal error"


if __name__ == "__main__":
    main()
