name = 'upy-minimal'

#################################
# MicroPython Generated Headers #
#################################

version_h = custom_target(
  'mpversion.h',
  output: 'mpversion.h',
  command: [
    micropython_gen_version,
    python3,
    micropython_dir,
    '@OUTPUT@',
  ],
)

modules_h = custom_target(
  'moduledefs.h',
  output: 'moduledefs.h',
  input: micropython_sources,
  command: [micropython_gen_modules, '@OUTPUT@', '@INPUT@'],
)

qstr_h = custom_target(
  'qstrdefs.generated.h',
  output: 'qstrdefs.generated.h',
  input: [modules_h, version_h, micropython_sources],
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
  qstr_h,
  include_directories: micropython_includes,
  dependencies: [libcard10, max32665_startup],
  link_with: upy,
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
