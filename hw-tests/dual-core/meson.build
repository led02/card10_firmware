name = 'dual-core0'
executable(
  name + '.elf',
  'main.c',
  dependencies: [libcard10, max32665_startup_core0],
  link_whole: [max32665_startup_core0_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)

name = 'dual-core1'
executable(
  name + '.elf',
  'core1.c',
  dependencies: [periphdriver, board_card10, max32665_startup_core1],
  link_whole: [max32665_startup_core1_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
