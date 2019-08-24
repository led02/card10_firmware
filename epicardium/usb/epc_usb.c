/**
 * 
 * 
 * Core of the USB implementation:
 * 
 * - device independent
 * - handles setup of MAXUSB stack
 * - handles events and state of the MAXUSB stack
 * 
 * Also contains definitions of device-independent String descriptors
 * 
 * 
 * 
 * 
 * 
 * swym's USB ramblings:
 * 
 * setting up USB basically consists of two parts:
 * 
 * - set up descriptors
 * - initialize the corresponding MAXUSB stacks - acm, msc...
 * 
 * at the moment, the descriptors are statically configured in
 * descriptors.h several descriptors point to enumeration descriptors
 * via iFooBar indices. Those enumeration descriptors are registered
 * via enum_register_descriptor(type, desc, idx), where the idx passed
 * corresponds to the iFooBar index mentioned above.
 * 
 * There are several callbacks, some of which do not perform anything meaningful
 * at the moment. For example usb_write_callback sets a global flag that is never
 * read from. These will be removed later on.
 * 
 * The  initialization routines of acm & msc refer to the descriptors from which,
 * among other things, they get their enpoint IDs. These are hard-coded right now
 * but it would make sense to allocate these IDs at runtime. There is, AFAICT, no
 * reason for these endpoint IDs to be 3, 1, 2 in the CDCACM case for example.
 * 
 * Allocating those at runtime would also make the setup less rigid: we can have all
 * combinations of {cdcacm, storage} interfaces without the awkward descriptor hackery
 * in epc_usb_init.
 * 
 * To generalize even further, the descriptors could be malloced. The total length of
 * the descriptor structure can easily be derived from the number of interfaces to be
 * initialized. The memory allocated will then be split up (with alignment of the single
 * descriptor structs taken care of) in a simple bump-allocator strategy on demand.
 * 
 * 
 */
#include "usb/epc_usb.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_delay.h"

// MAXUSB includes
#include "usb.h"
#include "usb_event.h"
#include "enumerate.h"
#include "msc.h"
#include "cdc_acm.h"

#include "usb/descriptors.h"
#include "modules/log.h"
#include "modules/filesystem.h"

//#define USE_REMOTE_WAKE_ENABLE

/***** Function Prototypes *****/
static int cb_usb_setconfig(usb_setup_pkt *sud, void *cbdata);
#ifdef USE_REMOTE_WAKE_ENABLE
static int cb_usb_setfeature(usb_setup_pkt *sud, void *cbdata);
static int cb_usb_clrfeature(usb_setup_pkt *sud, void *cbdata);
#endif
static int cb_usb_event(maxusb_event_t evt, void *data);
static int cb_usb_shutdown();
static int cb_usb_init();
static void usb_app_sleep(void);
static void usb_app_wakeup(void);
/* needed for usb_opts. mxc_delay() takes unsigned long, so can't use it directly */
static void delay_us(unsigned int usec);
static void device_deinit();
static bool device_deconfigure();
static int device_configure();

volatile int suspended;
#ifdef USE_REMOTE_WAKE_ENABLE
int remote_wake_en; //SWYM: unused, only written to!
#endif

static StaticTimer_t x;
static TimerHandle_t timerWakeup = NULL;
static void cb_timerReset(TimerHandle_t t);

static struct esb_config s_device = { 0 };

__attribute__((aligned(4))) uint8_t lang_id_desc[] = {
	0x04, /* bLength */
	0x03, /* bDescriptorType */
	0x09,
	0x04 /* bString = wLANGID (see usb_20.pdf 9.6.7 String) */
};

__attribute__((aligned(4))) uint8_t mfg_id_desc[] = {
	0x22, /* bLength */
	0x03, /* bDescriptorType */
	'M',  0, 'a', 0, 'x', 0, 'i', 0, 'm', 0, ' ', 0, 'I', 0, 'n', 0,
	't',  0, 'e', 0, 'g', 0, 'r', 0, 'a', 0, 't', 0, 'e', 0, 'd', 0,
};

__attribute__((aligned(4))) uint8_t prod_id_desc[] = {
	0x24, /* bLength */
	0x03, /* bDescriptorType */
	'C',  0, 'A', 0, 'R', 0, 'D', 0, '1', 0, '0', 0, ' ', 0, 'U', 0, 'S', 0,
	'B',  0, ' ', 0, 'G', 0, 'A', 0, 'D', 0, 'G', 0, 'E', 0, 'T', 0,
};

/* Not currently used (see device descriptor), but could be enabled if desired */
__attribute__((aligned(4)))
uint8_t serial_id_desc[] = { 0x14, /* bLength */
			     0x03, /* bDescriptorType */
			     'c',  0,   'a', 0,   'a', 0,   'a', 0,   'a',
			     0,    'd', 0,   '1', 0,   '0', 0,   '0', 0 };

int cb_usb_init()
{
	const sys_cfg_usbhs_t sys_usbhs_cfg = NULL;
	LOG_DEBUG("usb", "init");
	return SYS_USBHS_Init(&sys_usbhs_cfg);
}

