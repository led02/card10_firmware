l0dable_startup_lib = static_library(
  'l0dable-startup',
  'crt.s',
  pic: true,
)

l0dable_startup = declare_dependency(
  link_args: [
    '-nostdlib', '-n',
    '-T', meson.current_source_dir() + 'core1.ld',
  ],
  compile_args: [
    '-fPIE', '-pie',
  ],
)

subdir('blinky/')
