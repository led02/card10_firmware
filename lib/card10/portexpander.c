/* PCAL6408A I2C port expander */

/* **** Includes **** */
#include "portexpander.h"

#include "mxc_config.h"
#include "mxc_assert.h"
#include "i2c.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* **** Definitions **** */

/* clang-format off */
#define PE_ADDR 0x42

#define PE_C_INPUT_PORT         0x00
#define PE_C_OUTPUT_PORT        0x01
#define PE_C_POLARITY_INV       0x02
#define PE_C_CONFIG             0x03
#define PE_C_OUTPUT_DRIVE_STR_0 0x40
#define PE_C_OUTPUT_DRIVE_STR_1 0x41
#define PE_C_INPUT_LATCH        0x42
#define PE_C_PULL_ENABLE        0x43
#define PE_C_PULL_SEL           0x44
#define PE_C_INT_MASK           0x45
#define PE_C_INT_STATUS         0x46
#define PE_C_OUTPUT_PORT_CONFIG 0x4F

#define PE_CONFIG_OUT 0
#define PE_CONFIG_IN  1

#define PE_PULL_DOWN 0
#define PE_PULL_UP   1

#define PE_OUT_PUSH_PULL  0
#define PE_OUT_OPEN_DRAIN 1
/* clang-format on */

#define PE_INPUT_MASK ((uint8_t)0b01101000) // 3, 5, 6 = input

/* **** Globals **** */

static bool detected = false;

static volatile bool interrupt_pending;

static uint8_t type_state           = 0xFF;
static uint8_t output_state         = 0xFF;
static uint8_t pull_enable_state    = 0x00;
static uint8_t pull_selection_state = 0xFF;
static uint8_t int_mask_state       = 0xFF;

static gpio_int_pol_t int_edge_config[8] = { 0 };

static pe_callback callbacks[8] = { NULL };
static void *cbparam[8]         = { NULL };

const gpio_cfg_t pe_int_pin = { PORT_1, PIN_7, GPIO_FUNC_IN, GPIO_PAD_PULL_UP };

static const portexpander_cfg_t pe_pin_config[] = {
	{ PE_INPUT_MASK, GPIO_FUNC_IN, GPIO_PAD_PULL_UP },
	{ ~PE_INPUT_MASK, GPIO_FUNC_OUT, GPIO_PAD_PULL_UP },
};

/* **** Functions **** */

static int portexpander_write(uint8_t command, uint8_t data)
{
	uint8_t i2c_data[2] = { command, data };
	return I2C_MasterWrite(MXC_I2C1_BUS0, PE_ADDR, i2c_data, 2, 0);
}

/* ************************************************************************** */
static int portexpander_read(uint8_t command, uint8_t *data)
{
	I2C_MasterWrite(MXC_I2C1_BUS0, PE_ADDR, &command, 1, 1);
	return I2C_MasterRead(MXC_I2C1_BUS0, PE_ADDR, data, 1, 0);
}

/* ************************************************************************** */
int portexpander_init(void)
{
	int ret;

	// Set _all_ outputs to open-drain to support the high side p-channel transistors.
	ret = portexpander_write(PE_C_OUTPUT_PORT_CONFIG, PE_OUT_OPEN_DRAIN);
	if (ret != 2) {
		printf("portexpander NOT detected\n");
		detected = false;
		return E_NO_DEVICE;
	}
	detected = true;

	// Set outputs to high
	portexpander_out_set(~PE_INPUT_MASK);

	// Enable pull-ups for buttons
	// Enable outputs for the transistors, the LED and the LCD reset
	for (int i = 0; i < sizeof(pe_pin_config) / sizeof(pe_pin_config[0]);
	     i++) {
		MXC_ASSERT(
			portexpander_config(&pe_pin_config[i]) == E_NO_ERROR
		);
	}

	// Latch inputs so we can figure out whether an interrupt was caused by a rising or falling edge
	portexpander_write(PE_C_INPUT_LATCH, PE_INPUT_MASK);

	// Configure interrupt GPIO
	MXC_ASSERT(GPIO_Config(&pe_int_pin) == E_NO_ERROR);

	// Configure and enable portexpander interrupt
	GPIO_RegisterCallback(
		&pe_int_pin, &portexpander_interrupt_callback, NULL
	);
	MXC_ASSERT(
		GPIO_IntConfig(&pe_int_pin, GPIO_INT_EDGE, GPIO_INT_FALLING) ==
		E_NO_ERROR);
	GPIO_IntEnable(&pe_int_pin);
	NVIC_EnableIRQ((IRQn_Type)MXC_GPIO_GET_IRQ(pe_int_pin.port));

	return E_SUCCESS;
}

