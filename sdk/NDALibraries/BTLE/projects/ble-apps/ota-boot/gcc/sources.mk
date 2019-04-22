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

INC_DIRS += \
	$(ROOT_DIR)/platform/$(PLATFORM)/include \
	$(ROOT_DIR)/platform/$(PLATFORM)/sources/ota \
	$(ROOT_DIR)/ble-profiles/include/app \
	$(ROOT_DIR)/ble-profiles/sources/apps


C_FILES += \
	$(ROOT_DIR)/projects/ble-apps/ota-boot/main.c \
	$(ROOT_DIR)/platform/$(PLATFORM)/sources/ota/ota_common.c

#--------------------------------------------------------------------------------------------------
# 	Platform
#--------------------------------------------------------------------------------------------------

include $(ROOT_DIR)/wsf/$(RTOS)/build/sources.mk
include $(ROOT_DIR)/platform/$(PLATFORM)/build/sources_ota.mk

