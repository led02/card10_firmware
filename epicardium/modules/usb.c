/**
 * 
 * Implementation of public epic_usb_ interface
 * 
 * Configuration and FLASH-specific implementation of USB mass storage device
 * Configuration of USB CDCACM device
 * 
 * USB descriptors for both are defined here.
 * 
 */

#include "epicardium.h"

#include "modules/filesystem.h"

#include "usb/cdcacm.h"
#include "usb/mass_storage.h"
#include "usb/descriptors.h"
#include "usb/epc_usb.h"

#include "modules/log.h"

#include "mx25lba.h"
#include "msc.h"

/* memory access callbacks for the mass storage (FLASH) device */
static int mscmem_init();
static uint32_t mscmem_size(void);
static int mscmem_read(uint32_t lba, uint8_t *buffer);
static int mscmem_write(uint32_t lba, uint8_t *buffer);
static int mscmem_start();
static int mscmem_stop();
static int mscmem_ready();

/* USB descriptors, definition at the end of this file */
static usb_device_descriptor_t device_descriptor_cdcacm
	__attribute__((aligned(4)));
static struct config_descriptor_cdcacm config_descriptor_cdcacm
	__attribute__((aligned(4)));
static usb_device_descriptor_t device_descriptor_msc
	__attribute__((aligned(4)));
static struct config_descriptor_msc config_descriptor_msc
	__attribute__((aligned(4)));

/* definitions of config structs */
static const msc_mem_t s_mscCallbacks = {
	mscmem_init, mscmem_start, mscmem_stop,  mscmem_ready,
	mscmem_size, mscmem_read,  mscmem_write,
};

static struct esb_device_descriptors s_dd_cdcacm = {
	.device = &device_descriptor_cdcacm, .cdcacm = &config_descriptor_cdcacm
};

static struct esb_device_descriptors s_dd_msc = {
	.device = &device_descriptor_msc, .msc = &config_descriptor_msc
};

static struct esb_config s_cfg_cdcacm = {
	.name        = "cdcacm",
	.init        = esb_cdc_init,
	.configure   = esb_cdc_configure,
	.deconfigure = esb_cdc_deconfigure,
	.deinit      = NULL,

	.descriptors = &s_dd_cdcacm,
	.deviceData  = NULL,
};

static struct esb_config s_cfg_msc = {
	.name        = "msc FLASH",
	.init        = esb_msc_init,
	.configure   = esb_msc_configure,
	.deconfigure = esb_msc_deconfigure,
	.deinit      = NULL,

	.descriptors = &s_dd_msc,
	.deviceData  = (void *)&s_mscCallbacks,
};

/* function implementations */

static int dirty = 0;
static int mscmem_init()
{
	LOG_DEBUG("msc", "mem.init");
	fatfs_detach();
	return mx25_init();
}

static uint32_t mscmem_size(void)
{
	return mx25_size();
}

static int mscmem_read(uint32_t lba, uint8_t *buffer)
{
	return mx25_read(lba, buffer);
}

static int mscmem_write(uint32_t lba, uint8_t *buffer)
{
	if (dirty == 0) {
		//bootloader_dirty();
	}
	dirty = 2;
	return mx25_write(lba, buffer);
}

static int mscmem_start()
{
	return mx25_start();
}

static int mscmem_stop()
{
	LOG_DEBUG("msc", "mem.stop");
	int ret = mx25_stop();
	fatfs_schedule_attach();
	return ret;
}

static int mscmem_ready()
{
	if (dirty) {
		dirty--;
		if (dirty == 0) {
			LOG_DEBUG("msc", "sync");
			mx25_sync();
			//bootloader_clean();
		}
	}
	return mx25_ready();
}

static bool s_fsDetached = false;
int epic_usb_shutdown(void)
{
	esb_deinit();
	if (s_fsDetached) {
		fatfs_attach();
	}
	return 0;
}

int epic_usb_storage(void)
{
	esb_deinit();
	s_fsDetached = true;
	return esb_init(&s_cfg_msc);
}

int epic_usb_cdcacm(void)
{
	esb_deinit();
	if (s_fsDetached) {
		fatfs_attach();
	}
	return esb_init(&s_cfg_cdcacm);
}

/* clang-format off */
static usb_device_descriptor_t device_descriptor_cdcacm = {
	.bLength            = 0x12,
	.bDescriptorType    = DT_DEVICE,
	.bcdUSB             = 0x0110,
	.bDeviceClass       = CLS_COMM,
	.bDeviceSubClass    = 0x00,
	.bDeviceProtocol    = 0x00,
	.bMaxPacketSize     = 0x40,
	.idVendor           = VNDR_MAXIM,
	.idProduct          = 0x003C,
	.bcdDevice          = 0x0100,
	.iManufacturer      = 0x01,
	.iProduct           = 0x02,
	.iSerialNumber      = 0x00,
	.bNumConfigurations = 0x01
};

