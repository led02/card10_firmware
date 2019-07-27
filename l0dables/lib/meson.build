l0dable_startup_lib = static_library(
  'l0dable-startup',
  'crt.s',
  'hardware.c',
  dependencies: [periphdriver, api_caller],
  pic: true,
)

l0dable_startup = declare_dependency(
  link_args: [
    '-nostdlib', '-n',
    '-T', meson.current_source_dir() + 'l0dable.ld',
  ],
  compile_args: [
    '-fPIE', '-pie',
  ],
)

