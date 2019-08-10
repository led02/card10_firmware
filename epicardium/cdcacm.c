/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Id: main.c 32120 2017-11-28 23:51:11Z lorne.smith $
 *
 *******************************************************************************
 */

/**
 * @file    main.c
 * @brief   USB CDC-ACM example
 * @details This project creates a virtual COM port, which loops back data sent to it.
 *          Load the project, connect a cable from the PC to the USB connector
 *          on the Evaluation Kit, and observe that the PC now recognizes a new COM port.
 *          A driver for the COM port, if needed, is located in the Driver/ subdirectory.
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "board.h"
#include "led.h"
#include "usb.h"
#include "usb_event.h"
#include "enumerate.h"
#include "cdc_acm.h"
#include "descriptors.h"

#include "modules/log.h"
#include <errno.h>

/***** Definitions *****/
#define EVENT_ENUM_COMP MAXUSB_NUM_EVENTS

#define BUFFER_SIZE 64

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/***** Global Data *****/
//SWYM: rename to CDC_xy or put into struct CDC_state
volatile int configured; //SWYM: actually unused...
volatile int suspended;
volatile unsigned int event_flags;
int remote_wake_en;

/***** Function Prototypes *****/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata);
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int event_callback(maxusb_event_t evt, void *data);
static void usb_app_sleep(void);
static void usb_app_wakeup(void);
static int usb_read_callback(void);
//static void echo_usb(void);

/***** File Scope Variables *****/

/* This EP assignment must match the Configuration Descriptor */
static const acm_cfg_t acm_cfg = {
	1,                    /* EP OUT */
	MXC_USBHS_MAX_PACKET, /* OUT max packet size */
	2,                    /* EP IN */
	MXC_USBHS_MAX_PACKET, /* IN max packet size */
	3,                    /* EP Notify */
	MXC_USBHS_MAX_PACKET, /* Notify max packet size */
};

static volatile int usb_read_complete;

int usb_startup_cb()
{
	const sys_cfg_usbhs_t sys_usbhs_cfg = NULL;
	return SYS_USBHS_Init(&sys_usbhs_cfg);
}

int usb_shutdown_cb()
{
	return SYS_USBHS_Shutdown();
}

/* User-supplied function to delay usec micro-seconds */
void delay_us(unsigned int usec)
{
	/* mxc_delay() takes unsigned long, so can't use it directly */
	mxc_delay(usec);
}

/******************************************************************************/
int cdcacm_init(void)
{
	maxusb_cfg_options_t usb_opts;

	/* Initialize state */
	configured     = 0;
	suspended      = 0;
	event_flags    = 0;
	remote_wake_en = 0;

	/* Start out in full speed */
	usb_opts.enable_hs = 0;
	usb_opts.delay_us =
		delay_us; /* Function which will be used for delays */
	usb_opts.init_callback     = usb_startup_cb;
	usb_opts.shutdown_callback = usb_shutdown_cb;

	/* Initialize the usb module */
	if (usb_init(&usb_opts) != 0) {
		LOG_ERR("cdcacm", "usb_init() failed");
		return -EIO;
	}

	/* Initialize the enumeration module */
	if (enum_init() != 0) {
		LOG_ERR("cdcacm", "enum_init() failed");
		return -EIO;
	}

	/* Register enumeration data */
	enum_register_descriptor(
		ENUM_DESC_DEVICE, (uint8_t *)&device_descriptor, 0
	);
	enum_register_descriptor(
		ENUM_DESC_CONFIG, (uint8_t *)&config_descriptor, 0
	);
	enum_register_descriptor(ENUM_DESC_STRING, lang_id_desc, 0);
	enum_register_descriptor(ENUM_DESC_STRING, mfg_id_desc, 1);
	enum_register_descriptor(ENUM_DESC_STRING, prod_id_desc, 2);

	/* Handle configuration */
	enum_register_callback(ENUM_SETCONFIG, setconfig_callback, NULL);

	/* Handle feature set/clear */
	enum_register_callback(ENUM_SETFEATURE, setfeature_callback, NULL);
	enum_register_callback(ENUM_CLRFEATURE, clrfeature_callback, NULL);

	/* Initialize the class driver */
	if (acm_init(&config_descriptor.comm_interface_descriptor) != 0) {
		LOG_ERR("cdcacm", "acm_init() failed");
		return -EIO;
	}

	/* Register callbacks */
	usb_event_enable(MAXUSB_EVENT_NOVBUS, event_callback, NULL);
	usb_event_enable(MAXUSB_EVENT_VBUS, event_callback, NULL);
	acm_register_callback(ACM_CB_READ_READY, usb_read_callback);
	usb_read_complete = 0;

	/* Start with USB in low power mode */
	usb_app_sleep();
	/* TODO: Fix priority */
	NVIC_SetPriority(USB_IRQn, 6);
	NVIC_EnableIRQ(USB_IRQn);

	return 0;
}

