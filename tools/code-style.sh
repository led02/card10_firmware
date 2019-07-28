#!/bin/bash
set -e

###########################
# Check tools

# If any C files are formatted
if [[ "$*" == *.c* ]]; then
    if ! command -v clang-format >/dev/null 2>&1; then
        echo "$0: requires clang-format." >&2
        exit 127
    fi

    if ! command -v python3 >/dev/null 2>&1; then
        echo "$0: requires python3." >&2
        exit 127
    fi
fi

# If any Python files are formatted
if [[ "$*" == *.py* ]]; then
    if ! command -v black >/dev/null 2>&1; then
        echo "$0: requires black (python formatter)." >&2
        echo "" >&2
        echo "You can install \`black\` using pip3:" >&2
        echo "" >&2
        echo "	pip3 install --user black" >&2
        echo "" >&2
        exit 127
    fi
fi

if [[ "$#" == 0 ]]; then
    echo "usage: $0 <source.c> ..."
    exit 1
fi

script_dir="$(dirname "$0")"

for source_file in "$@"; do
    # Check if file is actually one which we want to check
    rel_path="$(realpath --relative-to="$script_dir/.." "$source_file")"
    if [[ ( "$rel_path" == lib/* && "$rel_path" != lib/card10/* ) ||
          ( "$rel_path" == openocd/* ) ||
          ( "$rel_path" == ble/* ) ]]; then
        echo " - Ignoring $source_file"
        continue
    fi

    if [[ "$source_file" == *.c ]]; then
        echo " - Formatting $source_file ..."
        clang-format -i "$source_file"
        python3 "$script_dir/fix-multi-decl.py" "$source_file"
    elif [[ "$source_file" == *.py ]]; then
        echo " - Formatting $source_file ..."
        black -q "$source_file"
    else
        echo " - Ignoring $source_file" >&2
	fi
done
