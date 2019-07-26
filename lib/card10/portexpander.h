#ifndef PORTEXPANDER_H
#define PORTEXPANDER_H

#include <stdint.h>
#include <stdbool.h>

void portexpander_init(void);
uint8_t portexpander_get(void);
void portexpander_set(uint8_t pin, uint8_t value);
bool portexpander_detected(void);

#endif
