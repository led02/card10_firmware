#!/usr/bin/env python3
import sys

try:
    import crc16

    crcfun = crc16.crc16xmodem
except ImportError:
    try:
        import crcmod

        crcfun = crcmod.predefined.mkCrcFun("xmodem")
    except ImportError:
        try:
            import crcelk

            crcfun = crcelk.CRC_XMODEM.calc_bytes
        except ImportError:
            raise Exception(
                "Could not find a CRC implementation. Tried: crc16, crcmod, crcelk."
            )


def main():
    data = open(sys.argv[1], "rb").read()
    crc = crcfun(data)
    # print(crc)

    padded = data + bytes([crc >> 8, crc & 0xFF])

    crc = crcfun(padded)
    # print(crc)

    open(sys.argv[1], "wb").write(padded)


if __name__ == "__main__":
    main()
