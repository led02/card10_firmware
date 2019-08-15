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

if get_option('debug_prints')
  add_global_arguments(
    ['-DLOG_ENABLE_DEBUG=1'],
    language: 'c',
  )
endif

add_global_link_arguments(
  '-Wl,--gc-sections',
  '-lm',
  language: 'c',
)

# python3 = import('python').find_installation('python3')
python3 = 'python3'

# Version Header
version_hdr = custom_target(
  'card10-version.h',
  output: 'card10-version.h',
  build_by_default: true,
  build_always_stale: true,
  command: [files('tools/version-header.sh'), '@OUTPUT@'],
)

jq = find_program('jq', required: false)
if jq.found()
  compile_commands = custom_target(
    'compile_commands_tidy.json',
    build_by_default: true,
    output: 'compile_commands_tidy.json',
    command: [
      files('tools/convert-ccjson.sh'),
      meson.current_source_dir(),
      meson.current_build_dir(),
      '@OUTPUT@',
    ],
  )
endif


subdir('lib/')
subdir('bootloader/')

subdir('epicardium/')
subdir('pycardium/')

subdir('hw-tests/')

subdir('l0dables/')
