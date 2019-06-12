################################################################################
# Copyright (C) 2014 Maxim Integrated Products, Inc., All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# Except as contained in this notice, the name of Maxim Integrated
# Products, Inc. shall not be used except as stated in the Maxim Integrated
# Products, Inc. Branding Policy.
#
# The mere transfer of this software does not imply any licenses
# of trade secrets, proprietary technology, copyrights, patents,
# trademarks, maskwork rights, or any other form of intellectual
# property whatsoever. Maxim Integrated Products, Inc. retains all
# ownership rights.
#
# $Id: btle.mk 36944 2018-08-13 23:37:01Z zach.metzinger $
#
################################################################################

################################################################################
# This file can be included in a project makefile to add BLE libraries
################################################################################

export CORDIO_DIR := ${abspath ${CORDIO_DIR}}

# Add to libraries list
LIBS += $(CORDIO_DIR)/lib/GCC/cordiolib.a
LIBS += $(CORDIO_DIR)/lib/GCC/libmaclib.a

# Add to include directory list
IPATH += ${CORDIO_DIR}/ble-host/include
IPATH += ${CORDIO_DIR}/ble-host/sources/hci/exactle
IPATH += ${CORDIO_DIR}/ble-host/sources/hci/max32565
IPATH += ${CORDIO_DIR}/ble-host/sources/sec/common
IPATH += ${CORDIO_DIR}/ble-host/sources/stack/att
IPATH += ${CORDIO_DIR}/ble-host/sources/stack/cfg
IPATH += ${CORDIO_DIR}/ble-host/sources/stack/dm
IPATH += ${CORDIO_DIR}/ble-host/sources/stack/hci
IPATH += ${CORDIO_DIR}/ble-host/sources/stack/l2c
IPATH += ${CORDIO_DIR}/ble-host/sources/stack/smp
IPATH += ${CORDIO_DIR}/ble-profiles/include/app
IPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/
IPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/app
IPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/
IPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/include
IPATH += ${CORDIO_DIR}/ble-profiles/sources/services
IPATH += ${CORDIO_DIR}/controller/sources/config
IPATH += ${CORDIO_DIR}/controller/sources/config/max32565
IPATH += ${CORDIO_DIR}/controller/sources/config/max32665
IPATH += ${CORDIO_DIR}/controller/sources/config/me05
IPATH += ${CORDIO_DIR}/controller/sources/dbb/control/pan
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/afe
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/afe/max32565
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/afe/max32565/include
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/afe/me99
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/afe/me99/include
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/include
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe/max32665
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe/max32665/include
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe/me99
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe/me99/include
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/include
IPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/shared
IPATH += ${CORDIO_DIR}/controller/sources/debug
IPATH += ${CORDIO_DIR}/controller/sources/llc/ble
IPATH += ${CORDIO_DIR}/controller/sources/llc/shared
IPATH += ${CORDIO_DIR}/controller/sources/mac/bb/imec
IPATH += ${CORDIO_DIR}/controller/sources/mac/ble/include
IPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lctr
IPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lhci
IPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lhci/include
IPATH += ${CORDIO_DIR}/controller/sources/mac/ble/ll
IPATH += ${CORDIO_DIR}/controller/sources/mac/cfg
IPATH += ${CORDIO_DIR}/controller/sources/mac/include
IPATH += ${CORDIO_DIR}/controller/sources/mac/sch
IPATH += ${CORDIO_DIR}/platform/bt4/img
IPATH += ${CORDIO_DIR}/platform/bt4/include
IPATH += ${CORDIO_DIR}/platform/bt4/sources/app
IPATH += ${CORDIO_DIR}/platform/bt4/sources/board/CMSIS/Device/SMD/TC/Include
IPATH += ${CORDIO_DIR}/platform/bt4/sources/board/CMSIS/Include
IPATH += ${CORDIO_DIR}/platform/bt4/sources/hci
IPATH += ${CORDIO_DIR}/platform/bt4/sources/llc/include
IPATH += ${CORDIO_DIR}/platform/bt4/sources/ota
IPATH += ${CORDIO_DIR}/platform/bt4/tools/fwupdater
IPATH += ${CORDIO_DIR}/platform/common/include
IPATH += ${CORDIO_DIR}/projects/ble-apps/ota-boot
IPATH += ${CORDIO_DIR}/uECC
IPATH += ${CORDIO_DIR}/wsf/baremetal/sources
IPATH += ${CORDIO_DIR}/wsf/common/include
IPATH += ${CORDIO_DIR}/wsf/common/include/util

