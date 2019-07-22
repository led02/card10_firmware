#!/bin/bash
set -e

# Check tools
if ! command -v clang-format >/dev/null 2>&1; then
    echo "$0: requires clang-format." >&2
    exit 127
fi

if ! command -v python3 >/dev/null 2>&1; then
    echo "$0: requires python3." >&2
    exit 127
fi

if [[ "$#" == 0 ]]; then
    echo "usage: $0 <source.c> ..."
    exit 1
fi

script_dir="$(dirname "$0")"

for source_file in "$@"; do
    if [[ "$source_file" == *.c ]]; then
        echo "Formatting $source_file ..."
        clang-format -i "$source_file"
        python3 "$script_dir/fix-multi-decl.py" "$source_file"
    else
        echo "Not a C file: $source_file" >&2
        continue
    fi
done
