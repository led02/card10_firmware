includes = include_directories(
  './',
)

sources = files(
  'mx25lba.c',
)

deps = [
    board_card10,
    periphdriver,
]

lib = static_library(
  'mx25lba',
  sources,
  include_directories: includes,
  dependencies: deps,
)

mx25lba = declare_dependency(
  include_directories: includes,
  link_with: lib,
  dependencies: deps,
)
