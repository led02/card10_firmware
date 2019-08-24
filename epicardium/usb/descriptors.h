#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <stdint.h>
#include "usb.h"

#define ESB_ENDPOINT_MSC_IN         2
#define ESB_ENDPOINT_MSC_OUT        1
#define ESB_ENDPOINT_CDCACM_NOTIFY  3
#define ESB_ENDPOINT_CDCACM_IN      2
#define ESB_ENDPOINT_CDCACM_OUT     1

/* device types */
#define DT_DEVICE       0x01
#define DT_CONFIG       0x02
#define DT_INTERFACE    0x04
#define DT_ENDPOINT     0x05
#define DT_FUNCTIONAL   0x24

/* interface classes */
#define CLS_UNSPECIFIED 0x00
#define CLS_COMM        0x02
#define CLS_MASS_STOR   0x08
#define CLS_DATA        0x0a

/* sub-classes */
#define SCLS_NONE       0x00
#define SCLS_ACM        0x02
#define SCLS_SCSI_CMDS  0x06

/* interface protocols */
#define PROT_AT_CMDS    0x01
#define PROT_BULK_TRANS 0x50

/* endpoint attributes */
#define ATTR_BULK       0x02
#define ATTR_INTERRUPT  0x03


#define VNDR_MAXIM  0x0B6A

#if defined(__GNUC__)
#define PACKED_STRUCT struct __attribute__((packed))
#else
#define PACKED_STRUCT __packed struct
#endif




PACKED_STRUCT
config_descriptor_cdcacm {
    usb_configuration_descriptor_t  config;
    usb_interface_descriptor_t      comm_interface;
    uint8_t                         header_functional[5];
    uint8_t                         call_management[5];
    uint8_t                         acm_functional[4];
    uint8_t                         union_functional[5];
    usb_endpoint_descriptor_t       endpoint_notify;
    usb_interface_descriptor_t      data_interface;
    usb_endpoint_descriptor_t       endpoint_out;
    usb_endpoint_descriptor_t       endpoint_in;
};

PACKED_STRUCT
config_descriptor_msc {
    usb_configuration_descriptor_t  config;
    usb_interface_descriptor_t      msc_interface;
    usb_endpoint_descriptor_t       endpoint_out;
    usb_endpoint_descriptor_t       endpoint_in;
};

struct esb_device_descriptors {
    usb_device_descriptor_t* device;
    union {
        usb_configuration_descriptor_t* config;
        struct config_descriptor_cdcacm* cdcacm;
        struct config_descriptor_msc* msc;
    };
};


#endif /* _DESCRIPTORS_H_ */
