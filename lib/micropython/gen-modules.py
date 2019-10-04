#!/usr/bin/env python3
# Usage: gen-modules.py <source path> <output.h> <input.c ...>

import sys
import os
import tempfile
import shutil


def main():
    # Get 'official' makemoduledefs.py
    path = sys.argv[1] + "/micropython/py"
    sys.path.insert(0, path)
    import makemoduledefs

    modules = set()
    for source in sys.argv[3:]:
        modules |= makemoduledefs.find_module_registrations(source)

    stdout = sys.stdout
    with tempfile.TemporaryFile("w+") as temp:
        sys.stdout = temp
        makemoduledefs.generate_module_table_header(sorted(modules))
        sys.stdout = stdout

        # Read contents of existing file and compare
        try:
            with open(sys.argv[2], "r") as f:
                old_content = f.read()
        except FileNotFoundError:
            old_content = ""

        temp.seek(0)
        new_content = temp.read()

        if new_content == old_content:
            # If both file contain the same content, exit early
            sys.exit(0)

        with open(sys.argv[2], "w") as f:
            f.write(new_content)

        try:
            os.mkdir(os.path.dirname(sys.argv[2]) + "/genhdr")
        except FileExistsError:
            pass

        linkname = (
            os.path.dirname(sys.argv[2]) + "/genhdr/" + os.path.basename(sys.argv[2])
        )
        if os.path.exists(linkname):
            os.unlink(linkname)
        shutil.copy(sys.argv[2], linkname)

    sys.stdout = stdout


if __name__ == "__main__":
    main()