volatile bool shutDownCompleted;
void do_usb_shutdown()
{
	LOG_DEBUG("usb", "shutting down...");
	shutDownCompleted = false;
	usb_shutdown();
	for (int i = 0; i < 10000 && !shutDownCompleted; ++i) {
	}
}
int cb_usb_shutdown()
{
	SYS_USBHS_Shutdown();
	SYS_Reset_Periph(SYS_RESET_USB);
	device_deinit();
	LOG_DEBUG("usb", "shutdown complete");
	shutDownCompleted = true;
	return 0;
}

/* User-supplied function to delay usec micro-seconds */
static void delay_us(unsigned int usec)
{
	/* mxc_delay() takes unsigned long, so can't use it directly */
	mxc_delay(usec);
}

static void device_deinit()
{
	esb_cfg_handler deinit = s_device.deinit;
	s_device.deinit        = NULL;
	s_device.configure     = NULL;
	s_device.deconfigure   = NULL;
	if (deinit) {
		LOG_DEBUG("usb", "deinit");
		deinit(&s_device);
	}
}

//de-configure last device, if any
static volatile bool s_configured = false;
static bool device_deconfigure()
{
	if (s_configured) {
		s_configured = false;
		if (s_device.deconfigure) {
			LOG_DEBUG("usb", "deconfigure");
			enum_clearconfig();
			s_device.deconfigure(&s_device);
			return true;
		}
	}
	return false;
}

static int device_configure()
{
	if (!s_configured && s_device.configure) {
		s_configured = true;
		LOG_DEBUG("usb", "configure");
		return s_device.configure(&s_device);
	}
	return 0;
}

static volatile bool s_connected = false;
static void disconnect()
{
	if (s_connected) {
		LOG_DEBUG("usb", "disconnect");
		s_connected = false;
		usb_disconnect();
		//		SYS_Reset_Periph(SYS_RESET_USB);
		mxc_delay(10000);
	}
}
static void connect()
{
	s_connected = true;
	usb_connect();
}

static volatile bool s_initialized = false;
void esb_deinit(void)
{
	if (s_initialized) {
		s_initialized = false;
		disconnect();
		device_deconfigure();
		do_usb_shutdown();
		device_deinit();
	}
}
int esb_init(struct esb_config *cfg)
{
	esb_deinit();
	if (cfg == NULL) {
		return 0;
	}
	if (timerWakeup == NULL) {
		timerWakeup = xTimerCreateStatic(
			"timerWakeup",     /* name */
			pdMS_TO_TICKS(50), /* period/time */
			pdFALSE,           /* auto reload */
			NULL,              /* timer ID */
			cb_timerReset,
			&x);
	}

	maxusb_cfg_options_t usb_opts;

	/* Initialize state */
	suspended = 0;
#ifdef USE_REMOTE_WAKE_ENABLE
	remote_wake_en = 0;
#endif

	/* Start out in full speed */
	usb_opts.enable_hs         = 0;
	usb_opts.delay_us          = delay_us;
	usb_opts.init_callback     = cb_usb_init;
	usb_opts.shutdown_callback = cb_usb_shutdown;

	/* Initialize the usb module */
	if (usb_init(&usb_opts) != 0) {
		LOG_ERR("usb", "usb_init() failed");
		return -EIO;
	}

	/* Initialize the enumeration module */
	if (enum_init() != 0) {
		do_usb_shutdown();
		LOG_ERR("usb", "enum_init() failed");
		return -EIO;
	}

	LOG_INFO("usb", "initializing device %s", cfg->name);
	if (cfg->init(cfg)) {
		enum_clearconfig();
		do_usb_shutdown();
		LOG_ERR("usb", "device init failed");
		return -EIO;
	}

	s_initialized = true;
	s_device      = *cfg;

	enum_register_descriptor(
		ENUM_DESC_DEVICE, (uint8_t *)cfg->descriptors->device, 0
	);
	enum_register_descriptor(
		ENUM_DESC_CONFIG, (uint8_t *)cfg->descriptors->config, 0
	);
	enum_register_descriptor(ENUM_DESC_STRING, lang_id_desc, 0);
	enum_register_descriptor(ENUM_DESC_STRING, mfg_id_desc, 1);
	enum_register_descriptor(ENUM_DESC_STRING, prod_id_desc, 2);
	enum_register_descriptor(ENUM_DESC_STRING, serial_id_desc, 3);
	//enum_register_descriptor(ENUM_DESC_STRING, cdcacm_func_desc, 4);
	//enum_register_descriptor(ENUM_DESC_STRING, msc_func_desc, 5);

	/* Handle configuration */
	enum_register_callback(ENUM_SETCONFIG, cb_usb_setconfig, NULL);
#ifdef USE_REMOTE_WAKE_ENABLE
	/* Handle feature set/clear */
	enum_register_callback(ENUM_SETFEATURE, cb_usb_setfeature, NULL);
	enum_register_callback(ENUM_CLRFEATURE, cb_usb_clrfeature, NULL);
#endif

	/* Register callbacks */
	usb_event_enable(MAXUSB_EVENT_NOVBUS, cb_usb_event, NULL);
	usb_event_enable(MAXUSB_EVENT_VBUS, cb_usb_event, NULL);

	/* Start with USB in low power mode */
	usb_app_sleep();
	/* TODO: Fix priority */
	NVIC_SetPriority(USB_IRQn, 6);
	NVIC_EnableIRQ(USB_IRQn);

	return 0;
}

