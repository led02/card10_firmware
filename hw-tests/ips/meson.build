name = 'ips'

executable(
  name + '.elf',
  'main.c',
  'image/image.c',
  'image/image2.c',
  dependencies: [libcard10],
  link_args: [
    '-Wl,-Map=' + meson.current_build_dir() + '/' + name + '.map',
  ],
)
