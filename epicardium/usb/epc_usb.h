#ifndef EPICARDIUM_USB_EPC_USB_H_INCLUDED
#define EPICARDIUM_USB_EPC_USB_H_INCLUDED

/**
 * 
 * EPC - it's not just a Universal Serial Bus,
 * it's an Epic Serial Bus!
 */


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct esb_config;
typedef int (*esb_cfg_handler)(struct esb_config* self);

struct esb_config {
    const char* name;
    esb_cfg_handler init;
    esb_cfg_handler configure;
    esb_cfg_handler deconfigure;
    esb_cfg_handler deinit;

    struct esb_device_descriptors* descriptors;
    void* deviceData;
};

int esb_init(struct esb_config* cfg);
void esb_deinit(void);

#endif//EPICARDIUM_USB_EPC_USB_H_INCLUDED
