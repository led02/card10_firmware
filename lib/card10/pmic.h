#ifndef PMIC_H
#define PMIC_H

#include <stdint.h>

void pmic_init(void);
void pmic_set_led(uint8_t led, uint8_t val);

#endif
