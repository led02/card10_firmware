#!/bin/sh
set -e

PYTHON="$1"
BIN1="$2"
BIN2="$3"
BINOUT="$4"

dd if=/dev/zero ibs=1k count=448 2>/dev/null | LANG=C LC_CTYPE=C LC_ALL=C LC_COLLATE=C tr "\000" "\377" > "$BINOUT"
dd if="$BIN1" of="$BINOUT" conv=notrunc 2>/dev/null
dd if="$BIN2" >> "$BINOUT" 2>/dev/null

"$PYTHON" "$(dirname "$0")/crc_patch.py" "$BINOUT"
