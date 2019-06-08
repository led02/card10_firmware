includes = include_directories(
  './',
)

sources = files(
  'bosch.c',
  'card10.c',
  'leds.c',
  'pmic.c',
)

deps = [
    bhy1,
    bma400,
    bme680,
    board_card10,
    libgfx,
    max77650,
    periphdriver,
]

lib = static_library(
  'card10',
  sources,
  include_directories: includes,
  dependencies: deps,
)

libcard10 = declare_dependency(
  include_directories: includes,
  link_with: lib,
  dependencies: [deps, max32665_startup],
)
