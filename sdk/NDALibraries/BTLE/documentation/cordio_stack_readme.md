Cordio Stack
============

Document No. ARM-EPM-115978 2.0


Legal
-----

Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
ARM Ltd. confidential and proprietary.


Changes
-------

The following changes were made in r2p3-02rel0.

Enhancements in this release:

    WBUSW-1701  HCI HW Error Event Handling to Application
    WBUSW-2130  SoC App Drops Connections within 1-3 mins
    WBUSW-2292  HCI_HW_ERROR notification in application
    WBUSW-2296  Create Stack and Profiles and WSF Doxygen

Resolved defects in this release:

    WBUSW-2188  DM_ADV_SET_STOP_IND is not sent to Application when host disables advertising
    WBUSW-2289  SMP SC Responder Numeric Comparison Accepted before Initiator is Ready
    WBUSW-2351  Triage and Fix Coverity Reported Issues

Known limitations in this release:

    None


Compliance
----------

This release complies with the Bluetooth Core.TCRL.2017-2 definitions using the following test
specification versions:

    * GAP.TS.5.0.2
    * L2CAP.TS.5.0.2
    * GATT.TS.5.0.2
    * SMP.TS.5.0.2


Folder Organization
-------------------
The folders in this distribution is organized as described below.

 -- cordio-pan-software             # Root software folder
    |-- documentation               # Documentation
    |-- projects                    # IDE project files
    |   `-- ble-apps                #     BLE application projects
    |-- ble-host                    # BLE Host component
    |   |-- build                   #     Build utilities
    |   |-- include                 #     Public interfaces
    |   `-- sources                 #     Source files
    |       |-- hci                 #         HCI files
    |       |-- sec                 #         Security files
    |       `-- stack               #         Host stack files
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

Doxygen generation has been tested with doxygen v1.8.14 and Graphviz 2.38  


Change History
--------------

=== Changed in r2p3-00eac0 ===

Enhancements in this release:

    None

Resolved defects in this release:

    None

Known limitations in this release:

    WBUSW-1820  Host may Send HCI_Reset without Waiting for the Status of LE_Create_Connection_Cancel Command

=== Changed in r2p2-00bet0 ===

Enhancements in this release:

    WBUSW-1597  Add versioning to stack
    WBUSW-1653  Move ERROR codes for HCI to ATT
    WBUSW-1656  Add support for read local version complete event
    WBUSW-1667  Add Central Support to DM Privacy Module
    WBUSW-1671  Add Multiple Connection Support to DM and AF
    WBUSW-1674  Multiple slave to master connections
    WBUSW-1682  Architecture-based folder organization
    WBUSW-1718  Add DM Support for HciReadRemoteVersion cmd
    WBUSW-1773  Cross-Protocol project interface for all platform components

Resolved defects in this release:

    WBUSW-1547  LE Set Random Address Should not occur during advertising or scanning
    WBUSW-1652  Host should pass ATT prepare write response data up to app
    WBUSW-1673  Failure to copy handle and status during hci_evt processing
    WBUSW-1730  SMP Issue with uninitialized security level
    WBUSW-1742  LESC requires 128-bit strength encryption key
    WBUSW-1776  CCCD Value of WDXS Got Overwritten after Pairing
    WBUSW-1892  MBED Driver issue for Windows 10 Machines

Known limitations in this release:

    WBUSW-1820  Host may Send HCI_Reset without Waiting for the Status of LE_Create_Connection_Cancel Command

=== Changed in r2p1-00eac ===

    * WBUSW-0929: Mandatory to support User Data Service for the WSP collector role
    * WBUSW-1424: Detokenized LL trace messages
    * WBUSW-1451: Dynamic ATT Tables
    * WBUSW-1491: Connect DM Adv events to LL Adv enable command complete event
    * WBUSW-1492: Zero-Copy ATT Notification/Indication
    * WBUSW-1494: BLE5 - Create Max Throughput Apps
    * WBUSW-1530: Add Flow Control Support for Multiple ATT Notifications
    * WBUSW-1535: Buffer Pool Allocation  Diagnostics
    * WBUSW-1536: Thin-HCI Zero Copy Support
    * WBUSW-1544: ATT Signed Write Should Comply with E4243
    * WBUSW-1338: Connection Update Request state machine problem
    * WBUSW-1450: AppDbGetNextRecord loop goes beyond array bounds
    * WBUSW-1484: Cordio Stack DmDevReset() for Soft Reset doesn't reset state
    * WBUSW-1539: Stack Not Processing the Fragmented AdvData to the App Correctly
    * WBUSW-1625: L2C_COC_CBACK_START callback event overlaps with some DM callback events

=== Changed in r2p0-10eac ===

    * WBUSW-1210    TSE6680 support ofr LeCoC
    * WBUSW-1247    BLE5 - Host Support for LE AE Master Non-Periodic
    * WBUSW-1307    Send L2C_Rej for unidentified response PDU
    * WBUSW-1337    Connection Parameter Negotiation Issue
    * WBUSW-1367    BLE5 - Array elements should be in same order as Initiating PHYs bits
    * WBUSW-1380    BLE5 - AdvData should be allowed in extended connectable advertising type
    * WBUSW-1410    HIDApp missing events for generating and storing ECC keys
    * WBUSW-509     Core Specification Supplement (CSS) v6 changes
    * WBUSW-545     Assert if CCC set length is 0
    * WBUSW-1024    Signed Write should not be used when the link is encrypted
    * WBUSW-1076    ESR09&10, E5666 missing error code definition for Table 4.20
    * WBUSW-1104    BLE5 - Add/Handle new device type (0xFF Devices sending anonymous advertisments) in HCI
    * WBUSW-1169    Merge HID Apps
    * WBUSW-1177    Thin-HCI Sample Application Support
    * WBUSW-1198    HCI Handler ID hard coded in WsfMsgEnq
    * WBUSW-1243    HCI fails to free connection structure when connection fails
    * WBUSW-1252    MTU Size limit for indication / notification
    * WBUSW-1281    BLE5 - Host Support for LE Enhanced Privacy 1.2.1
    * WBUSW-1294    Handle OOB Request during pairing
    * WBUSW-1352    BLE5 - Create 2Mbps Sample App
    * WBUSW-1383    BLE5 - Shanghai r10 Stack Updates
    * WBUSW-1395    BLE5 - HCI VS AE modules for generic controllers and single-chip
    * WBUSW-1404    BLE5 - Extended advertising should advertise from beginning of advertising data
    * WBUSW-1413    Privacy mode should be set before advertising or scanning enabled
    * WBUSW-1414    Register UriBeacon utility service to manage CCCDs
    * WBUSW-1426    connSupervisionTimeout must be larger than (1+connSlaveLatency)*connInterval*2


=== Changed in r2p0-00bet0 ===

Released: September 2016

New BLE5 features:

    * LE Advertising Extensions (non-periodic)
    * 2Mbps LE Phy (host) support
    * Host high duty cycle non-connectable advertising

New BLE4.2 features:

    * Sample applications available for Cordio BT4 SDK
    * Cycling Power Profile and Service
    * Cycling Speed and Cadence Profile and Service
    * Running Speed and Cadence Profile and Service
    * Pulse Oximeter Profile and Service
    * Internet Protocol Support Service
    * Scan Parameters Profile (ScPP) and Scan Parameters Service (ScPS)

Bug Fixes and Enhancements

    * WBUSW-708     BLE5 - Host Support for 2 Mbps LE PHY
    * WBUSW-751     Move stack-specific WSF files to stack directories
    * WBUSW-907     Stack should not send another ATT request when the previous ATT timeout
    * WBUSW-964     GLS does not request Security on Glucose Feature, but the profile mandates
    * WBUSW-967     Dats stops working if sending AppBtnUiTest(2) when connected
    * WBUSW-980     Unsafe Pointer Usage with Callback Events
    * WBUSW-981     Add Scan Parameters Profile (ScPP) and Scan Parameters Service (ScPS)
    * WBUSW-986     LESC passkey responder fails with some devices
    * WBUSW-989     Cycling Power Profile and Service
    * WBUSW-1004    LESC OOB doesn't skip the confirm check when the OOB data present flag is FALSE
    * WBUSW-1008    Add New Profiles and Services
    * WBUSW-1042    BLE5 - Host Support for LE Advertising Extensions
    * WBUSW-1101    Signed Writes need port to new CMAC
    * WBUSW-1107    Remove Stack Generic Projects
    * WBUSW-1108    BLE5 - Host Support for High Duty Cycle Non-Connectable Advertising
    * WBUSW-1115    Update documentation to ARM format
    * WBUSW-1121    Add Glucose service to MEDS app
    * WBUSW-1170    ScPP Callback Enhancement
    * WBUSW-1171    BLE5 - Add Thin-HCI Support for Advertising Extensions
    * WBUSW-1172    BLE5 - Add Thin-HCI Support for LE 2Mbps
    * WBUSW-1173    HCI_EVT not properly handling ADV_REPORT_IND
    * WBUSW-1183    Signed Write Not Working with New CMAC

Known Issues

    * WBUSW-1024    Signed Write should not be used when the link is encrypted
    * WBUSW-1193    MBED flash programming fails for large binaries


=== Changed in exactle_01.03.02_r7043 ===

Released: May 9, 2016

New Features:

    * Profile qualification listing update
    * IMPORTANT NOTE:  File hci_defs.h has been moved to a new directory /sw/include.  The include
      path in any of your makefiles or project files will need to be updated.

Bug Fixes and Enhancements:

    #900    Synchronize hci_defs.h with LL
    #921    Clean warnings generated with 'cast-align' & 'switch-default' options
    #948    Add a configuration flag in Medc to indicate whether or not to disconnect upon ATT timeout
    #939    Read Glucose Feature should not required security
    #940    gluc: Glucose sensor application button enhancement
    #938    GLS should return invalid operator (0x03)
    #932    WSS reserved bit is set as default for Weight Scale Feature Characteristic
    #926    Keyboard, Mouse and Remote enfores security to read HID service declaration
    #482    Allow DIS values to be application-configurable
    #797    Replace 'wicentric app' name in stack apps
    #908    LL connection parameter events do not match "thin HCI" fields

Known Issues:

    #907    Stack should not send another ATT request when the previous ATT timeout
    #964    GLS does not request Security on Glucose Feature, but the profile mandates
    #967    'Dats' sample app stops working if sending AppUiBtnTest(2) when connected


=== Changed in exactle_01.03.01_r6636 ===

Release: April 5, 2016

New Features:

    * Host Support for LE Remote Connection Parameter Request
    * LE Ping commands/event
    * 4.2 qualification listing update

Bug Fixes and Enhancements:

    #783    LESC HCI commands/events are little-endian
    #799    Pass command complete for resolving list commands up to app
    #844    LESC Clear RA in OOB should happen based on device's own smp cfg
    #765    DHKey Check Fails when Re-Pairing
    #860    App Slave should check for peer's LL Address Resolution support
    #863    Pass 'Clear Resolving List' Command to DM Privacy
    #858    Add CSRK and Sign Counter Support to App DB

Known Issues:

    #908    LL connection parameter events do not match "thin HCI" fields


=== Changed in exactle_01.03.00_r5781 ===

Released: February 12, 2016

New Features:

    * Support for Bluetooth 4.2 extended length packets.
    * Support for Bluetooth 4.2 enhanced privacy.

Bug Fixes and Enhancements:

    #328    Convert bstream macros to functions to reduce code size
    #341    Mouse Only HID Profile
    #352    update qualification test ATT services
    #395    Revert ATT permission change related to LESC
    #411    DM 4.2 spec updates.
    #469    Dereference of NULL pointer after check in WSF function
    #524    Improve comments in wsf_sec.h
    #536    Host packet size configuration
    #550    L2CAP COC peer MPS not set correctly
    #551    LESC Passkey selected instead of Numeric Comparison
    #552    Privacy 1.1 Passkey Entry selected instead of Passkey Display
    #554    Numeric Verification Value Not Correct
    #559    HCI Support for Enhanced Privacy 1.2
    #574    Host Support for Enhanced Privacy 1.2
    #578    HCI Enhancements
    #617    Encryption setup using SC LTK fails
    #638    Host support for LE Data Packet Length Extensions
    #749    DM Support for LE Set Data Length Command
    #757    Re-pairing fails after reconnection

Known Issues:

    #765    DHKey Check Fails when Re-Pairing with LE secure connections