static int cb_usb_setconfig(usb_setup_pkt *sud, void *cbdata)
{
	LOG_DEBUG("usb", "setconfig %d", sud->wValue);
	if (sud->wValue == s_device.descriptors->config->bConfigurationValue) {
		return device_configure();
	} else if (sud->wValue == 0) {
		device_deconfigure();
		return 0;
	}
	return -1;
}

#ifdef USE_REMOTE_WAKE_ENABLE
static int cb_usb_setfeature(usb_setup_pkt *sud, void *cbdata)
{
	if (sud->wValue == FEAT_REMOTE_WAKE) {
		remote_wake_en = 1;
		return 0;
	}
	return -1;
}

static int cb_usb_clrfeature(usb_setup_pkt *sud, void *cbdata)
{
	if (sud->wValue == FEAT_REMOTE_WAKE) {
		remote_wake_en = 0;
		return 0;
	}
	return -1;
}
#endif

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

static void cb_timerReset(TimerHandle_t t)
{
	(void)t;
	LOG_DEBUG("usb", "cb_timerReset %08x", usb_get_status());

	LOG_DEBUG("usb", "SYS_USBHS_Shutdown");
	SYS_USBHS_Shutdown();
	LOG_DEBUG("usb", "SYS_Reset_Periph");
	SYS_Reset_Periph(SYS_RESET_USB);

	//copy-paste from esb_init(), need to refactor
	maxusb_cfg_options_t usb_opts;
	usb_opts.enable_hs         = 0;
	usb_opts.delay_us          = delay_us;
	usb_opts.init_callback     = cb_usb_init;
	usb_opts.shutdown_callback = cb_usb_shutdown;

	/* Initialize the usb module */
	if (usb_init(&usb_opts) != 0) {
		LOG_ERR("usb", "usb_init() failed");
		return;
	}
	usb_event_enable(MAXUSB_EVENT_NOVBUS, cb_usb_event, NULL);
	usb_event_enable(MAXUSB_EVENT_VBUS, cb_usb_event, NULL);
}

static void scheduleReset()
{
	LOG_DEBUG("usb", "scheduleReset");
	xTimerChangePeriodFromISR(timerWakeup, pdMS_TO_TICKS(500), NULL);
}

#if LOG_ENABLE_DEBUG
static const char *maxusb_event_string(maxusb_event_t evt)
{
	static const char *names[MAXUSB_NUM_EVENTS] = {
		[MAXUSB_EVENT_DPACT]  = "DPACT",
		[MAXUSB_EVENT_RWUDN]  = "RWUDN",
		[MAXUSB_EVENT_BACT]   = "BACT",
		[MAXUSB_EVENT_BRST]   = "BRST",
		[MAXUSB_EVENT_SUSP]   = "SUSP",
		[MAXUSB_EVENT_NOVBUS] = "NOVBUS",
		[MAXUSB_EVENT_VBUS]   = "VBUS",
		[MAXUSB_EVENT_BRSTDN] = "BRSTDN",
		[MAXUSB_EVENT_SUDAV]  = "SUDAV",
	};
	return evt < MAXUSB_NUM_EVENTS ? names[evt] : "<INVALID>";
}
#endif

/******************************************************************************/
static int cb_usb_event(maxusb_event_t evt, void *data)
{
	static int s_VBUS_SUSP_sequence = 0;
	LOG_DEBUG("usb", "event %s (%d)\n", maxusb_event_string(evt), evt);

	switch (evt) {
	case MAXUSB_EVENT_NOVBUS:
		usb_event_disable(MAXUSB_EVENT_BRST);
		usb_event_disable(MAXUSB_EVENT_SUSP);
		usb_event_disable(MAXUSB_EVENT_DPACT);
		usb_disconnect();
		device_deconfigure();
		usb_app_sleep();
		break;
	case MAXUSB_EVENT_VBUS:
		s_VBUS_SUSP_sequence = 1;
		usb_event_clear(MAXUSB_EVENT_BRST);
		usb_event_enable(MAXUSB_EVENT_BRST, cb_usb_event, NULL);
		usb_event_clear(MAXUSB_EVENT_SUSP);
		usb_event_enable(MAXUSB_EVENT_SUSP, cb_usb_event, NULL);
		connect();
		usb_app_sleep();
		break;
	case MAXUSB_EVENT_BRST:
		usb_app_wakeup();
		device_deconfigure();
		suspended = 0;
		break;
	case MAXUSB_EVENT_SUSP:
		if (!s_VBUS_SUSP_sequence) {
			scheduleReset();
		}
		s_VBUS_SUSP_sequence = 0;
		//usb_app_sleep();
		break;
	case MAXUSB_EVENT_DPACT:
		usb_app_wakeup();
		break;
	default:
		break;
	}

	return 0;
}
