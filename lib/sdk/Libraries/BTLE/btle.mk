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
# $Id: btle.mk 45434 2019-08-14 18:28:59Z kevin.gillespie $
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

ROOT_DIR            := $(abspath $(CORDIO_DIR))
CORDIO_PATH          = ${ROOT_DIR}

LINKLAYER_DIR       := ${ROOT_DIR}/link_layer
LINKLAYER_BUILD_DIR := ${LINKLAYER_DIR}/build
LINKLAYER_LIB_DIR   := ${LINKLAYER_BUILD_DIR}/library
LINKLAYER_LIB       := ${LINKLAYER_LIB_DIR}/liblinklayer.a

STACK_DIR           := ${ROOT_DIR}/stack
STACK_BUILD_DIR     := ${STACK_DIR}/build
STACK_LIB_DIR       := ${STACK_BUILD_DIR}/library
STACK_LIB           := ${STACK_LIB_DIR}/libstack.a

WSF_DIR             := ${ROOT_DIR}/wsf
WSF_BUILD_DIR       := ${WSF_DIR}/build
WSF_LIB_DIR         := ${WSF_BUILD_DIR}/library
WSF_LIB             := ${WSF_LIB_DIR}/libwsf.a

# Add to libraries list
LIBS += $(LINKLAYER_LIB)
LIBS += $(STACK_LIB)
LIBS += $(WSF_LIB)
LIBS += $(notdir $(wildcard $(ROOT_DIR)/cordio-phy.a))

# Physical Layer
CORDIO_PATH += ${ROOT_DIR}/phy
CORDIO_PATH += ${ROOT_DIR}/phy/dbb
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot/shared
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot/ble
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot/ble/pan2g5
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot/ble/pan2g5/include
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot/ble/pan2g5/afe
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot/ble/pan2g5/afe/${TARGET_LC}
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/prot/ble/pan2g5/afe/${TARGET_LC}/include
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/control
CORDIO_PATH += ${ROOT_DIR}/phy/dbb/control/pan
CORDIO_PATH += ${ROOT_DIR}/phy/bb
CORDIO_PATH += ${ROOT_DIR}/phy/debug
CORDIO_PATH += ${ROOT_DIR}/phy/include
CORDIO_PATH += ${ROOT_DIR}/phy/llc
CORDIO_PATH += ${ROOT_DIR}/phy/llc/shared
CORDIO_PATH += ${ROOT_DIR}/phy/llc/ble

# Add to source directory list
VPATH += ${CORDIO_PATH}

# Add to include directory list
IPATH += ${CORDIO_PATH}

# Add Link Layer include paths
IPATH += ${ROOT_DIR}/link_layer/platform/common/include
IPATH += ${ROOT_DIR}/link_layer/controller/include/ble
IPATH += ${ROOT_DIR}/link_layer/controller/include/common
IPATH += ${ROOT_DIR}/link_layer/controller/sources/ble/include

# Add Stack include paths
IPATH += ${ROOT_DIR}/stack/platform/$(TARGET_LC)
IPATH += ${ROOT_DIR}/stack/ble-host/sources/stack/cfg
IPATH += ${ROOT_DIR}/stack/ble-host/include
IPATH += ${ROOT_DIR}/stack/ble-host/sources/stack/hci
IPATH += ${ROOT_DIR}/stack/ble-host/sources/hci/dual_chip
IPATH += ${ROOT_DIR}/stack/ble-profiles/include/app
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/app
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/cycling
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/datc
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/dats
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/fit
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/gluc
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/hidapp
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/medc
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/meds
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/sensor
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/tag
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/uribeacon
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/watch
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/apps/wdxs
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/services
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/profiles
IPATH += ${ROOT_DIR}/stack/ble-profiles/sources/profiles/include

# Add WSF include paths
IPATH += ${ROOT_DIR}/wsf/include
IPATH += ${ROOT_DIR}/wsf/include/util

# Add sources
ifneq "$(CORDIO_PATH_FILTER)" ""
BTLE_SRCS := $(notdir $(filter $(CORDIO_PATH_FILTER), $(foreach dir, $(CORDIO_PATH), $(wildcard $(dir)/*.c))))
else
BTLE_SRCS := $(notdir $(foreach dir, $(CORDIO_PATH), $(wildcard $(dir)/*.c)))
endif

# Add configurable sources
BTLE_SRCS += hci_drv_sdma.c
BTLE_SRCS += hci_drv.c
BTLE_SRCS += hci_vs.c
BTLE_SRCS += hci_tr.c
BTLE_SRCS += lctr_main_tester.c
BTLE_SRCS += init_ctr.c
BTLE_SRCS += init_ctr_ext.c
BTLE_SRCS += init_ext.c
BTLE_SRCS += init.c

VPATH     += ${ROOT_DIR}/stack/platform/${TARGET_LC}
VPATH     += ${ROOT_DIR}/link_layer/controller/sources/ble/lctr
VPATH     += ${ROOT_DIR}/link_layer/controller/sources/ble/init

# Filter sources based on configuration
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
FILTERED_SRCS+=hci_drv.c

ifeq ($(SDMA_ADV),)
ifeq ($(SDMA_SCN),)
PROJ_AFLAGS +=-D__SDMA_SIZE=0x2E000
endif
endif

ifneq ($(SDMA_ADV),)
PROJ_CFLAGS+=-DSDMA_ADV=TRUE
PROJ_AFLAGS +=-D__SDMA_SIZE=0x20800 # Adv/Slave only
endif

ifneq ($(SDMA_SCN),)
PROJ_CFLAGS+=-DSDMA_SCN=TRUE
PROJ_AFLAGS +=-D__SDMA_SIZE=0x27800 # Scan/Master only
endif
endif

SRCS+=$(filter-out $(FILTERED_SRCS),$(BTLE_SRCS))

# Add definitions
PROJ_CFLAGS+=-DLHCI_ENABLE_VS=TRUE
PROJ_CFLAGS+=-DBB_CLK_RATE_HZ=1600000
PROJ_CFLAGS+=-DLCTR_CONN_NO_TIFS_REASSEMBLY=1
PROJ_CFLAGS+=-DBB_ENABLE_INLINE_ENC_TX=1
PROJ_CFLAGS+=-DBB_ENABLE_INLINE_DEC_RX=1
PROJ_CFLAGS+=-DFORCE_PMU_WAKEUP=1
PROJ_CFLAGS+=-DWSF_TRACE_ENABLED=1

# Build libraries
export ROOT_DIR
export PROJ_CFLAGS
export IPATH

include ${LINKLAYER_BUILD_DIR}/sources_ll.mk
${LINKLAYER_LIB}: ${LINKLAYER_C_FILES} ${LINKLAYER_H_FILES}
	$(MAKE) -C ${LINKLAYER_BUILD_DIR} lib

include ${STACK_BUILD_DIR}/sources_stack.mk
${STACK_LIB}: ${STACK_C_FILES} ${STACK_H_FILES}
	$(MAKE) -C ${STACK_BUILD_DIR} lib

include ${WSF_BUILD_DIR}/sources_wsf.mk
${WSF_LIB}: ${WSF_C_FILES} ${WSF_H_FILES}
	$(MAKE) -C ${WSF_BUILD_DIR} lib

clean.stack:
	@rm -rf $(STACK_LIB_DIR)/*

clean.linklayer:
	@rm -rf $(LINKLAYER_LIB_DIR)/*

clean.wsf:
	@rm -rf $(WSF_LIB_DIR)/*

clean.ble: clean.stack clean.linklayer clean.wsf
