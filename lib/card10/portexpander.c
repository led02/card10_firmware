#include "portexpander.h"

#include "i2c.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// PCAL6408A I2C port expander

static bool detected = false;
static uint8_t output_state;

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

static int portexpander_write(uint8_t command, uint8_t data)
{
	uint8_t i2c_data[2] = { command, data };
	return I2C_MasterWrite(MXC_I2C1_BUS0, PE_ADDR, i2c_data, 2, 0);
}

static int portexpander_read(uint8_t command, uint8_t *data)
{
	I2C_MasterWrite(MXC_I2C1_BUS0, PE_ADDR, &command, 1, 1);
	return I2C_MasterRead(MXC_I2C1_BUS0, PE_ADDR, data, 1, 0);
}

void portexpander_init(void)
{
	int ret;

	// Enable pull-ups for buttons (type defaults to pull-up)
	ret = portexpander_write(PE_C_PULL_ENABLE, PE_INPUT_MASK);

	if (ret != 2) {
		printf("portexpander NOT detected\n");
		detected = false;
		return;
	}
	detected = true;

	// Set _all_ outputs to open-drain to support the high side p-channel transistors.
	portexpander_write(PE_C_OUTPUT_PORT_CONFIG, PE_OUT_OPEN_DRAIN);

	// Enable outputs for the transistors, the LED and the LCD reset
	portexpander_write(PE_C_CONFIG, PE_INPUT_MASK);

	// Set outputs to high (i.e. open-drain)
	output_state = ~PE_INPUT_MASK;
	portexpander_write(PE_C_OUTPUT_PORT, output_state);
}

uint8_t portexpander_get(void)
{
	uint8_t buf = 0xFF;

	if (detected) {
		portexpander_read(PE_C_INPUT_PORT, &buf);
	}

	return buf;
}

bool portexpander_detected(void)
{
	return detected;
}

void portexpander_set(uint8_t pin, uint8_t value)
{
	if (detected && pin < 8) {
		if (value) {
			output_state |= (1 << pin);
		} else {
			output_state &= ~(1 << pin);
		}

		portexpander_write(PE_C_OUTPUT_PORT, output_state);
	}
}

void portexpander_prep(uint8_t pin, uint8_t value)
{
	if (pin < 8) {
		if (value) {
			output_state |= (1 << pin);
		} else {
			output_state &= ~(1 << pin);
		}
	}
}

void portexpander_update(void)
{
	if (detected) {
		portexpander_write(PE_C_OUTPUT_PORT, output_state);
	}
}

void portexpander_set_mask(uint8_t mask, uint8_t values)
{
	if (detected) {
		output_state &= ~(mask & ~values);
		output_state |= mask & values;
		portexpander_write(PE_C_OUTPUT_PORT, output_state);
	}
}
