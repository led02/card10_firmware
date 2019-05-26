#ifndef CARD10_H

#include "gpio.h"

#include <stdint.h>

extern const gpio_cfg_t bhi_interrupt_pin;

void card10_init(void);
void card10_diag(void);

#endif
