#!/bin/sh
set -e

# makeversionhdr has to be executed in the micropython subdir
# so it picks up the correct version
OUT="$(realpath "$3")"
OUTDIR="$(dirname "$OUT")"

mkdir -p "$OUTDIR/genhdr"
ln -sfr "$OUT" "$OUTDIR/genhdr/$(basename "$OUT")"

cd "$2/micropython"
"$1" "$2/micropython/py/makeversionhdr.py" "$OUT" >/dev/null
