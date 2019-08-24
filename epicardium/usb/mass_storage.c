/**
 * backend-independent implementation of the mass storage device
 * services MSC API and esb_cfg API
 */

#include "usb/mass_storage.h"
#include "usb/epc_usb.h"
#include "usb/descriptors.h"

#include "modules/log.h"

#include "msc.h"
#include "usb.h"
#include "max32665.h"

#define VENDOR_STRING "CCC"
#define PRODUCT_STRING "card10"
#define VERSION_STRING "1.0"

static inline struct config_descriptor_msc *descriptors(struct esb_config *self)
{
	return self->descriptors->msc;
}

int esb_msc_configure(struct esb_config *self)
{
	LOG_DEBUG("msc", "configure");
	struct config_descriptor_msc *dsc = descriptors(self);
	const msc_cfg_t msc_cfg           = {
                dsc->endpoint_out.bEndpointAddress,
                MXC_USBHS_MAX_PACKET, /* OUT max packet size */
                dsc->endpoint_in.bEndpointAddress & 0x0f,
                MXC_USBHS_MAX_PACKET, /* IN max packet size */
	};
	return msc_configure(&msc_cfg);
}

int esb_msc_deconfigure(struct esb_config *self)
{
	LOG_DEBUG("msc", "deconfigure");
	(void)self;
	return msc_deconfigure();
}

int esb_msc_init(struct esb_config *self)
{
	LOG_DEBUG("msc", "init");
	static const msc_idstrings_t ids = { VENDOR_STRING,
					     PRODUCT_STRING,
					     VERSION_STRING };

	msc_mem_t *mem                    = self->deviceData;
	struct config_descriptor_msc *dsc = descriptors(self);
	return msc_init(&dsc->msc_interface, &ids, mem);
}
