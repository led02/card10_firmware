name = 'imutest'

executable(
  name + '.elf',
  'main.c',
  dependencies: [libcard10, max32665_startup],
  link_whole: max32665_startup_lib,
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
