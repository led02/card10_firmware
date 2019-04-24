import sys


def crc16_update(crc, a):
    crc ^= a
    for i in xrange(8):
        if crc & 1:
            crc = (crc >> 1) ^ 0x8408
        else:
            crc = (crc >> 1)
    return crc

crc = 0

data = open(sys.argv[1]).read()

for b in data:
    crc = crc16_update(crc, ord(b))


padded = data + chr(crc & 0xFF) + chr(crc >> 8)

crc = 0
for b in padded:
    crc = crc16_update(crc, ord(b))

open(sys.argv[1], 'w').write(padded)
