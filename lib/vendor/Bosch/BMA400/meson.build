includes = include_directories(
  './',
)

sources = files(
  './bma400.c',
)

lib = static_library(
  'bma400',
  sources,
  include_directories: includes,
)

bma400 = declare_dependency(
  include_directories: includes,
  link_with: lib,
)
