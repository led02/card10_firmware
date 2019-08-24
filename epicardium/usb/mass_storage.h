#ifndef EPICARDIUM_USB_MSC_H_INCLUDED
#define EPICARDIUM_USB_MSC_H_INCLUDED

#include "usb/epc_usb.h"

int esb_msc_configure(struct esb_config* self);
int esb_msc_deconfigure(struct esb_config* self);
int esb_msc_init(struct esb_config* self);

#endif//EPICARDIUM_USB_MSC_H_INCLUDED
