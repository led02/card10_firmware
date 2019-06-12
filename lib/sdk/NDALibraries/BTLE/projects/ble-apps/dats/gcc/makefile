###################################################################################################
#
# Makefile for building the dats sample application
#
# Copyright (c) 2015-2017 ARM Ltd. All Rights Reserved.
# ARM Ltd. confidential and proprietary.
#
# IMPORTANT.  Your use of this file is governed by a Software License Agreement
# ("Agreement") that must be accepted in order to download or otherwise receive a
# copy of this file.  You may not use or copy this file for any purpose other than
# as described in the Agreement.  If you do not agree to all of the terms of the
# Agreement do not use this file and delete all copies in your possession or control;
# if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
# to any use, copying or further distribution of this software.
#
###################################################################################################

#--------------------------------------------------------------------------------------------------
#     Project
#--------------------------------------------------------------------------------------------------

# Inputs
ROOT_DIR        := ../../../..
PLATFORM        := bt4
RTOS            := baremetal

# Output
BIN_FILE        := dats.elf

# Options
DEBUG           := 0
TOKEN           := 0
TRACE           := 1
BT_VER          := 9
USE_EXACTLE     := 0
USE_UECC        := 1
WDXS_INCLUDED   := 1
CFG_APP         :=

#--------------------------------------------------------------------------------------------------
#     Configuration
#--------------------------------------------------------------------------------------------------

CFG_DEV         += HCI_ACL_QUEUE_LO=4
CFG_DEV         += INIT_PERIPHERAL INIT_ENCRYPTED
CFG_DEV         += WDXS_INCLUDED=$(WDXS_INCLUDED)
CFG_DEV         += $(CFG_APP)

ifeq ($(BT_VER),8)
CFG_DEV         += CS50_INCLUDED=0
else
CFG_DEV         += CS50_INCLUDED=1
endif

#--------------------------------------------------------------------------------------------------
#     Targets
#--------------------------------------------------------------------------------------------------

include $(ROOT_DIR)/platform/$(PLATFORM)/build/build.mk
include $(ROOT_DIR)/platform/$(PLATFORM)/build/install.mk
