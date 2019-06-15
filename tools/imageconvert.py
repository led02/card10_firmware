#!/usr/bin/env python

from PIL import Image
import sys

if len(sys.argv) < 2:
    print("Usage: %s <image file>" % sys.argv[0])
    sys.exit(1)

im = Image.open(sys.argv[1])

out_name = '.'.join(sys.argv[1].split('.')[:-1])

print out_name
out = open(out_name + ".h", 'w')

out.write("const unsigned char %s[] = {\n" % out_name.split('/')[-1])

for x in range(im.size[1]):
    for y in range(im.size[0]):
        px = im.getpixel((y, x))

        px16 = ((px[0] >> 3) << 11) | ((px[1] >> 2) << 5) | (px[2] >> 3)

        px16h = (px16 & 0xFF00) >> 8
        px16l = px16 & 0xFF

        out.write("0x%02x, 0x%02x,\n" % (px16l, px16h))

out.write("};\n")
