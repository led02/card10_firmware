import sys
import crc16

crc = 0

data = open(sys.argv[1]).read()
crc = crc16.crc16xmodem(data)
print crc

padded = data + chr(crc >> 8) + chr(crc & 0xFF)

crc = 0
crc = crc16.crc16xmodem(padded)
print crc

open(sys.argv[1], 'w').write(padded)
