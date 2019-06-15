name = 'bootloader'

executable(
  name + '.elf',
  'main.c',
  'mscmem.c',
  'bootloader-usb.c',
  'crc16-ccitt.c',
  dependencies: [
    libcard10,
    max32665_startup_boot,
    libff13,
    maxusb,
  ],
  link_whole: [max32665_startup_boot_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)

# build_image.sh
build_image = [files('./build_image.sh'), python3]
