#!/bin/bash
set -e

SOURCE_DIR="$1"
OUTPUT="$(realpath "$2")"

cd "$SOURCE_DIR"/micropython/mpy-cross
make -j "$(nproc)" >/dev/null
cp mpy-cross "$OUTPUT"
