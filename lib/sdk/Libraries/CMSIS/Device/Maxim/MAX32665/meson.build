sources = files(
  './Source/GCC/startup_max32665.S',
  './Source/system_max32665.c',
  './Source/heap.c',
)

lib = static_library(
  'max32665-startup',
  sources,
  dependencies: periphdriver,
)

max32665_startup = declare_dependency(
  link_whole: lib,
  link_args: [
    '-T', meson.current_source_dir() + 'Source/GCC/max32665.ld',
    '--entry', 'Reset_Handler',
  ],
)
