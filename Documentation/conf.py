import os
import subprocess
import sys
import time
import sphinx.util.logging

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
sys.path.insert(0, os.path.abspath("../pycardium/modules/py"))
sys.path.insert(0, os.path.abspath("./"))

logger = sphinx.util.logging.getLogger("card10/conf.py")


# -- Project information -----------------------------------------------------

project = "card10-firmware"
copyright = "2019"

# The full version, including alpha/beta/rc tags
release = (
    subprocess.check_output(["git", "describe", "--long", "--always"]).decode().strip()
)
release += "<br />"
release += time.strftime("%F %R")
version = release


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.viewcode",
    "sphinx.ext.ifconfig",
    "sphinx.ext.todo",
]

todo_include_todos = True

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ["output", "Thumbs.db", ".DS_Store", "hawkmoth"]

# -- Options for HTML output ------------------------------------------------- {{{

# The Read the Docs theme is available from
# - https://github.com/snide/sphinx_rtd_theme
# - https://pypi.python.org/pypi/sphinx_rtd_theme
# - python-sphinx-rtd-theme package (on Debian)
try:
    import sphinx_rtd_theme

    html_theme = "sphinx_rtd_theme"
    html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
    pygments_style = "monokai"
except ImportError:
    logger.warning(
        'The Sphinx "sphinx_rtd_theme" HTML theme was not found. Make sure you have the theme installed to produce pretty HTML output. Falling back to the default theme.'
    )


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ["static"]

# Theme Options
html_theme_options = {"style_external_links": True}

# Show "Edit on GitLab" links
html_show_sourcelink = False
html_context = {
    "display_gitlab": True,
    "gitlab_host": "git.card10.badge.events.ccc.de",
    "gitlab_user": "card10",
    "gitlab_repo": "firmware",
    "gitlab_version": "master/",
    "conf_py_path": "Documentation/",
    "theme_vcs_pageview_mode": "edit",
}
# }}}

# -- Options for Auto-Doc ---------------------------------------------------- {{{
autodoc_mock_imports = [
    "buttons",
    "interrupt",
    "sys_display",
    "sys_leds",
    "sys_max30001",
    "ucollections",
    "urandom",
    "utime",
]

autodoc_member_order = "bysource"
# }}}

# -- Options for Hawkmoth ---------------------------------------------------- {{{
has_hawkmoth = False
try:
    # Attempt importing hawkmoth
    import hawkmoth  # noqa: F401

    extensions.append("hawkmoth")

    cautodoc_root = os.path.abspath("..")
    cautodoc_clang = "-D__SPHINX_DOC"
    has_hawkmoth = True
except ImportError as e:
    if e.name == "clang":
        logger.warning(
            "hawkmoth requires the clang python module.  Documentation for Epicardium API will not be generated."
        )
# }}}


# -- Sphinx Setup ------------------------------------------------------------
def setup(app):
    app.add_config_value("has_hawkmoth", has_hawkmoth, "")
