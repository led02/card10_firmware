name = 'blinky'

elf = executable(
  name + '.elf',
  'main.c',
  build_by_default: true,
  dependencies: [l0dable_startup, api_caller],
  link_whole: [l0dable_startup_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
  pie: true,
)
