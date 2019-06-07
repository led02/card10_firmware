/**
 * @file       wut.c
 * @brief      This file contains the function implementations for the
 *             WUT peripheral module.
 */

/* *****************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-08-28 22:03:02 +0000 (Tue, 28 Aug 2018) $
 * $Revision: 37424 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include <string.h>
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "wut.h"

/**
 * @ingroup wut
 * @{
 */

int WUT_init(wut_config_t *config) {
	//Do the configuration

	return E_NO_ERROR;
}

int WUT_enable(void) {
	//Set the enable bit?

	return E_NO_ERROR;
}

int WUT_disable(void) {
	//Unset the enable bit?

	return E_NO_ERROR;
}

int WUT_get_time(void) {
	//return WUT time register?
	return -1;
}

int WUT_register_callback(wut_handler_t handler) {

	return E_NO_ERROR;
}

int WUT_unregister_callback(void) {

	return E_NO_ERROR;
}

void WUT_clear_flags(void) {
	return;
}

int WUT_get_flags(void) {
	return 0;
}

int WUT_set_timeout_ms(unsigned int timeout) {
	return E_NO_ERROR;
}

int WUT_mode_ctrl(wut_mode_t mode) {
	return E_NO_ERROR;
}

void WUT_Handler(void) {
	return;
}
/**@} end of group wut */
