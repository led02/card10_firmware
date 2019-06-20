#!/bin/sh
set -e

echo "build multi image" $1 $2 $3 $4
PYTHON="$1"
BIN1="$2"
BIN2="$3"
BINOUT="$4"

dd if=/dev/zero ibs=1k count=448 | tr "\000" "\377" > "$BINOUT"
dd if="$BIN1" of="$BINOUT" conv=notrunc
dd if="$BIN2" of="$BINOUT" conv=notrunc oflag=append

"$PYTHON" "$(dirname "$0")/crc_patch.py" "$BINOUT"
