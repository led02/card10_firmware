#!/usr/bin/env python3
import sys
import crc16


def main():
    data = open(sys.argv[1], 'rb').read()
    crc = crc16.crc16xmodem(data)
    # print(crc)

    padded = data + bytes([crc >> 8, crc & 0xFF])

    crc = crc16.crc16xmodem(padded)
    # print(crc)

    open(sys.argv[1], 'wb').write(padded)


if __name__ == "__main__":
    main()
