includes = include_directories(
  './',
)

lib = static_library(
  'max77650',
  'MAX77650-Arduino-Library.c',
  dependencies: periphdriver,
)

max77650 = declare_dependency(
  include_directories: includes,
  link_with: lib,
)
