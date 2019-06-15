includes = include_directories(
  './Source/',
)

sources = files(
  './Source/diskio.c',
  './Source/ff.c',
  './Source/ffsystem.c',
  './Source/ffunicode.c',
)

lib = static_library(
  'ff13',
  sources,
  include_directories: includes,
  dependencies: [periphdriver, mx25lba],
)

libff13 = declare_dependency(
  include_directories: includes,
  link_with: lib,
  dependencies: [periphdriver, mx25lba],
)
