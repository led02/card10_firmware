includes = include_directories(
  'include/core/musbhsfc',
  'include/core',
  'include/devclass',
  'include/enumerate',
  'include/util',
  'include/dbg_log',
)

sources = files(
  'src/core/musbhsfc/usb.c',
  'src/core/usb_event.c',
  'src/dbg_log/dbg_log.c',
  'src/devclass/cdc_acm.c',
  'src/devclass/hid_kbd.c',
  'src/devclass/hid_raw.c',
  'src/devclass/msc.c',
  'src/enumerate/enumerate.c',
  'src/util/fifo.c',
)

lib = static_library(
  'maxusb',
  sources,
  include_directories: includes,
  dependencies: periphdriver,
  c_args: '-w',
)

maxusb = declare_dependency(
  include_directories: includes,
  link_with: lib,
  dependencies: periphdriver,
)
