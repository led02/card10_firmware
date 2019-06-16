#!/bin/bash
set -e

PYTHON="$1"
SOURCE_DIR="$2"
PROJECT_SRC="$3"
OUTPUT="$4"
CUSTOM_QSTR="$5"

shift 5

OUTPUT_DIR="$(dirname "$OUTPUT")"

mkdir -p "$OUTPUT_DIR/genhdr"
ln -sfr "$OUTPUT" "$OUTPUT_DIR/genhdr/$(basename "$OUTPUT")"

# call gcc -E to generate qstr.i.last
gcc -E -DNO_QSTR -I"$SOURCE_DIR/micropython" -I"$PROJECT_SRC" -I"$OUTPUT_DIR" "$@" >"$OUTPUT_DIR/qstr.i.last"

# Generate qstr.split
"$PYTHON" "$SOURCE_DIR/micropython/py/makeqstrdefs.py" split \
    "$OUTPUT_DIR/qstr.i.last" "$OUTPUT_DIR/qstr" "$OUTPUT_DIR/qstrdefs.collected.h" >/dev/null

# Generate qstr.collected.h
"$PYTHON" "$SOURCE_DIR/micropython/py/makeqstrdefs.py" cat \
    "$OUTPUT_DIR/qstr.i.last" "$OUTPUT_DIR/qstr" "$OUTPUT_DIR/qstrdefs.collected.h" >/dev/null

# Preprocess Header ... I did not come up with this, this is code copied from
#    the official make file.  Seriously.
cat "$SOURCE_DIR/micropython/py/qstrdefs.h" "$CUSTOM_QSTR" "$OUTPUT_DIR/qstrdefs.collected.h" \
    | sed 's/^Q(.*)/"&"/' \
    | gcc -E -DNO_QSTR -I"$SOURCE_DIR/micropython" -I"$PROJECT_SRC" -I"$OUTPUT_DIR" - \
    | sed 's/^\"\(Q(.*)\)\"/\1/' \
    >"$OUTPUT_DIR/qstrdefs.preprocessed.h"

# Call makeqstrdata
"$PYTHON" "$SOURCE_DIR/micropython/py/makeqstrdata.py" "$OUTPUT_DIR/qstrdefs.preprocessed.h" >"$OUTPUT"
