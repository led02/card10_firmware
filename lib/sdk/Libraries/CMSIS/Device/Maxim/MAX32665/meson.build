#############################################################
# For applications using just core0 or not dual-binaries
#############################################################

sources = files(
  './Source/GCC/startup_max32665.S',
  './Source/system_max32665.c',
  './Source/heap.c',
)

max32665_startup_lib = static_library(
  'max32665-startup',
  sources,
  dependencies: periphdriver,
)

max32665_startup = declare_dependency(
  link_args: [
    '-T', meson.current_source_dir() + 'Source/GCC/max32665.ld',
    '--entry', 'Reset_Handler',
  ],
)

#############################################################
# For core0 applications with dual-binaries
#############################################################

sources = files(
  './Source/GCC/startup_max32665.S',
  './Source/system_max32665.c',
  './Source/heap.c',
)

max32665_startup_core0_lib = static_library(
  'max32665-startup-core0',
  sources,
  dependencies: periphdriver,
)

max32665_startup_core0 = declare_dependency(
  link_args: [
    '-T', meson.current_source_dir() + 'Source/GCC/max32665_core0.ld',
    '--entry', 'Reset_Handler',
  ],
)

#############################################################
# For core1 applications with dual-binaries
#############################################################

sources = files(
  './Source/GCC/startup_max32665.S',
  './Source/system_max32665_core1.c',
  './Source/heap.c',
)

max32665_startup_core1_lib = static_library(
  'max32665-startup-core1',
  sources,
  dependencies: periphdriver,
)

max32665_startup_core1 = declare_dependency(
  link_args: [
    '-T', meson.current_source_dir() + 'Source/GCC/max32665_core1.ld',
    '--entry', 'Reset_Handler',
  ],
)
