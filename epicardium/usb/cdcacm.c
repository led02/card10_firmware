/**
 * 
 * epicardium-specific implemnetation of cdcacm device
 * services CDCACM API
 */

#include "usb/epc_usb.h"
#include "usb/cdcacm.h"
#include "usb/descriptors.h"

#include <stdio.h>
#include <stddef.h>
#include <errno.h>

#include "usb.h"
#include "usb_event.h"
#include "cdc_acm.h"

#include "modules/log.h"
#include "modules/modules.h"

#include "FreeRTOS.h"
#include "task.h"

static inline struct config_descriptor_cdcacm *
descriptors(struct esb_config *self)
{
	return self->descriptors->cdcacm;
}

/******************************************************************************/
static volatile int usb_read_complete; // SWYM: only ever written to
static int cb_acm_read_ready(void)
{
	usb_read_complete = 1;
	return 0;
}

int esb_cdc_init(struct esb_config *self)
{
	LOG_DEBUG("cdcacm", "init");
	struct config_descriptor_cdcacm *dsc = descriptors(self);
	usb_read_complete                    = 0;
	acm_register_callback(
		ACM_CB_READ_READY,
		cb_acm_read_ready); //SWYM: actually not needed
	return acm_init(&dsc->comm_interface);
}

int esb_cdc_configure(struct esb_config *self)
{
	struct config_descriptor_cdcacm *dsc = descriptors(self);

	//acm_configure does not keep the amc_cfg_t pointer around
	//so stack-local lifetime is fine
	acm_cfg_t acm_cfg = {
		.in_ep         = dsc->endpoint_in.bEndpointAddress & 0x0f,
		.in_maxpacket  = MXC_USBHS_MAX_PACKET,
		.out_ep        = dsc->endpoint_out.bEndpointAddress,
		.out_maxpacket = MXC_USBHS_MAX_PACKET,
		.notify_ep     = dsc->endpoint_notify.bEndpointAddress & 0x0f,
		.notify_maxpacket = MXC_USBHS_MAX_PACKET,
	};

	LOG_DEBUG(
		"cdcacm",
		"configure, endpoints %d,%d,%d",
		acm_cfg.in_ep,
		acm_cfg.out_ep,
		acm_cfg.notify_ep
	);

	return acm_configure(&acm_cfg);
}

int esb_cdc_deconfigure(struct esb_config *self)
{
	LOG_DEBUG("cdcacm", "deconfigure");
	return acm_deconfigure();
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

extern TaskHandle_t serial_task_id;
void USB_IRQHandler(void)
{
	usb_event_handler();

	if (serial_task_id != NULL) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xTaskNotifyFromISR(
			serial_task_id,
			SERIAL_READ_NOTIFY,
			eSetBits,
			&xHigherPriorityTaskWoken
		);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
