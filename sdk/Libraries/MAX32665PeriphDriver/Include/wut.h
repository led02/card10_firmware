/**
 * @file    wut.h
 * @brief   WUT function prototypes and data types.
 */

/* ****************************************************************************
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
 * $Date: 2018-11-08 21:08:58 +0000 (Thu, 08 Nov 2018) $
 * $Revision: 39056 $
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _WUT_H_
#define _WUT_H_

/* **** Includes **** */
#include "mxc_config.h"
#include "wut_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup wut Wake-up Timer (WUT)
 * @ingroup periphlibs
 * @{
 */


/** Wake-up timer initialization state */
typedef enum {
	WUT_STATE_NOT_INITIALIZED = 0, 							/**< State not initialized */
	WUT_STATE_INITIALIZED 									/**< State initialized */
} wut_state_t;

/** Wake-up timer status values */
typedef enum {
	WUT_STATUS_DISABLED = 0, 								/**< Status disabled */
	WUT_STATUS_ENABLED 										/**< Status enabled */
} wut_status_t;

typedef enum {
	WUT_MODE_WUT
} wut_mode_t;

typedef enum {
	WUT_POLARITY_NORM,
	WUT_POLARITY_INV
} wut_polarity_t;

typedef enum {
	WUT_PRESCALE_1
} wut_prescale_t;

/* Structures *****************************************************************/
typedef void (*wut_handler_t)(void);

/** This structure contains the Wake-up Wake-up timer device configuration parameters */
typedef struct {
	unsigned int timeout; 					/**< Compare value which is used to set the maximum count value to initiate a reload of the wake-up timer to 0x0001 */
	unsigned int count; 					/**< Value set in WUT_CNT register */
	unsigned int pwm_value; 				/**< PWM value. pwm_value is value compared to the current wake-up timer count (store in WUT_CNT) */
	wut_prescale_t clock; 				/**< Prescaler */
	wut_mode_t mode; 					/**< Wake-up timer mode */
	wut_polarity_t polarity;				/**< Wake-up timer polarity */
	volatile wut_handler_t handler;		/**< IRQ handler */
} wut_config_t;

/** This structure contains information about wake-up timer */
typedef struct {
	wut_state_t state; 					/**< Wake-up timer state */
	wut_status_t status; 				/**< Wake-up timer status */
	wut_handler_t handler; 				/**< Wake-up timer interrupt handler */
} wut_info_t;

/**
 * This function initializes specified wake-up timer with the configuration parameters.
 * @param[in] config				Points to configuration parameters
 * @retval NO_ERROR					No error
 */
int WUT_init(wut_config_t *config);

/**
 * This function enables (starts) the timer
 * @retval NO_ERROR							No error
 */
int WUT_enable(void);

/**
 * This function disables (stops) the timer 
 * @retval NO_ERROR							No error
 */
int WUT_disable(void);

/**
 * This function reads the timer ID current value
 * @retval 		Current Timer Value
 */
int WUT_get_time(void);

/**
 * This function registers the interrupt handler function for the
 * specified Wake-up timer ID device.
 * @param[in] handler						Interrupt handler to be registered
 * @retval NO_ERROR							No error
 */
int WUT_register_callback(wut_handler_t handler);

/**
 * This function unregisters the interrupt handler function for the
 * specified Wake-up timer ID device if any.
 * @retval NO_ERROR							No error
 * @retval ERR_NOT_INITIALIZED		Wake-up timer is not initialized
 * @retval ERR_NOT_ALLOWED			Operation not allowed
 */
int WUT_unregister_callback(void);

/**
 * This functions clears interrupt
 */
void WUT_clear_flags(void);

/**
 * @brief returns the WUT interrupt flags
 * @return flags
 */
int WUT_get_flags(void);

/**
 * This function is used to set a timeout.
 * @param[in] timeout			Timeout value to set in milliseconds.
 * @retval NO_ERROR				No error
 */
int WUT_set_timeout_ms(unsigned int timeout);

/**
 * This function configures the mode for the Wake-up timer
 * @param config							Points to configuration parameters
 * @retval NO_ERROR							No error
 */
int WUT_mode_ctrl(wut_mode_t config);

/**
 *  Wake-up timer interrupt handler for all timers
 */
void WUT_handler(void);
/**@} end of group wut */

#ifdef __cplusplus
}
#endif

#endif /* _WUT_H_ */
