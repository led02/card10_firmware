#ifndef PMIC_H
#define PMIC_H

#include <stdint.h>
#include <stdbool.h>

void pmic_init(void);
void pmic_set_led(uint8_t led, uint8_t val);

typedef void (*pmic_button_callback_fn)(bool falling);
void pmic_set_button_callback(pmic_button_callback_fn cb);

#endif
