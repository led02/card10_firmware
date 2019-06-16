name = 'epicardium'

elf = executable(
  name + '.elf',
  'main.c',
  dependencies: [libcard10, max32665_startup_core0],
  link_whole: [max32665_startup_core0_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)

custom_target(
  name + '.bin',
  build_by_default: true,
  output: name + '.bin',
  input: elf,
  command: [build_image, '@INPUT@', '@OUTPUT0@'],
)
