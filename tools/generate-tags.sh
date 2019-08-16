#!/usr/bin/env bash
set -e

source_dir="$(dirname "$0")/.."

find "$source_dir" -name '*.[ch]' \
    | grep -v "$source_dir/lib/sdk/Libraries/FreeRTOS" \
    | grep -v "$source_dir/lib/FreeRTOS-Plus" \
    | grep -v "$source_dir/lib/micropython/micropython/ports" \
    | grep -v "$source_dir/lib/micropython/micropython/lib/cmsis" \
    | grep -v "$source_dir/hw-tests/upy-minimal" \
    | xargs -d\\n ctags
