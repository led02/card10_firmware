#!/usr/bin/env bash
set -e

PYTHON="$1"
SOURCE_DIR="$2"
OUTPUT="$3"
QSTR_HEADER="$4"

shift 4

# We need the defs header, not the generated
QSTR_HEADER="$(dirname "$QSTR_HEADER")/qstrdefs.preprocessed.h"

"$PYTHON" "$SOURCE_DIR"/micropython/tools/mpy-tool.py \
    --freeze \
    --qstr-header "$QSTR_HEADER" \
    -mlongint-impl mpz \
    "$@" >"$OUTPUT"
