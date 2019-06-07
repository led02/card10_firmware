includes = include_directories(
  './Include/',
  '../Include/',
)

sources = files(
  './Source/board.c',
  '../Source/led.c',
  '../Source/mx25.c',
  '../Source/pb.c',
)

lib = static_library(
  'board-card10',
  sources,
  include_directories: includes,
  dependencies: periphdriver,
)

libstdio = static_library(
  'board-stdio',
  '../Source/stdio.c',
  include_directories: includes,
  dependencies: periphdriver,
)

board_card10 = declare_dependency(
  include_directories: includes,
  link_with: lib,
  link_whole: libstdio,
)
