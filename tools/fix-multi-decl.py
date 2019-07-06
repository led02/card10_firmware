#!/usr/bin/env python3
#
# I prefer to have the closing brace of a multi-line declaration or function call on its
# own line.  This is unfortunately not yet supported by clang-format.  This script is
# a hack to manually introduce my style.
#
# Tracking for this style in clang-format: https://reviews.llvm.org/D33029
#
# Example
# =======
# This is the style I want:
#
#     printf(
#         "Hello %s %d\n",
#         "World",
#         123
#     );
import re
import sys

EXPR = r"""                         # Indentation is saved to <ws>
^(?P<ws>[ \t]*)(?P<decl>.*\(\n      # Opening line, ending with (\n
    (?:.*,\n)*                      # Lines with arguments, all ending with ,\n
    .*)\)(?P<final>[); \t]*)\n      # Last line with closing brace & optional following characters
(?![ \t]*\).*)                      # Don't match an already expanded decl
"""


def main():
    with open(sys.argv[1], "r") as f:
        source = f.read()

    fixed = re.sub(
        EXPR,
        r"\g<ws>\g<decl>\n\g<ws>)\g<final>\n",
        source,
        flags=re.MULTILINE | re.VERBOSE,
    )

    # Fix function definitions that can now be pulled into one line
    fixed = re.sub(
        r"^\)\n{$",
        r") {",
        fixed,
        flags=re.MULTILINE,
    )

    with open(sys.argv[1], "w") as f:
        f.write(fixed)


if __name__ == "__main__":
    main()
