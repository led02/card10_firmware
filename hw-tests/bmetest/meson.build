name = 'bmetest'

executable(
  name + '.elf',
  'main.c',
  dependencies: [libcard10, max32665_startup],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
