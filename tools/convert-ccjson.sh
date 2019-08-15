#!/usr/bin/env bash
set -e

if [[ "$#" == 0 ]]; then
    echo "usage: $0 <sourcedir> <compile_commands_file>"
    exit 1
fi

script_dir="$(dirname "$0")"
source_dir="$1"
build_dir="$2"
output="$3"

jq -f "$script_dir/compile_commands.jq" "$build_dir/compile_commands.json" >"$source_dir/compile_commands.json"

if [[ "$output" != "" ]]; then
    touch "$output"
fi
