# Scripts
micropython_gen_modules = [
  python3,
  files('gen-modules.py'),
  meson.current_source_dir(),
]

micropython_gen_version = [
  files('./gen-version.sh'),
  python3,
  meson.current_source_dir(),
]

micropython_gen_qstr = [
  files('gen-qstr.sh'),
  python3,
  meson.current_source_dir(),
]

# mpy-cross
mpy_cross_bin = custom_target(
  'mpy-cross',
  output: 'mpy-cross',
  build_by_default: true,
  command: [files('gen-mpy-cross.sh'), meson.current_source_dir(), '@OUTPUT@'],
)

# seriously meson, this is retarded
mpy_cross_wrapper = executable(
  'mpy-cross-wrapper',
  'mpy-cross-wrapper.c',
  link_depends: mpy_cross_bin,
  native: true,
  c_args: ['-DMPY_CROSS_PATH="' + meson.current_build_dir() + '"'],
)

mpy_cross = generator(
  mpy_cross_wrapper,
  output: '@BASENAME@.mpy',
  arguments: ['-o', '@OUTPUT@', '-s', '@PLAINNAME@', '-march=armv7m', '@INPUT@'],
)

micropython_gen_frozen = [
  files('gen-frozen.sh'),
  python3,
  meson.current_source_dir(),
]


# Sources
micropython_includes = include_directories(
  './micropython/',
  './micropython/lib/utils',
  './micropython/extmod/',
)

micropython_mod_sources = files(
  'micropython/py/modarray.c',
)

micropython_sources = files(
  'micropython/py/argcheck.c',
  'micropython/py/asmarm.c', 'micropython/py/asmbase.c',
  'micropython/py/asmthumb.c',
  'micropython/py/asmx64.c',
  'micropython/py/asmx86.c',
  'micropython/py/asmxtensa.c',
  'micropython/py/bc.c',
  'micropython/py/binary.c',
  'micropython/py/builtinevex.c',
  'micropython/py/builtinhelp.c',
  'micropython/py/builtinimport.c',
  'micropython/py/compile.c',
  'micropython/py/emitbc.c',
  'micropython/py/emitcommon.c',
  'micropython/py/emitglue.c',
  'micropython/py/emitinlinethumb.c',
  'micropython/py/emitinlinextensa.c',
  'micropython/py/emitnarm.c',
  'micropython/py/emitnative.c',
  'micropython/py/emitnthumb.c',
  'micropython/py/emitnx64.c',
  'micropython/py/emitnx86.c',
  'micropython/py/emitnxtensa.c',
  'micropython/py/formatfloat.c',
  'micropython/py/frozenmod.c',
  'micropython/py/gc.c',
  'micropython/py/lexer.c',
  'micropython/py/malloc.c',
  'micropython/py/map.c',
  'micropython/py/modarray.c',
  'micropython/py/modbuiltins.c',
  'micropython/py/modcmath.c',
  'micropython/py/modcollections.c',
  'micropython/py/modgc.c',
  'micropython/py/modio.c',
  'micropython/py/modmath.c',
  'micropython/py/modmicropython.c',
  'micropython/py/modstruct.c',
  'micropython/py/modsys.c',
  'micropython/py/modthread.c',
  'micropython/py/moduerrno.c',
  'micropython/py/mpprint.c',
  'micropython/py/mpstate.c',
  'micropython/py/mpz.c',
  'micropython/py/nativeglue.c',
  'micropython/py/nlr.c',
  'micropython/py/nlrsetjmp.c',
  'micropython/py/nlrthumb.c',
  'micropython/py/nlrx64.c',
  'micropython/py/nlrx86.c',
  'micropython/py/nlrxtensa.c',
  'micropython/py/obj.c',
  'micropython/py/objarray.c',
  'micropython/py/objattrtuple.c',
  'micropython/py/objbool.c',
  'micropython/py/objboundmeth.c',
  'micropython/py/objcell.c',
  'micropython/py/objclosure.c',
  'micropython/py/objcomplex.c',
  'micropython/py/objdeque.c',
  'micropython/py/objdict.c',
  'micropython/py/objenumerate.c',
  'micropython/py/objexcept.c',
  'micropython/py/objfilter.c',
  'micropython/py/objfloat.c',
  'micropython/py/objfun.c',
  'micropython/py/objgenerator.c',
  'micropython/py/objgetitemiter.c',
  'micropython/py/objint.c',
  'micropython/py/objint_longlong.c',
  'micropython/py/objint_mpz.c',
  'micropython/py/objlist.c',
  'micropython/py/objmap.c',
  'micropython/py/objmodule.c',
  'micropython/py/objnamedtuple.c',
  'micropython/py/objnone.c',
  'micropython/py/objobject.c',
  'micropython/py/objpolyiter.c',
  'micropython/py/objproperty.c',
  'micropython/py/objrange.c',
  'micropython/py/objreversed.c',
  'micropython/py/objset.c',
  'micropython/py/objsingleton.c',
  'micropython/py/objslice.c',
  'micropython/py/objstr.c',
  'micropython/py/objstringio.c',
  'micropython/py/objstrunicode.c',
  'micropython/py/objtuple.c',
  'micropython/py/objtype.c',
  'micropython/py/objzip.c',
  'micropython/py/opmethods.c',
  'micropython/py/parse.c',
  'micropython/py/parsenum.c',
  'micropython/py/parsenumbase.c',
  'micropython/py/persistentcode.c',
  'micropython/py/pystack.c',
  'micropython/py/qstr.c',
  'micropython/py/reader.c',
  'micropython/py/repl.c',
  'micropython/py/runtime.c',
  'micropython/py/runtime_utils.c',
  'micropython/py/scheduler.c',
  'micropython/py/scope.c',
  'micropython/py/sequence.c',
  'micropython/py/showbc.c',
  'micropython/py/smallint.c',
  'micropython/py/stackctrl.c',
  'micropython/py/stream.c',
  'micropython/py/unicode.c',
  'micropython/py/vm.c',
  'micropython/py/vstr.c',
  'micropython/py/warning.c',
)

micropython_additional_sources = files(
  'micropython/lib/utils/stdout_helpers.c',
  'micropython/lib/utils/pyexec.c',
  'micropython/lib/mp-readline/readline.c',
  'micropython/lib/timeutils/timeutils.c'
)

micropython_extmod_sources = files(
  'micropython/extmod/utime_mphal.c',
  'micropython/extmod/modbtree.c',
  'micropython/extmod/modframebuf.c',
  'micropython/extmod/modubinascii.c',
  'micropython/extmod/moducryptolib.c',
  'micropython/extmod/moductypes.c',
  'micropython/extmod/moduhashlib.c',
  'micropython/extmod/moduheapq.c',
  'micropython/extmod/modujson.c',
  'micropython/extmod/modurandom.c',
  'micropython/extmod/modure.c',
  'micropython/extmod/moduselect.c',
  'micropython/extmod/modutimeq.c',
)
