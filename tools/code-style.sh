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

formatter_blacklist=(
    lib/ff13/
    lib/FreeRTOS/
    lib/FreeRTOS-Plus/
    lib/gfx/
    lib/micropython/
    lib/mx25lba/
    lib/sdk/
    lib/vendor/
    openocd/
    docker/
)

for source_file in "$@"; do
    # Check if file is actually one which we want to check
    rel_path="$(realpath --relative-to="$script_dir/.." "$source_file")"
    format_item=true
    for blacklist_item in "${formatter_blacklist[@]}"; do
        if [[ "$rel_path" == "$blacklist_item"* ]]; then
            format_item=false
            break
        fi
    done
    if [[ "$format_item" == false ]]; then
        echo -e "\tIGN\t\t$source_file"
        continue
    fi

    if [[ "$source_file" == *.c ]]; then
        echo -e "\tCLANG-FORMAT\t$source_file"
        clang-format -i "$source_file"
        python3 "$script_dir/fix-multi-decl.py" "$source_file"
    elif [[ "$source_file" == *.py ]]; then
        echo -e "\tBLACK\t\t$source_file"
        black -q "$source_file"
    else
        echo " - Ignoring $source_file" >&2
	fi
done
