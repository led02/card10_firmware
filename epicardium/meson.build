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
  'api/interrupt-receiver.c',
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
  'api/interrupt-sender.c',
  'api/control.c',
  api[1], # Dispatcher
  dependencies: [libcard10, periphdriver],
)

##########################################################################
#
# FreeRTOS
#
##########################################################################

freertos = static_library(
  'freertos',
  freertos_sources,
  freertos_heap3,
  dependencies: periphdriver,
  include_directories: [
    freertos_includes,
    include_directories('./'),
  ],
)

##########################################################################
#
# Epicardium executable
#
##########################################################################

subdir('modules/')
subdir('ble/')

subdir('l0der/')

elf = executable(
  name + '.elf',
  'cdcacm.c',
  'main.c',
  'support.c',
  'fs/filesystem_fat.c',
  module_sources,
  l0der_sources,
  ble_sources,
  version_hdr,
  dependencies: [libcard10, max32665_startup_core0, maxusb, libff13, ble, bhy1],
  link_with: [api_dispatcher_lib, freertos],
  link_whole: [max32665_startup_core0_lib, board_card10_lib, newlib_heap_lib],
  include_directories: [freertos_includes],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
    '-Wl,--defsym=_write=_write_epicardium',
  ],
)

epicardium_bin = custom_target(
  name + '.bin',
  build_by_default: true,
  output: name + '.bin',
  input: elf,
  command: [build_image, '@INPUT@', '@OUTPUT0@'],
)
