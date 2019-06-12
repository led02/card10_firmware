Cordio Profile
==============

Document No. ARM-EPM-136035 2.0


Legal
-----

Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
ARM Ltd. confidential and proprietary.


Changes
-------

The following changes were made in r2p3-02rel0.

Enhancements in this release:

    WBUSW-2134  SoC Cordio Datc Fails to Reconnect After Initial Connection With Bonded Peer
    WBUSW-2170  Add Make time Configuration for MedC and MedS
    WBUSW-2204  Add WDXS device configuration support for changing PHY
    WBUSW-2234  1.29 Controller and Host Changes (1.29 BT4 Controller Image)
    WBUSW-2292  HCI_HW_ERROR notification in application
    WBUSW-2296  Create Stack and Profiles and WSF Doxygen

Resolved defects in this release:

    WBUSW-2123  CPP Measurement Notification With Size Exceeds Default MTU
    WBUSW-2134  SoC Cordio Datc Fails to Reconnect After Initial Connection With Bonded Peer
    WBUSW-2351  Triage and Fix Coverity Reported Issues

Known limitations in this release:

    None


Compliance
----------

Compliance is verified with the Profile Tuning Suites (PTS) compliance tester using the following version:

    * PTS 7.1.1

The following limitation exists with this version of the PTS:

    * GLS/SEN/CN-BV-12-C: PTS does not check for the characteristic notification in the second record


Folder Organization
-------------------
The folders in this distribution is organized as described below.

 -- cordio-pan-software             # Root software folder
    |-- documentation               # Documentation
    |-- projects                    # IDE project files
    |   `-- ble-apps                #     BLE application projects
    |-- ble-profiles                # BLE profiles component
    |   |-- build                   #     Build utilities
    |   |-- include                 #     Public interfaces
    |   `-- sources                 #     Source files
    |       |-- apps                #     Sample application files
    |       |-- profiles            #     Profile files
    |       `-- services            #     Services files
    |-- platform                    # Platform component
    |   |-- common                  #     Common files
    |   |   |-- build               #         Build utilities
    |   |   |-- include             #         Public interface
    |   |   `-- sources             #         Source files
    |   `-- bt4                     #     BT4 host files
    |   |   |-- build               #         Build utilities
    |   |   |-- img                 #         BT4 controller images
    |   |   |-- include             #         Public interface
    |   |   `-- sources             #         Source files
    |   |   |-- tools               #         Platform tools
    `-- wsf                         # Wireless Software Foundation component
        |-- common                  #     Common files
        |   |-- include             #         Public interface
        |   `-- sources             #         Source files
        `-- baremetal               #     Baremetal files
            |-- build               #         Build utilities
            `-- sources             #         Source files


Tool Dependencies
-----------------

Building and debugging projects for the Cordio BT4 Evaluation Board depend on the
latest GCC or Keil toolchains.  Please see the Cordio BT4 Customer Evaluation and
Demonstration Kit User's Guide for more information on this platform.

The GCC toolchain can be obtained from the following location:

    https://developer.arm.com/open-source/gnu-toolchain/gnu-rm

For Windows, GNU utilities (i.e. make, grep, ls, etc.) can be obtained from:

    http://mingw.org/wiki/msys

The Keil embedded development tools can be obtained from the following location:

    http://www.keil.com/


Change History
--------------

=== Changed in r2p3-00eac0 ===

Enhancements in this release:

    None

Resolved defects in this release:

    WBUSW-1904  Datc crashes if Streaming stopped after 5 seconds
    WBUSW-1942  Incorrect WSF buffer sizes for EXACTLE=1 builds
    WBUSW-1949  Correctly initialize medc_wsp service discovery
    WBUSW-2105  Tag uses wrong peer address type with directed advertising

Known limitations in this release:

    WBUSW-2134  SoC Cordio Datc Fails to Reconnect After Initial Connection With Boded Peer


=== Changed in r2p2-00bet0 ===

Enhancements in this release:

    WBUSW-1493  BLE5 - App Framework Support for LE AE Periodic Advertising
    WBUSW-1671  Add Multiple Connection Support to DM and AF
    WBUSW-1675  Move All Common WDXC/S Defines into New File
    WBUSW-1683  Sample Project Master that supports multiple slaves
    WBUSW-1693  Add verification to txPhy and rxPhy in the scripts
    WBUSW-1794  Remove the WDXS_INCLUDED Compile Option for Cordio Dats

Resolved defects in this release:

    WBUSW-1142  Cosmetic errors in profile code
    WBUSW-1589  Create Sample Project w/ Master and Slave support
    WBUSW-1648  App should use 'pAttCfg' to set MTU size
    WBUSW-1763  Sending commands fast causes the WDXC to use a pointer that isn't setup until discovery completes

Known limitations in this release:

    WBUSW-1904  Datc crashes if Streaming stopped after 5 seconds


See cordio_stack_readme.md for change history prior to r2p2-00bet0.
