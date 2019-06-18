name = 'pycardium'

modsrc = files(
  'modules/utime.c',
  'modules/leds.c',
)

#################################
# MicroPython Generated Headers #
#################################

version_h = custom_target(
  'mpversion.h',
  output: 'mpversion.h',
  command: [micropython_gen_version, '@OUTPUT@'],
)

modules_h = custom_target(
  'moduledefs.h',
  output: 'moduledefs.h',
  input: [micropython_sources, modsrc],
  command: [micropython_gen_modules, '@OUTPUT@', '@INPUT@'],
)

qstr_h = custom_target(
  'qstrdefs.generated.h',
  output: 'qstrdefs.generated.h',
  input: [
    'modules/qstrdefs.h',
    'mpconfigport.h',
    micropython_sources,
  ],
  depends: [modules_h, version_h],
  command: [micropython_gen_qstr, meson.current_source_dir(), '@OUTPUT@', '@INPUT@'],
)

mp_headers = [version_h, modules_h, qstr_h]

###################
# MicroPython Lib #
###################

upy = static_library(
  'micropython',
  micropython_sources,
  micropython_additional_sources,
  mp_headers,
  include_directories: micropython_includes,
)

executable(
  name + '.elf',
  'main.c',
  'mphalport.c',
  modsrc,
  mp_headers,
  include_directories: micropython_includes,
  dependencies: [max32665_startup_core1, board_card10, periphdriver, api_caller],
  link_with: upy,
  link_whole: [max32665_startup_core1_lib, board_card10_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
