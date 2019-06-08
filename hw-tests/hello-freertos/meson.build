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

executable(
  name + '.elf',
  sources,
  include_directories: freertos_sdk_includes,
  dependencies: [libcard10, max32665_startup],
  link_with: freertos,
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
