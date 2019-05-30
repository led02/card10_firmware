# card10 Firmware Readme

## Toolchain

The card10 is based on an ARM Cortex-M4F CPU. To compile the firmware, you need a working cross compiler. To flash the firmware, you either need to use the bootloader or an SWD adapter and OpenOCD.


### GCC

Please install `arm-none-eabi-gcc` on your system. Most distributions have it packaged (might be named `gcc-arm-none-eabi`). You can also download a precompiled toolchain from [ARM](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).

### GDB

If you want to debug code or replace the bootloader, you need OpenOCD and GDB.

Use `arm-none-eabi-gdb` (might be packaged as `gdb-arm-none-eabi` or get it from the binary distribution above).

Add the following line to your `~/.gdbinit` file:
```
add-auto-load-safe-path <PATH TO THIS REPOSITORY>
```
This allows gdb to use our .gdbinit files without specifying them on the command line.


### OpenOCD
If you want to debug code or replace the bootloader, you need OpenOCD and GDB.

Get OpenOCD from [Maxim](https://github.com/maximmbed/openocd) and checkout the `max32xxx` branch. Build using `./configure` and 'make'. Make sure that the `CMSIS-DAP Compliant Debugger` is set to `yes(auto)` after running `./configure` (you might need to install libusb and other USB related libaries).

Please run `make install` after removing any already installed OpenOCD version. Otherwise please always specify the full path to openocd (the binary is under `src/openocd`, when calling `openocd`.

Run OpenOCD from the root `openocd/scripts` directory in this repository like this: `openocd -f interface/cmsis-dap.cfg -f target/max32665.cfg`. It should display something similar to:

```
Info : CMSIS-DAP: SWD  Supported
Info : CMSIS-DAP: FW Version = 1.0
Info : CMSIS-DAP: Interface Initialised (SWD)
Info : SWCLK/TCK = 0 SWDIO/TMS = 1 TDI = 0 TDO = 0 nTRST = 0 nRESET = 1
Info : CMSIS-DAP: Interface ready
Info : clock speed 2000 kHz
Info : SWD DPIDR 0x2ba01477
Info : max32xxx.cpu: hardware has 6 breakpoints, 4 watchpoints
Info : Listening on port 3333 for gdb connections

```

## Debugger Connection
If you have the debugger provided with the `card10`, connect it as follows:
  - `HDK`: This connection provides debugging (SWD) and UART
  - `DEV`: This connection provides power (battery charger) and the native USB connection (bootloader)
  - `USB-C`: Connect the proved USB-C cable with the side which has the blue dot, so the blue dots have the same side

Now connect the USB-C cable, so the name of the cable manufacturer facing downwards (i.e. is not visible when looking at the display).

## Compiling
Simply go to an example and run `make`. If you used the `./build_image` command before (see section "Bootloader"), run a `make clean` before. TODO: Unify the two build options.


## Flashing
Run `arm-none-eabi-gdb` in the applications folder. It should connect to OpenOCD and say something like:
```
TMR_Delay (tmr=tmr@entry=0x40010000, us=us@entry=10000, sys_cfg=sys_cfg@entry=0x0) at Source/tmr_utils.c:59
59	    while (TMR_TO_Check(tmr) != E_TIME_OUT) {}
(gdb) 
```

Enter `load`. It should say something like:
```
(gdb) load
Loading section .text, size 0x12514 lma 0x10010000
Loading section .ARM.exidx, size 0x8 lma 0x10022514
Loading section .data, size 0x8d8 lma 0x1002251c
Start address 0x10012160, load size 77300
Transfer rate: 19 KB/sec, 11042 bytes/write.
(gdb)
```

To run the program, type: `mon mww 0x40000004 0x80000000`. This is a quirk as the prototypes to not have a reset line exposed to the debugger. Will be solved with production hardware.

TODO: Provide a make command to flash card10.

## Debugging
After flashing and the initial reset using `mon mww 0x40000004 0x80000000`, you can say `mon reset halt` and then `continue`. You can now debug as usual.

## Serial Console
card10 outputs debug information on the serial console. Baudrate is 115200. The provided USB adapter creates a CDC device (under Linux /dev/ttyACM0). You can use screen to open and view it: `screen /dev/ttyACM0 115200`.


## Bootloader
The bootloader is used to flash card10 without an external debugger. It exposes the file system via USB and accepts a `card10.bin` file as firmware image.

To flash the bootloader, go to the `bootloader` directory and follow the steps above.

To execute the bootloader, turn off card10, press the lower right button and turn on. Connect card10 via a USB cable to a computer and mount the file system. You can now copy your card10.bin file to the file system. Eject the drive (IMPORTANT!) via your operating system (`umount` and `sync` is not enough). Turn card10 off and back on again. It will try to boot the image.

IMPORTANT: The prototypes have the USB data lines reversed. Use the provided USB adapter to connect card10.

### Building card10.bin
Run `./build_image`. TODO: Add to makefile

The tool uses Python to generate a CRC. Make sure to have `python-crc16` installed.

## Examples
### Hello World
Turns on the display, enumerates I2C, prints debug information on the console.

