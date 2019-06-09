#!/usr/bin/env python3
# Usage: gen-modules.py <source path> <output.h> <input.c ...>

import sys
import os


def main():
    # Get 'official' makemoduledefs.py
    path = sys.argv[1] + "/micropython/py"
    sys.path.insert(0, path)
    import makemoduledefs

    modules = set()
    for source in sys.argv[3:]:
        modules |= makemoduledefs.find_module_registrations(source)

    stdout = sys.stdout
    with open(sys.argv[2], "w") as f:
        sys.stdout = f
        makemoduledefs.generate_module_table_header(sorted(modules))
    sys.stdout = stdout

    try:
        os.mkdir(os.path.dirname(sys.argv[2]) + "/genhdr")
    except FileExistsError:
        pass

    linkname = os.path.dirname(sys.argv[2]) + "/genhdr/" + os.path.basename(sys.argv[2])
    if os.path.exists(linkname):
        os.unlink(linkname)
    os.symlink(
        "../" + os.path.basename(sys.argv[2]),
        linkname,
    )


if __name__ == "__main__":
    main()