/* ************************************************************************** */
int portexpander_config(const portexpander_cfg_t *cfg)
{
	// Set the GPIO type
	switch (cfg->func) {
	case GPIO_FUNC_IN:
		type_state |= cfg->mask;
		break;
	case GPIO_FUNC_OUT:
		type_state &= ~cfg->mask;
		break;
	default:
		return E_BAD_PARAM;
	}

	if (portexpander_write(PE_C_CONFIG, type_state) != 2) {
		return E_NO_DEVICE;
	}

	switch (cfg->pad) {
	case GPIO_PAD_NONE:
		pull_enable_state &= ~cfg->mask;
		break;
	case GPIO_PAD_PULL_UP:
		pull_selection_state |= cfg->mask;
		pull_enable_state |= cfg->mask;
		break;
	case GPIO_PAD_PULL_DOWN:
		pull_selection_state &= ~cfg->mask;
		pull_enable_state |= cfg->mask;
		break;
	default:
		return E_BAD_PARAM;
	}

	portexpander_write(PE_C_PULL_ENABLE, pull_selection_state);
	portexpander_write(PE_C_PULL_ENABLE, pull_enable_state);

	return E_NO_ERROR;
}

/* ************************************************************************** */
uint8_t portexpander_in_get(uint8_t mask)
{
	// Reading the input port clears interrupts, so we need to check them here to avoid losing information
	portexpander_poll();

	uint8_t buf = 0xFF;

	if (detected) {
		portexpander_read(PE_C_INPUT_PORT, &buf);
	}

	return buf & mask;
}

/* ************************************************************************** */
bool portexpander_detected(void)
{
	return detected;
}

/* ************************************************************************** */
void portexpander_out_set(uint8_t mask)
{
	if (detected) {
		output_state |= mask;
		portexpander_write(PE_C_OUTPUT_PORT, output_state);
	}
}

/* ************************************************************************** */
void portexpander_out_clr(uint8_t mask)
{
	if (detected) {
		output_state &= ~mask;
		portexpander_write(PE_C_OUTPUT_PORT, output_state);
	}
}

/* ************************************************************************** */
uint8_t portexpander_out_get(uint8_t mask)
{
	return output_state & mask;
}

/* ************************************************************************** */
void portexpander_out_put(uint8_t mask, uint8_t val)
{
	if (detected) {
		output_state = (output_state & ~mask) | (val & mask);
		portexpander_write(PE_C_OUTPUT_PORT, output_state);
	}
}

/* ************************************************************************** */
void portexpander_out_toggle(uint8_t mask)
{
	if (detected) {
		output_state ^= mask;
		portexpander_write(PE_C_OUTPUT_PORT, output_state);
	}
}

/* ************************************************************************** */
void portexpander_int_config(uint8_t mask, gpio_int_pol_t edge)
{
	if (detected) {
		for (uint8_t pin = 0; pin < 8; ++pin) {
			if (mask & (1 << pin)) {
				int_edge_config[pin] = edge;
			}
		}
	}
}

/* ************************************************************************** */
void portexpander_int_enable(uint8_t mask)
{
	if (detected) {
		int_mask_state &= ~mask;
		portexpander_write(PE_C_INT_MASK, int_mask_state);
	}
}

/* ************************************************************************** */
void portexpander_int_disable(uint8_t mask)
{
	if (detected) {
		int_mask_state |= mask;
		portexpander_write(PE_C_INT_MASK, int_mask_state);
	}
}

/* ************************************************************************** */
uint8_t portexpander_int_status()
{
	uint8_t buf = 0;
	if (detected) {
		portexpander_read(PE_C_INT_STATUS, &buf);
	}

	return buf;
}

/* ************************************************************************** */
void portexpander_int_clr(uint8_t mask)
{
	if (detected) {
		uint8_t tmp_mask = int_mask_state | mask;

		// Setting an interrupt mask clears the corresponding interrupt
		portexpander_write(PE_C_INT_MASK, tmp_mask);
		portexpander_write(PE_C_INT_MASK, int_mask_state);
	}
}

/* ************************************************************************** */
int portexpander_register_callback(
	uint8_t mask, pe_callback callback, void *cbdata
) {
	if (!detected) {
		return E_NO_DEVICE;
	}

	for (uint8_t pin = 0; pin < 8; ++pin) {
		if (mask & (1 << pin)) {
			callbacks[pin] = callback;
			cbparam[pin]   = cbdata;
		}
	}

	return E_NO_ERROR;
}

/* ************************************************************************** */
__attribute__((weak)) void portexpander_interrupt_callback(void *_)
{
	GPIO_IntDisable(&pe_int_pin);
	GPIO_IntClr(&pe_int_pin);
	interrupt_pending = true;
}

/* ************************************************************************** */
void portexpander_poll()
{
	if (detected && interrupt_pending) {
		interrupt_pending = false;

		uint8_t caused_by = portexpander_int_status();
		// Port read resets interrupts
		uint8_t port_levels = portexpander_in_get(0xFF);

		GPIO_IntEnable(&pe_int_pin);

		for (uint8_t pin = 0; pin < 8; ++pin) {
			if ((caused_by & (1 << pin)) && callbacks[pin]) {
				gpio_int_pol_t edge_type =
					(port_levels & (1 << pin) ?
						 GPIO_INT_RISING :
						 GPIO_INT_FALLING);
				if ((int_edge_config[pin] == GPIO_INT_BOTH) ||
				    (edge_type == int_edge_config[pin])) {
					callbacks[pin](edge_type, cbparam[pin]);
				}
			}
		}
	}
}
