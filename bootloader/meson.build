name = 'bootloader'

splash_screen = custom_target(
  'splash-screen.h',
  output: 'splash-screen.h',
  input: 'splash-screen.png',
  command: [
    python3,
    meson.current_source_dir() + '../tools/bootloader-image.py',
    '-n', 'splash',
    '@INPUT@',
    '@OUTPUT@',
  ],
)

executable(
  name + '.elf',
  'main.c',
  'mscmem.c',
  'bootloader-display.c',
  'bootloader-usb.c',
  'crc16-ccitt.c',
  splash_screen,
  version_hdr,
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

# build_multi_image.sh
build_multi_image = [files('./build_multi_image.sh'), python3]
