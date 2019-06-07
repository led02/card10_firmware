project(
  'card10-firmware',
  'c',
  default_options: [
    'buildtype=minsize',
    'c_lto=true',
    'c_std=c99',
    'b_staticpic=false',
    'b_asneeded=false',
  ],
)

assert(
  meson.is_cross_build(),
  'card10-firmware can only be cross-compiled for card10.\n' +
  'Please use `--cross-file card10-cross.ini`.',
)

add_global_arguments(
  meson.get_cross_property('target_defs'),
  language: 'c',
)

add_global_link_arguments(
  '-Wl,--gc-sections',
  '-lm',
  language: 'c',
)

subdir('lib/')
subdir('hw-tests/')
