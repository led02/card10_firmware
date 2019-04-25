import sys
import crc16

#def crc16_update(crc, a):
"""
    for j in xrange(8):
        if (a ^ crc) & 1:
            crc = crc >> 1
            crc = crc ^ 0x8408
        else:
            crc = crc >> 1;

        a = a >> 1

    return crc
"""

"""
    crc ^= a
    for i in xrange(8):
        if crc & 1:
            crc = (crc >> 1) ^ 0x8408
        else:
            crc = (crc >> 1)
    return crc
"""
crc = 0

data = open(sys.argv[1]).read()

#for b in data:
#    crc = crc16_update(crc, ord(b))

crc = crc16.crc16xmodem(data)

print crc

#padded = data + chr(crc & 0xFF) + chr(crc >> 8)
padded = data + chr(crc >> 8) + chr(crc & 0xFF)

crc = 0
#for b in padded:
#    crc = crc16_update(crc, ord(b))

crc = crc16.crc16xmodem(padded)

print crc

open(sys.argv[1], 'w').write(padded)
