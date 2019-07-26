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
# $Id: btle.mk 44576 2019-07-11 21:30:22Z kevin.gillespie $
#
################################################################################

################################################################################
# This file can be included in a project makefile to build the library for the 
# project.
################################################################################

ifeq "$(TARGET)" ""
$(error TARGET must be specified")
endif
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

CORDIO_DIR := $(abspath $(CORDIO_DIR))
CORDIO_PATH = ${CORDIO_DIR}

# WSF (RTOS)
CORDIO_PATH += ${CORDIO_DIR}/wsf
CORDIO_PATH += ${CORDIO_DIR}/wsf/include
CORDIO_PATH += ${CORDIO_DIR}/wsf/include/util
CORDIO_PATH += ${CORDIO_DIR}/wsf/sources
CORDIO_PATH += ${CORDIO_DIR}/wsf/sources/util
CORDIO_PATH += ${CORDIO_DIR}/wsf/sources/port
CORDIO_PATH += ${CORDIO_DIR}/wsf/sources/port/baremetal

# Stack (Host and Application Profile)
CORDIO_PATH += ${CORDIO_DIR}/stack
CORDIO_PATH += ${CORDIO_DIR}/stack/platform
CORDIO_PATH += ${CORDIO_DIR}/stack/platform/${TARGET_LC}
CORDIO_PATH += ${CORDIO_DIR}/stack/platform/include
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/include
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/include/app
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/apps
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/apps/app
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/apps/app/common
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/anpc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/bas
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/blpc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/blps
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/cpp
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/cscp
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/dis
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/fmpl
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/gap
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/gatt
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/glpc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/glps
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/hid
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/hrpc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/hrps
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/htpc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/htps
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/include
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/paspc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/plxpc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/plxps
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/rscp
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/scpps
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/sensor
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/tipc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/udsc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/uribeacon
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/wdxc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/wdxs
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/wpc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/profiles/wspc
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/sources/services
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-profiles/build
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/include
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/sec
# CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/sec/pal
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/sec/common
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/hci
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/hci/dual_chip
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/hci/common
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/stack
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/stack/att
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/stack/l2c
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/stack/hci
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/stack/smp
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/stack/dm
CORDIO_PATH += ${CORDIO_DIR}/stack/ble-host/sources/stack/cfg

# Link Layer
CORDIO_PATH += ${CORDIO_DIR}/link_layer
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/${TARGET_LC}
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/common
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/common/include
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/common/sources
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/common/sources/chci
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/common/sources/bb
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/common/sources/bb/ble
CORDIO_PATH += ${CORDIO_DIR}/link_layer/platform/common/sources/ll
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/include
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/include/common
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/include/ble
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/common
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/common/bb
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/common/sch
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/lhci
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/init
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/bb
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/sch
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/include
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/ll
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/lmgr
CORDIO_PATH += ${CORDIO_DIR}/link_layer/controller/sources/ble/lctr
CORDIO_PATH += ${CORDIO_DIR}/link_layer/thirdparty
CORDIO_PATH += ${CORDIO_DIR}/link_layer/thirdparty/uecc

# Physical Layer
CORDIO_PATH += ${CORDIO_DIR}/phy
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot/shared
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot/ble
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot/ble/pan2g5
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot/ble/pan2g5/include
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot/ble/pan2g5/afe
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot/ble/pan2g5/afe/${TARGET_LC}
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/prot/ble/pan2g5/afe/${TARGET_LC}/include
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/control
CORDIO_PATH += ${CORDIO_DIR}/phy/dbb/control/pan
CORDIO_PATH += ${CORDIO_DIR}/phy/bb
CORDIO_PATH += ${CORDIO_DIR}/phy/debug
CORDIO_PATH += ${CORDIO_DIR}/phy/include
CORDIO_PATH += ${CORDIO_DIR}/phy/llc
CORDIO_PATH += ${CORDIO_DIR}/phy/llc/shared
CORDIO_PATH += ${CORDIO_DIR}/phy/llc/ble

# Add to source directory list
VPATH += ${CORDIO_PATH}

# Add to include directory list
IPATH += ${CORDIO_PATH}

# Add sources
ifneq "$(CORDIO_PATH_FILTER)" ""
BTLE_SRCS := $(notdir $(filter $(CORDIO_PATH_FILTER), $(foreach dir, $(CORDIO_PATH), $(wildcard $(dir)/*.c))))
else
BTLE_SRCS := $(notdir $(foreach dir, $(CORDIO_PATH), $(wildcard $(dir)/*.c)))
endif

FILTERED_SRCS :=
ifeq ($(ENABLE_LL_TESTER),)
FILTERED_SRCS+=lctr_main_tester.c
else
PROJ_CFLAGS+=-DLL_ENABLE_TESTER=TRUE
endif

ifeq ($(ENABLE_SDMA),)
FILTERED_SRCS+=hci_drv_sdma.c
else
PROJ_CFLAGS+=-DENABLE_SDMA=TRUE
PROJ_AFLAGS +=-D__SDMA_SIZE=0x40000
FILTERED_SRCS+=hci_drv.c
endif

SRCS+=$(filter-out $(FILTERED_SRCS),$(BTLE_SRCS))

# Add object files.
LIBS += $(notdir $(wildcard $(CORDIO_DIR)/cordio-phy.a))

# Add definitions
PROJ_CFLAGS+=-DLHCI_ENABLE_VS=TRUE
PROJ_CFLAGS+=-DBB_CLK_RATE_HZ=1600000
PROJ_CFLAGS+=-DLCTR_CONN_NO_TIFS_REASSEMBLY=1
PROJ_CFLAGS+=-DBB_ENABLE_INLINE_ENC_TX=1
PROJ_CFLAGS+=-DBB_ENABLE_INLINE_DEC_RX=1
PROJ_CFLAGS+=-DFORCE_PMU_WAKEUP=1
