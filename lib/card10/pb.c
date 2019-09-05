/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-08-13 23:38:48 +0000 (Mon, 13 Aug 2018) $
 * $Revision: 36945 $
 *
 ******************************************************************************/

#include "mxc_config.h"
#include "mxc_assert.h"
#include "pb.h"
#include "portexpander.h"
#include "MAX77650-Arduino-Library.h"
#include <stddef.h>

static const uint8_t expander_pins[] = { 5, 0x0, 3, 6 };
static pb_callback pb_callbacks[4]   = { NULL };

/******************************************************************************/
int PB_Init(void)
{
	int retval = E_NO_ERROR;
	unsigned int i;

	// If we have a port expander, its pins are already configured
	if (!portexpander_detected()) {
		// Enable pushbutton inputs
		for (i = 0; i < num_pbs; i++) {
			if (GPIO_Config(&pb_pin[i]) != E_NO_ERROR) {
				retval = E_UNKNOWN;
			}
		}
	}

	return retval;
}

static void pe_pb_callback(gpio_int_pol_t edge_type, void *cbdata)
{
	unsigned int pb = (unsigned int)cbdata;
	if (pb_callbacks[pb - 1]) {
		pb_callbacks[pb - 1](pb, edge_type == GPIO_INT_FALLING);
	}
}

static void gpio_pb_callback(void *cbdata)
{
	unsigned int pb = (unsigned int)cbdata;
	if (pb_callbacks[pb - 1]) {
		int level = GPIO_InGet(&pb_pin[pb - 1]);
		pb_callbacks[pb - 1](pb, !level);
	}
}

/******************************************************************************/
int PB_RegisterCallback(unsigned int pb, pb_callback callback)
{
	MXC_ASSERT((pb > 0) && (pb <= num_pbs));

	if (pb == 2) {
		return E_INVALID;
	}

	pb_callbacks[pb - 1] = callback;

	uint8_t mask = (1 << expander_pins[pb - 1]);

	if (callback) {
		if (portexpander_detected()) {
			// Register callback
			portexpander_register_callback(
				mask, pe_pb_callback, (void *)pb
			);

			// Configure and enable interrupt
			portexpander_int_config(mask, GPIO_INT_BOTH);
			portexpander_int_enable(mask);
		} else {
			// Register callback
			GPIO_RegisterCallback(
				&pb_pin[pb - 1], gpio_pb_callback, (void *)pb
			);

			// Configure and enable interrupt
			GPIO_IntConfig(
				&pb_pin[pb - 1], GPIO_INT_EDGE, GPIO_INT_BOTH
			);
			GPIO_IntEnable(&pb_pin[pb - 1]);
			NVIC_EnableIRQ((IRQn_Type)MXC_GPIO_GET_IRQ(
				pb_pin[pb - 1].port)
			);
		}
	} else {
		if (portexpander_detected()) {
			// Disable interrupt and clear callback
			portexpander_int_disable(mask);
			portexpander_register_callback(mask, NULL, NULL);
		} else {
			// Disable interrupt and clear callback
			GPIO_IntDisable(&pb_pin[pb - 1]);
			GPIO_RegisterCallback(&pb_pin[pb - 1], NULL, NULL);
		}
	}

	return E_NO_ERROR;
}

//******************************************************************************
void PB_IntEnable(unsigned int pb)
{
	MXC_ASSERT((pb > 0) && (pb <= num_pbs));
	if (pb == 2) {
		return;
	}

	if (portexpander_detected()) {
		portexpander_int_enable((1 << expander_pins[pb - 1]));
	} else {
		GPIO_IntEnable(&pb_pin[pb - 1]);
	}
}

//******************************************************************************
void PB_IntDisable(unsigned int pb)
{
	MXC_ASSERT((pb > 0) && (pb <= num_pbs));
	if (pb == 2) {
		return;
	}

	if (portexpander_detected()) {
		portexpander_int_disable((1 << expander_pins[pb - 1]));
	} else {
		GPIO_IntDisable(&pb_pin[pb - 1]);
	}
}

//******************************************************************************
void PB_IntClear(unsigned int pb)
{
	MXC_ASSERT((pb > 0) && (pb <= num_pbs));
	if (pb == 2) {
		return;
	}

	if (portexpander_detected()) {
		portexpander_int_clr((1 << expander_pins[pb - 1]));
	} else {
		GPIO_IntClr(&pb_pin[pb - 1]);
	}
}

//******************************************************************************
int PB_Get(unsigned int pb)
{
	static const uint8_t expander_pins[] = { 5, 0x0, 3, 6 };
	MXC_ASSERT(pb <= 4);
	switch (pb) {
	case 1:
	case 3:
	case 4:
		if (portexpander_detected()) {
			return portexpander_in_get(
				       (1 << expander_pins[pb - 1])) == 0;
		} else {
			return GPIO_InGet(&pb_pin[pb - 1]) == 0;
		}
		break;
	case 2:
		return MAX77650_getDebounceStatusnEN0();
		break;
	}
	return 0;
}