int cdcacm_num_read_avail(void)
{
	return acm_canread();
}

uint8_t cdcacm_read(void)
{
	while (acm_canread() <= 0) {
	}

	uint8_t buf;
	acm_read(&buf, 1);
	return buf;
}

void cdcacm_write(uint8_t *data, int len)
{
	static int lockup_disable = 0;
	if (acm_present() && !lockup_disable) {
		int ret = acm_write(data, len);
		if (ret < 0) {
			LOG_ERR("cdcacm", "fifo lockup detected");
			lockup_disable = 1;
		} else if (ret != len) {
			LOG_WARN(
				"cdcacm", "write length mismatch, got %d", ret
			);
		}
	}
}

/******************************************************************************/
#if 0
static void echo_usb(void)
{
  int chars;
  uint8_t buffer[BUFFER_SIZE];
  
  if ((chars = acm_canread()) > 0) {

    if (chars > BUFFER_SIZE) {
      chars = BUFFER_SIZE;
    }

    // Read the data from USB
    if (acm_read(buffer, chars) != chars) {
      printf("acm_read() failed\n");
      return;
    }

    // Echo it back
    if (acm_present()) {
      if (acm_write(buffer, chars) != chars) {
        printf("acm_write() failed\n");
      }
    }
  }
}
#endif
/******************************************************************************/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata)
{
	/* Confirm the configuration value */
	if (sud->wValue ==
	    config_descriptor.config_descriptor.bConfigurationValue) {
		configured = 1;
		MXC_SETBIT(&event_flags, EVENT_ENUM_COMP);
		return acm_configure(&acm_cfg); /* Configure the device class */
	} else if (sud->wValue == 0) {
		configured = 0;
		return acm_deconfigure();
	}

	return -1;
}

/******************************************************************************/
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
	if (sud->wValue == FEAT_REMOTE_WAKE) {
		remote_wake_en = 1;
	} else {
		// Unknown callback
		return -1;
	}

	return 0;
}

/******************************************************************************/
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
	if (sud->wValue == FEAT_REMOTE_WAKE) {
		remote_wake_en = 0;
	} else {
		// Unknown callback
		return -1;
	}

	return 0;
}

/******************************************************************************/
static void usb_app_sleep(void)
{
	/* TODO: Place low-power code here */
	suspended = 1;
}

/******************************************************************************/
static void usb_app_wakeup(void)
{
	/* TODO: Place low-power code here */
	suspended = 0;
}

/******************************************************************************/
static int event_callback(maxusb_event_t evt, void *data)
{
	/* Set event flag */
	MXC_SETBIT(&event_flags, evt);

	switch (evt) {
	case MAXUSB_EVENT_NOVBUS:
		usb_event_disable(MAXUSB_EVENT_BRST);
		usb_event_disable(MAXUSB_EVENT_SUSP);
		usb_event_disable(MAXUSB_EVENT_DPACT);
		usb_disconnect();
		configured = 0;
		enum_clearconfig();
		acm_deconfigure();
		usb_app_sleep();
		break;
	case MAXUSB_EVENT_VBUS:
		usb_event_clear(MAXUSB_EVENT_BRST);
		usb_event_enable(MAXUSB_EVENT_BRST, event_callback, NULL);
		usb_event_clear(MAXUSB_EVENT_SUSP);
		usb_event_enable(MAXUSB_EVENT_SUSP, event_callback, NULL);
		usb_connect();
		usb_app_sleep();
		break;
	case MAXUSB_EVENT_BRST:
		usb_app_wakeup();
		enum_clearconfig();
		acm_deconfigure();
		configured = 0;
		suspended  = 0;
		break;
	case MAXUSB_EVENT_SUSP:
		usb_app_sleep();
		break;
	case MAXUSB_EVENT_DPACT:
		usb_app_wakeup();
		break;
	default:
		break;
	}

	return 0;
}

/******************************************************************************/
static int usb_read_callback(void)
{
	usb_read_complete = 1;
	return 0;
}

/******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t serial_task_id;
void USB_IRQHandler(void)
{
	usb_event_handler();

	if (serial_task_id != NULL) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(
			serial_task_id, &xHigherPriorityTaskWoken
		);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

/******************************************************************************/
/* TODO: We probably need to fix something related to this */
#if 0
void SysTick_Handler(void)
{
    mxc_delay_handler();
}
#endif /* 0 */
