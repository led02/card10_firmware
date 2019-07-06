name = 'hello-freertos'

freertos = static_library(
  'freertos',
  freertos_sources,
  freertos_heap2,
  freertos_cli_sources,
  dependencies: periphdriver,
  include_directories: [
    freertos_includes,
    freertos_cli_includes,
    include_directories('./'),
  ]
)

sources = files(
  'CLI-commands.c',
  'freertos_tickless.c',
  'main.c',
)

elf = executable(
  name + '.elf',
  sources,
  include_directories: [freertos_includes, freertos_cli_includes],
  dependencies: [libcard10, max32665_startup],
  link_with: freertos,
  link_whole: [max32665_startup_lib, board_card10_lib],
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
