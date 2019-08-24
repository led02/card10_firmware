#ifndef CDCACM_H
#define CDCACM_H

#include "usb/epc_usb.h"

// callbacks for esb_config
int esb_cdc_init(struct esb_config* self);
int esb_cdc_configure(struct esb_config* self);
int esb_cdc_deconfigure(struct esb_config* self);

int cdcacm_num_read_avail(void);
uint8_t cdcacm_read(void);
void cdcacm_write(uint8_t *data, int len);

#endif
