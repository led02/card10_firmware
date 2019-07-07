#ifndef PORTEXPANDER_H
#define PORTEXPANDER_H

#include <stdint.h>
#include <stdbool.h>

void portexpander_init(void);
uint8_t portexpander_get(void);
bool portexpander_detected(void);

#endif
