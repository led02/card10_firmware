includes = include_directories(
  './',
)

sources = files(
  './bme680.c',
)

lib = static_library(
  'bme680',
  sources,
  include_directories: includes,
)

bme680 = declare_dependency(
  include_directories: includes,
  link_with: lib,
)
