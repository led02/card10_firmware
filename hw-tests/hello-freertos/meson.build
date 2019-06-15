name = 'hello-freertos'

freertos = static_library(
  'freertos-sdk',
  freertos_sdk_sources,
  dependencies: periphdriver,
  include_directories: [
    freertos_sdk_includes,
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
  include_directories: freertos_sdk_includes,
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
