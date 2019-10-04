#!/usr/bin/env bash
set -e

cd "$(dirname "$0")/.."

# Script to aid in creating a new card10 release
args=$(
    getopt -l "version:,name:,help" -o "v:n:h" -- "$@"
)
eval set -- $args

while [ $# -ge 1 ]; do
    case "$1" in
        -n|--name)
            name="$2"
            shift
            ;;
        -v|--version)
            version="$2"
            shift
            ;;
        -h|--help)
            echo "usage: $0 --name <release-name> --version x.x" >&2
            exit 0
            ;;
    esac
    shift
done

[[ "$name" == "" ]] && { echo "name must be set" >&2; exit 1; }
[[ "$version" == "" ]] && { echo "version must be set" >&2; exit 1; }

# add a leading v to the version
if [[ "$version" =~ ^[^v].*$ ]]; then
    version="v$version"
fi

message() {
    echo "$(tput bold)>>> $(tput sgr0)$(tput setaf 6)$*$(tput sgr0)"
}

release_name="card10-${version}-${name}"

message "Building release \"$release_name\" ..."

git_version="$(git describe --always --dirty)"
if [[ "$git_version" != "$version" ]]; then
    echo "Git says version is \"$git_version\" instead of \"$version\"!" >&2
    exit 1
fi

release_dir="release-$version"
mkdir "$release_dir"

message "Building release version ..."
./bootstrap.sh
ninja -C build/

message "Creating release archive ..."
mkdir "$release_dir/$release_name"
cp -r -t "$release_dir/$release_name" preload/*
cp build/pycardium/pycardium_epicardium.bin "$release_dir/$release_name/card10.bin"
( cd "$release_dir"; zip -r "$release_name"{.zip,}; )

# Copy ELFs
mkdir "$release_dir/elfs"
cp -t "$release_dir/elfs" build/epicardium/epicardium.elf build/pycardium/pycardium.elf build/bootloader/bootloader.elf

message "Done!"
echo "Archive: $release_dir/$release_name.zip"
