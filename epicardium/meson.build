name = 'epicardium'

##########################################################################
#
# API
#
##########################################################################

api = custom_target(
  'api_*.c',
  input: 'epicardium.h',
  output: ['api_caller.c', 'api_dispatcher.c'],
  command: [
    python3,
    meson.current_source_dir() + '/api/genapi.py',
    '-H', '@INPUT0@',
    '-c', '@OUTPUT0@', '-s', '@OUTPUT1@',
  ],
  depend_files: 'api/genapi.py',
)

api_caller_lib = static_library(
  'api-caller',
  'api/caller.c',
  api[0], # Caller
  dependencies: periphdriver,
)

api_caller = declare_dependency(
  include_directories: include_directories('.'),
  link_with: api_caller_lib,
  dependencies: periphdriver,
)

api_dispatcher_lib = static_library(
  'api-dispatcher',
  'api/dispatcher.c',
  api[1], # Dispatcher
  dependencies: periphdriver,
)

##########################################################################
#
# Epicardium executable
#
##########################################################################

elf = executable(
  name + '.elf',
  'main.c',
  dependencies: [libcard10, max32665_startup_core0],
  link_with: api_dispatcher_lib,
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
