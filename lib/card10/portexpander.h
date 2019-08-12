#ifndef PORTEXPANDER_H
#define PORTEXPANDER_H

#include <stdint.h>
#include <stdbool.h>

void portexpander_init(void);
uint8_t portexpander_get(void);
void portexpander_set(uint8_t pin, uint8_t value);
void portexpander_set_mask(uint8_t mask, uint8_t values);
void portexpander_prep(uint8_t pin, uint8_t value);
void portexpander_update(void);
bool portexpander_detected(void);

#endif
