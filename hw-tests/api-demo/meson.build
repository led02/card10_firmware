api_stubs = custom_target(
  'api_*.c',
  input: 'api.h',
  output: ['api_client.c', 'api_server.c'],
  command: [
    python3,
    meson.current_source_dir() + '/genapi.py',
    '-H', '@INPUT0@',
    '-c', '@OUTPUT0@', '-s', '@OUTPUT1@',
  ],
  depend_files: 'genapi.py',
)


name = 'api-demo-core0'

executable(
  name + '.elf',
  'main.c',
  './api/api_caller.c',
  api_stubs[0],
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
  './api/api_dispatcher.c',
  api_stubs[1],
  dependencies: [libcard10, max32665_startup_core1],
  link_whole: [max32665_startup_core1_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