static struct config_descriptor_cdcacm config_descriptor_cdcacm = {
    .config = {
        .bLength            = 0x09,
        .bDescriptorType    = DT_CONFIG,
        .wTotalLength       = 0x0043,     /*  sizeof(struct config_descriptor_cdcacm) */
        .bNumInterfaces     = 0x02,
        .bConfigurationValue= 0x01,
        .iConfiguration     = 0x00,
        .bmAttributes       = 0xE0,       /*  (self-powered, remote wakeup) */
        .bMaxPower          = 0x01,       /*  MaxPower is 2ma (units are 2ma/bit) */
    },
    .comm_interface = { /*  First Interface Descriptor For Comm Class Interface */
        .bLength            = 0x09,
        .bDescriptorType    = DT_INTERFACE,
        .bInterfaceNumber   = 0x00,
        .bAlternateSetting  = 0x00,
        .bNumEndpoints      = 0x01,
        .bInterfaceClass    = CLS_COMM,
        .bInterfaceSubClass = SCLS_ACM,
        .bInterfaceProtocol = PROT_AT_CMDS,
        .iInterface         = 0x00,
    },
    .header_functional = {
        0x05,         /*  bFunctionalLength = 5           */
        DT_FUNCTIONAL,/*  bDescriptorType                 */
        0x00,         /*  bDescriptorSubtype              */
        0x10, 0x01,   /*  bcdCDC                          */
    },
    .call_management = {
        0x05,         /*  bFunctionalLength = 5           */
        DT_FUNCTIONAL,/*  bDescriptorType                 */
        0x01,         /*  bDescriptorSubtype              */
        0x03,         /*  bmCapabilities = Device handles call management itself (0x01), management over data class (0x02) */
        0x01,         /*  bmDataInterface                 */
    },
    .acm_functional = {
        0x04,         /*  bFunctionalLength = 4           */
        DT_FUNCTIONAL,/*  bDescriptorType                 */
        0x02,         /*  bDescriptorSubtype              */
        0x02,         /*  bmCapabilities                  */
    },
    .union_functional = {
        0x05,         /*  bFunctionalLength = 5           */
        DT_FUNCTIONAL,/*  bDescriptorType                 */
        0x06,         /*  bDescriptorSubtype              */
        0x00,         /*  bmMasterInterface               */
        0x01,         /*  bmSlaveInterface0               */
    },
    .endpoint_notify = {
        .bLength            = 0x07,
        .bDescriptorType    = DT_ENDPOINT,
        .bEndpointAddress   = 0x80 | ESB_ENDPOINT_CDCACM_NOTIFY,
        .bmAttributes       = ATTR_INTERRUPT,
        .wMaxPacketSize     = 0x0040,
        .bInterval          = 0xff,
    },
    .data_interface = {
        .bLength            = 0x09,
        .bDescriptorType    = DT_INTERFACE,
        .bInterfaceNumber   = 0x01,
        .bAlternateSetting  = 0x00,
        .bNumEndpoints      = 0x02,
        .bInterfaceClass    = CLS_DATA,
        .bInterfaceSubClass = 0x00,
        .bInterfaceProtocol = 0x00,
        .iInterface         = 0x00, //not 1?
    },
    .endpoint_out = {
        .bLength            = 0x07,
        .bDescriptorType    = DT_ENDPOINT,
        .bEndpointAddress   = ESB_ENDPOINT_CDCACM_OUT,
        .bmAttributes       = ATTR_BULK,
        .wMaxPacketSize     = 0x0040,
        .bInterval          = 0x00,
    },
    .endpoint_in = {
        .bLength            = 0x07,
        .bDescriptorType    = DT_ENDPOINT,
        .bEndpointAddress   = 0x80 | ESB_ENDPOINT_CDCACM_IN,
        .bmAttributes       = ATTR_BULK,
        .wMaxPacketSize     = 0x0040,
        .bInterval          = 0x00
    }
};

static usb_device_descriptor_t device_descriptor_msc = {
	.bLength            = 0x12,
	.bDescriptorType    = DT_DEVICE,
	.bcdUSB             = 0x0200,
	.bDeviceClass       = CLS_UNSPECIFIED,
	.bDeviceSubClass    = 0x00,
	.bDeviceProtocol    = 0x00,
	.bMaxPacketSize     = 0x40,
	.idVendor           = VNDR_MAXIM,
	.idProduct          = 0x4402,
	.bcdDevice          = 0x0100,
	.iManufacturer      = 0x01,
	.iProduct           = 0x02,
	.iSerialNumber      = 0x03,
	.bNumConfigurations = 0x01
};

static struct config_descriptor_msc config_descriptor_msc =
{
    .config = {
        .bLength            = 0x09,
        .bDescriptorType    = DT_CONFIG,
        .wTotalLength       = 0x0020,
        .bNumInterfaces     = 0x01,
        .bConfigurationValue= 0x01,
        .iConfiguration     = 0x00,
        .bmAttributes       = 0xC0,       /* (self-powered, no remote wakeup) */
        .bMaxPower          = 0x32,
    },
    .msc_interface = {
        .bLength                = 0x09,
        .bDescriptorType        = DT_INTERFACE,
        .bInterfaceNumber       = 0x00,
        .bAlternateSetting      = 0x00,
        .bNumEndpoints          = 0x02,
        .bInterfaceClass        = CLS_MASS_STOR,
        .bInterfaceSubClass     = SCLS_SCSI_CMDS,
        .bInterfaceProtocol     = PROT_BULK_TRANS,
        .iInterface             = 0x00,
    },
    .endpoint_out = {
        .bLength            = 0x07,
        .bDescriptorType    = DT_ENDPOINT,
        .bEndpointAddress   = ESB_ENDPOINT_MSC_OUT,
        .bmAttributes       = ATTR_BULK,
        .wMaxPacketSize     = 0x0040,
        .bInterval          = 0x00,
    },
    .endpoint_in = {
        .bLength            = 0x07,
        .bDescriptorType    = DT_ENDPOINT,
        .bEndpointAddress   = 0x80 | ESB_ENDPOINT_MSC_IN,
        .bmAttributes       = ATTR_BULK,
        .wMaxPacketSize     = 0x0040,
        .bInterval          = 0x00
    }
};
/* clang-format on */