name = 'pycardium'

modsrc = files(
  'modules/interrupt.c',
  'modules/leds.c',
  'modules/sys_display.c',
  'modules/utime.c',
  'modules/vibra.c',
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
  input: [micropython_mod_sources, modsrc],
  command: [micropython_gen_modules, '@OUTPUT@', '@INPUT@'],
)

qstr_h = custom_target(
  'qstrdefs.generated.h',
  output: 'qstrdefs.generated.h',
  input: [
    'modules/qstrdefs.h',
    'mpconfigport.h',
    micropython_sources,
    micropython_extmod_sources,
  ],
  depends: [version_h],
  command: [micropython_gen_qstr, meson.current_source_dir(), '@OUTPUT@', '@INPUT@'],
)

mp_headers = [version_h, modules_h, qstr_h]

#################################
#    Python Frozen Modules      #
#################################

subdir('modules/py')

frozen_source = custom_target(
  'frozen.c',
  output: 'frozen.c',
  input: [qstr_h, frozen_modules],
  build_by_default: true,
  command: [micropython_gen_frozen, '@OUTPUT@', '@INPUT@'],
)

###################
# MicroPython Lib #
###################

upy = static_library(
  'micropython',
  micropython_sources,
  micropython_additional_sources,
  micropython_extmod_sources,
  mp_headers,
  include_directories: micropython_includes,
)

elf = executable(
  name + '.elf',
  'main.c',
  'mphalport.c',
  frozen_source,
  modsrc,
  mp_headers,
  include_directories: micropython_includes,
  dependencies: [max32665_startup_core1, periphdriver, api_caller],
  link_with: upy,
  link_whole: [max32665_startup_core1_lib, api_caller_lib],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)

bin = custom_target(
  name + '.bin',
  build_by_default: true,
  output: name + '.bin',
  input: elf,
  command: [build_image, '@INPUT@', '@OUTPUT0@'],
)

custom_target(
  name + '_epicardium.bin',
  build_by_default: true,
  output: name + '_epicardium.bin',
  input: [epicardium_bin, bin],
  command: [build_multi_image, '@INPUT@', '@OUTPUT0@'],
)
