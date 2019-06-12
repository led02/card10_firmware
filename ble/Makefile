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
# $Id: Makefile 38694 2018-10-24 20:41:14Z zach.metzinger $
#
################################################################################

# This is the name of the build output file
ifeq "$(PROJECT)" ""
PROJECT=max32665
endif

# Specify the target processor
ifeq "$(TARGET)" ""
TARGET=MAX32665
endif

# Create Target name variables
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

# Select 'GCC' or 'IAR' compiler
COMPILER=GCC

# Specify the board used
ifeq "$(BOARD)" ""
#BOARD=EvKit_V1
BOARD=card10
endif

# Last octet address modifier
ifneq "$(BLE_ADDR_LSB)" ""
PROJ_CFLAGS+=-DBLE_ADDR_LSB=$(BLE_ADDR_LSB)
endif

## Subsitute WSF critical section entry for default MAXUSB
#PROJ_CFLAGS+=-DMAXUSB_ENTER_CRITICAL=WsfCsEnter
#PROJ_CFLAGS+=-DMAXUSB_EXIT_CRITICAL=WsfCsExit

# This is the path to the CMSIS root directory
ifeq "$(MAXIM_PATH)" ""
LIBS_DIR=../lib/sdk/Libraries
NDALIBS_DIR=../lib/sdk/NDALibraries
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
NDALIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/NDALibraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
SRCS  = main.c
SRCS += stack_fit.c
SRCS += fit_main.c
SRCS += svc_dis.c

# Where to find source files for this test
VPATH  = .

# Where to find header files for this test
IPATH  = .

# Enable assertion checking for development
PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE

# Specify the target revision to override default
# "A2" in ASCII
# TARGET_REV=0x4132

# Use this variables to specify and alternate tool path
#TOOL_DIR=/opt/gcc-arm-none-eabi-4_8-2013q4/bin

# Use these variables to add project specific tool options
#PROJ_CFLAGS+=--specs=nano.specs
#PROJ_LDFLAGS+=--specs=nano.specs

PROJ_CFLAGS+=-std=c99

# Point this variable to a startup file to override the default file
#STARTUPFILE=start.S

# Override the default optimization level using this variable
#MXC_OPTIMIZE_CFLAGS=-O1

# Point this variable to a linker file to override the default file
ifneq "$(APP)" ""
LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC)_app.ld
endif

################################################################################
# Include external library makefiles here

# Include the BSP
BOARD_DIR=$(LIBS_DIR)/Boards/$(BOARD)
include $(BOARD_DIR)/board.mk

# Include the Peripheral Driver Library
PERIPH_DRIVER_DIR=$(LIBS_DIR)/$(TARGET)PeriphDriver
include ${PERIPH_DRIVER_DIR}/periphdriver.mk

# Include Cordio BTLE Library
TRACE=1
CORDIO_DIR=$(NDALIBS_DIR)/BTLE
include ${CORDIO_DIR}/btle.mk

# Include MAXUSB library
MAXUSB_DIR=$(LIBS_DIR)/MAXUSB
include $(MAXUSB_DIR)/maxusb.mk

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk
