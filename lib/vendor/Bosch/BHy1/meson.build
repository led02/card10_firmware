includes = include_directories(
  './driver/inc/',
  './examples/firmware/',
  # For bosch.h
  '../../../card10/',
)

sources = files(
  './driver/src/bhy.c',
  './driver/src/bhy_support.c',
  './driver/src/bhy_uc_driver.c',
)

lib = static_library(
  'bhy1',
  sources,
  include_directories: includes,
  dependencies: periphdriver,
  c_args: '-w',
)

bhy1 = declare_dependency(
  include_directories: includes,
  link_with: lib,
)