# Add to source directory list
VPATH += ${CORDIO_DIR}/ble-host/sources/hci/common
VPATH += ${CORDIO_DIR}/ble-host/sources/hci/exactle
VPATH += ${CORDIO_DIR}/ble-host/sources/hci/max32565
VPATH += ${CORDIO_DIR}/ble-host/sources/sec/common
VPATH += ${CORDIO_DIR}/ble-host/sources/sec/uecc
VPATH += ${CORDIO_DIR}/ble-host/sources/stack/att
VPATH += ${CORDIO_DIR}/ble-host/sources/stack/cfg
VPATH += ${CORDIO_DIR}/ble-host/sources/stack/dm
VPATH += ${CORDIO_DIR}/ble-host/sources/stack/hci
VPATH += ${CORDIO_DIR}/ble-host/sources/stack/l2c
VPATH += ${CORDIO_DIR}/ble-host/sources/stack/smp
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/app
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/app/common
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/cycling
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/datc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/dats
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/fit
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/gluc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/hidapp
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/medc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/meds
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/sensor
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/tag
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/uribeacon
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/watch
VPATH += ${CORDIO_DIR}/ble-profiles/sources/apps/wdxs
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/anpc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/bas
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/blpc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/blps
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/cpp
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/cscp
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/dis
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/fmpl
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/gap
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/gatt
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/glpc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/glps
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/hid
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/hrpc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/hrps
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/htpc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/htps
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/paspc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/plxpc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/plxps
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/rscp
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/scpps
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/sensor
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/tipc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/udsc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/uribeacon
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/wdxc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/wdxs
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/wpc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/wspc
VPATH += ${CORDIO_DIR}/ble-profiles/sources/profiles/wsps
VPATH += ${CORDIO_DIR}/ble-profiles/sources/services
VPATH += ${CORDIO_DIR}/controller/sources/dbb/control/pan
VPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g
VPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/afe/max32565
VPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g/afe/me99
VPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5
VPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe
VPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe/max32665
VPATH += ${CORDIO_DIR}/controller/sources/dbb/prot/ble/pan2g5/afe/me99
VPATH += ${CORDIO_DIR}/controller/sources/debug
VPATH += ${CORDIO_DIR}/controller/sources/llc/ble
VPATH += ${CORDIO_DIR}/controller/sources/llc/shared
VPATH += ${CORDIO_DIR}/controller/sources/mac/bb/imec
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lctr
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lhci
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lhci/generic
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lhci/imec
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lhci/max32565
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lhci/max32665
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/ll
VPATH += ${CORDIO_DIR}/controller/sources/mac/ble/lmgr
VPATH += ${CORDIO_DIR}/controller/sources/mac/sch
VPATH += ${CORDIO_DIR}/platform/bt4/sources/app
VPATH += ${CORDIO_DIR}/platform/bt4/sources/board
VPATH += ${CORDIO_DIR}/platform/bt4/sources/board/CMSIS/Device/SMD/TC/Source
VPATH += ${CORDIO_DIR}/platform/bt4/sources/board/CMSIS/Device/SMD/TC/Source/GCC
VPATH += ${CORDIO_DIR}/platform/bt4/sources/hci
VPATH += ${CORDIO_DIR}/platform/bt4/sources/llc/cordio-tc2
VPATH += ${CORDIO_DIR}/platform/bt4/sources/llc/generic
VPATH += ${CORDIO_DIR}/platform/bt4/sources/ota
VPATH += ${CORDIO_DIR}/platform/bt4/tools/fwupdater
VPATH += ${CORDIO_DIR}/platform/common/sources/bb/ble
VPATH += ${CORDIO_DIR}/platform/common/sources/chci
VPATH += ${CORDIO_DIR}/platform/common/sources/ll
VPATH += ${CORDIO_DIR}/platform/common/sources/prand
VPATH += ${CORDIO_DIR}/platform/max32565/sources
VPATH += ${CORDIO_DIR}/platform/max32665/sources
VPATH += ${CORDIO_DIR}/projects/ble-apps/cycling
VPATH += ${CORDIO_DIR}/projects/ble-apps/datc
VPATH += ${CORDIO_DIR}/projects/ble-apps/dats
VPATH += ${CORDIO_DIR}/projects/ble-apps/fit
VPATH += ${CORDIO_DIR}/projects/ble-apps/hidapp
VPATH += ${CORDIO_DIR}/projects/ble-apps/medc
VPATH += ${CORDIO_DIR}/projects/ble-apps/meds
VPATH += ${CORDIO_DIR}/projects/ble-apps/ota-boot
VPATH += ${CORDIO_DIR}/projects/ble-apps/tag
VPATH += ${CORDIO_DIR}/projects/ble-apps/uribeacon
VPATH += ${CORDIO_DIR}/projects/ble-apps/watch
VPATH += ${CORDIO_DIR}/uECC
VPATH += ${CORDIO_DIR}/wsf/baremetal/sources
VPATH += ${CORDIO_DIR}/wsf/common/sources/util

SRCS += $(sort $(wildcard ${CORDIO_DIR}/ble-host/sources/hci/exactle/*.c))
IPATH += ${CORDIO_DIR}/ble-host/sources/stack/hci
VPATH += ${CORDIO_DIR}/ble-host/sources/hci/exactle
