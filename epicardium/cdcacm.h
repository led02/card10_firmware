#ifndef CDCACM_H
#define CDCACM_H
#include <stdint.h>

int cdcacm_init(void);
int cdcacm_num_read_avail(void);
uint8_t cdcacm_read(void);
void cdcacm_write(uint8_t *data, int len);

#endif
