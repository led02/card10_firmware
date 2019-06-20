#!/bin/sh
set -e

PYTHON="$1"
ELF="$2"
BIN="$3"

arm-none-eabi-objcopy -O binary "$ELF" "$BIN"
"$PYTHON" "$(dirname "$0")/crc_patch.py" "$BIN"
