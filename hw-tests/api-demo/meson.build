name = 'api-demo-core0'

executable(
  name + '.elf',
  'main.c',
  dependencies: [libcard10, max32665_startup_core0],
  link_whole: [max32665_startup_core0_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)

name = 'api-demo-core1'

executable(
  name + '.elf',
  'test-payload.c',
  dependencies: [libcard10, max32665_startup_core1],
  link_whole: [max32665_startup_core1_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
