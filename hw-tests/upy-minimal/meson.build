name = 'upy-minimal'

modsrc = files(
  'modules/buzzer.c',
  'modules/leds.c',
  'modules/utime.c',
)

#################################
# MicroPython Generated Headers #
#################################

version_h = custom_target(
  'mpversion-upymin.h',
  output: 'mpversion.h',
  command: [micropython_gen_version, '@OUTPUT@'],
)

modules_h = custom_target(
  'moduledefs-upymin.h',
  output: 'moduledefs.h',
  input: [micropython_sources, modsrc],
  command: [micropython_gen_modules, '@OUTPUT@', '@INPUT@'],
)

qstr_h = custom_target(
  'qstrdefs.generated-upymin.h',
  output: 'qstrdefs.generated.h',
  input: ['modules/qstrdefs.h', micropython_sources],
  depends: [modules_h, version_h],
  command: [micropython_gen_qstr, meson.current_source_dir(), '@OUTPUT@', '@INPUT@'],
)

###################
# MicroPython Lib #
###################

upy = static_library(
  'micropython',
  micropython_sources,
  micropython_additional_sources,
  modules_h,
  qstr_h,
  version_h,
  include_directories: micropython_includes,
)

executable(
  name + '.elf',
  'main.c',
  'uart.c',
  'systick.c',
  modsrc,
  qstr_h,
  include_directories: micropython_includes,
  dependencies: [libcard10, max32665_startup],
  link_whole: [max32665_startup_lib, board_card10_lib],
  link_with: upy,
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
