#ifndef PMIC_H
#define PMIC_H

#include <stdint.h>
#include <stdbool.h>

/* Dropout Detector Rising */
#define MAX77650_INT_DOD_R    (1 << 6)
/* Thermal Alarm 2 Rising */
#define MAX77650_INT_TJAL2_R  (1 << 5)
/* Thermal Alarm 1 Rising */
#define MAX77650_INT_TJAL1_R  (1 << 4)
/* nEN Rising */
#define MAX77650_INT_nEN_R    (1 << 3)
/* nEN Falling */
#define MAX77650_INT_nEN_F    (1 << 2)
/* GPI Rising */
#define MAX77650_INT_GPI_R    (1 << 1)
/* GPI Falling */
#define MAX77650_INT_GPI_F    (1 << 0)

void pmic_init(void);
void pmic_set_led(uint8_t led, uint8_t val);
void pmic_poll(void);

/* weak, so it can be overwritten by applications */
void pmic_interrupt_callback(void*_);

typedef void (*pmic_button_callback_fn)(bool falling);
void pmic_set_button_callback(pmic_button_callback_fn cb);

#endif
