###################################################################################################
#
# Source and include definition
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
# 	Application
#--------------------------------------------------------------------------------------------------

ifeq ($(BT_VER),8)
APP_FILTER_OUT := \
	$(wildcard $(ROOT_DIR)/ble-profiles/sources/apps/app/*ae*.c)
else
APP_FILTER_OUT :=
endif

INC_DIRS += \
	$(ROOT_DIR)/ble-profiles/include/app \
	$(ROOT_DIR)/ble-profiles/sources/apps \
	$(ROOT_DIR)/ble-profiles/sources/apps/app

C_FILES += \
	$(sort $(filter-out $(APP_FILTER_OUT),$(wildcard $(ROOT_DIR)/ble-profiles/sources/apps/app/*.c))) \
	$(sort $(wildcard $(ROOT_DIR)/ble-profiles/sources/apps/app/common/*.c)) \
	$(ROOT_DIR)/ble-profiles/sources/apps/hidapp/hidapp_main.c \
	$(ROOT_DIR)/projects/ble-apps/hidapp/main.c \
	$(ROOT_DIR)/projects/ble-apps/hidapp/stack_hidapp.c

#--------------------------------------------------------------------------------------------------
# 	Host
#--------------------------------------------------------------------------------------------------

include $(ROOT_DIR)/ble-profiles/build/sources_services.mk

ifeq ($(BT_VER),9)
include $(ROOT_DIR)/ble-profiles/build/sources_profiles_5.mk
else
include $(ROOT_DIR)/ble-profiles/build/sources_profiles_4.mk
endif

ifeq ($(BT_VER),9)
include $(ROOT_DIR)/ble-host/build/sources_stack_5.mk
else
include $(ROOT_DIR)/ble-host/build/sources_stack_4.mk
endif

ifeq ($(USE_EXACTLE),1)
include $(ROOT_DIR)/ble-host/build/sources_hci_exactle.mk
else
include $(ROOT_DIR)/ble-host/build/sources_hci_dual_chip.mk
endif

#--------------------------------------------------------------------------------------------------
# 	Controller
#--------------------------------------------------------------------------------------------------

ifeq ($(USE_EXACTLE),1)
include $(ROOT_DIR)/controller/build/sources_ll_5.mk
endif

#--------------------------------------------------------------------------------------------------
# 	Platform
#--------------------------------------------------------------------------------------------------

include $(ROOT_DIR)/wsf/$(RTOS)/build/sources.mk
include $(ROOT_DIR)/platform/$(PLATFORM)/build/sources_common.mk
include $(ROOT_DIR)/platform/$(PLATFORM)/build/sources_ble-host.mk

ifeq ($(USE_EXACTLE),1)
include $(ROOT_DIR)/platform/$(PLATFORM)/build/sources_ll.mk
endif
