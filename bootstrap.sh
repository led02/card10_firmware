#!/bin/sh
set -xe

cd "$(dirname "$0")"
test -d build/ && rm -r build/

git submodule update --init ./lib/micropython
meson --cross-file card10-cross.ini build/ "$@"

set +x

echo "---------------------------------------------------------------"
echo "   Build configured successfully!"
echo "---------------------------------------------------------------"
echo "   You can now start building using"
echo "       ninja -C build/"
echo ""
echo "   The path after -C is the path to the build-directory."
echo "   Suppose you are in 'hw-tests/hello-world/', you could"
echo "   then run 'ninja -C ../../build/'"
echo ""
echo "   You can also build a single target.  For example:"
echo "       ninja -C build/ hw-tests/hello-world/hello-world.elf"
