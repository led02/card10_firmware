Wireless Software Foundation Release Notes
==========================================

Document No. ARM-EPM-137788 2.0

Legal
-----

Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
Arm Ltd. confidential and proprietary.


Changes
-------

The following changes were made in r2p3-03rel0.

Enhancements in this release:

    WBUSW-2296  Doxygen API documentation
    WBUSW-2310  Increased default WSF_MAX_HANDLER to support applications with larger tasks

Resolved defects in this release:

    None

Known limitations in this release:

    WBUSW-153   Tokenized trace messages limited with special characters

Incomplete features in this release:

    None


Folder Organization
-------------------

The folders in this distribution is organized as described below.

 -- cordio-software                 # Root software folder
    |-- documentation               # Documentation
    `-- wsf                         # Wireless Software Foundation component
        |-- common                  #     Common files
        |   |-- include             #         Public interface
        |   `-- sources             #         Source files
        `-- baremetal               #     Baremetal files
            |-- build               #         Build utilities
            `-- sources             #         Source files


Change History
--------------

Changes in r2p3-02eac0.

    WBUSW-2098  [EBQ] Disconnection Complete Event is received before supervision timeout

