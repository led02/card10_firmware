#ifndef PORTEXPANDER_H
#define PORTEXPANDER_H

#include "mxc_config.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * Structure type for configuring the portexpander.
 */
typedef struct {
        uint8_t mask;           /**< Pin mask (multiple pins may be set) */
        gpio_func_t func;       /**< Function type */
        gpio_pad_t pad;         /**< Pad type */
} portexpander_cfg_t;


typedef void (*pe_callback)(gpio_int_pol_t edge_type, void *cbdata);

int portexpander_init(void);
bool portexpander_detected(void);

int portexpander_config(const portexpander_cfg_t *cfg);

uint8_t portexpander_in_get(uint8_t mask);

void portexpander_out_set(uint8_t mask);
void portexpander_out_clr(uint8_t mask);
void portexpander_out_put(uint8_t mask, uint8_t val);
void portexpander_out_toggle(uint8_t mask);
uint8_t portexpander_out_get(uint8_t mask);

void portexpander_int_config(uint8_t mask, gpio_int_pol_t edge);
void portexpander_int_enable(uint8_t mask);
void portexpander_int_disable(uint8_t mask);
uint8_t portexpander_int_status();
void portexpander_int_clr(uint8_t mask);
int portexpander_register_callback(uint8_t mask, pe_callback callback, void *cbdata);
void portexpander_poll();

void portexpander_interrupt_callback(void *_);

#endif
