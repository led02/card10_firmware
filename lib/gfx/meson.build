includes = include_directories(
  './',
  './Fonts/',
  './GUI_DEV/',
  './LCD/',
)

sources = files(
  './GUI_DEV/DEV_Config.c',
  './LCD/LCD_Driver.c',
  './Fonts/font8.c',
  './Fonts/font12.c',
  './Fonts/font16.c',
  './Fonts/font20.c',
  './Fonts/font24.c',
  'framebuffer.c',
  'gfx.c',
  'textbuffer.c'
)

lib = static_library(
  'gfx',
  sources,
  include_directories: includes,
  dependencies: periphdriver,
  c_args: '-Wno-missing-braces',
)

libgfx = declare_dependency(
  include_directories: includes,
  link_with: lib,
)
