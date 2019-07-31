#ifndef CARD10_H

#include "gpio.h"

#include <stdint.h>

extern const gpio_cfg_t bhi_interrupt_pin;

void card10_init(void);
void card10_diag(void);

void core1_start(void *isr);
void core1_stop(void);

void card10_poll(void);
void card10_reset(void);
#endif
