includes = include_directories(
  './Include/',
  '../Include/',
)

sources = files(
  './Source/board.c',
  '../Source/led.c',
  '../Source/mx25.c',
  '../Source/pb.c',
  '../Source/stdio.c',
)

board_card10_lib = static_library(
  'board-card10',
  sources,
  include_directories: includes,
  dependencies: periphdriver,
)

board_card10 = declare_dependency(
  include_directories: includes,
)
