# card10 Firmware Readme

Firmware for the card10 badge for the Chaos Communication Camp 2019.

We keep documentation in the `Documentation/` subdirectory.  You can find a
rendered version over at <https://firmware.card10.badge.events.ccc.de/>.

## How To Help
If you want to help us ensure the card10 will have been exactly as we reconstructed from the audio-logs we recovered, please head over to our [wiki](https://card10.badge.events.ccc.de/en/firmware/), or directly to the [issue tracker](https://git.card10.badge.events.ccc.de/card10/firmware/issues?sort=label_priority).

## How To Build
A full guide for building is available in the docs as [How To Build](https://firmware.card10.badge.events.ccc.de/how-to-build.html).

## How To Flash
Flasing with or without a debugger is described in detail in [How To Flash](https://firmware.card10.badge.events.ccc.de/how-to-flash.html).

## Debugger
If you have a debugger, head over to our [Debugger](https://firmware.card10.badge.events.ccc.de/debugger.html) page to learn how to use it.

---

# Old README Contents:
TODO: Move over into new docs

## Serial Console
Learn more about [card10's Serial Console](https://firmware.card10.badge.events.ccc.de/pycardium/overview.html#serial-console).

## Bootloader
The bootloader is used to flash card10 without an external debugger. It exposes the file system via USB and accepts a `card10.bin` file as firmware image.

To flash the bootloader, go to the `bootloader` directory and follow the steps above.

To execute the bootloader, turn off card10, press the lower right button and turn on. Connect card10 via a USB cable to a computer and mount the file system. You can now copy your card10.bin file to the file system. Eject the drive (IMPORTANT!) via your operating system (`umount` and `sync` is not enough). Turn card10 off and back on again. It will try to boot the image.

IMPORTANT: The prototypes have the USB data lines reversed. Use the provided USB adapter to connect card10.

### Building card10.bin
Run `./build_image`. TODO: Add to meson

The tool uses Python to generate a CRC. Make sure to have `python-crc16` installed.
