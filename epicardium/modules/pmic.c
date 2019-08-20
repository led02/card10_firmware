#include "epicardium.h"
#include "modules/modules.h"
#include "modules/log.h"

#include "card10.h"
#include "pmic.h"
#include "MAX77650-Arduino-Library.h"

#include "max32665.h"
#include "mxc_sys.h"
#include "mxc_pins.h"
#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

/* Task ID for the pmic handler */
static TaskHandle_t pmic_task_id = NULL;

void pmic_interrupt_callback(void *_)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (pmic_task_id != NULL) {
		vTaskNotifyGiveFromISR(pmic_task_id, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

int pmic_read_amux(enum pmic_amux_signal sig, float *result)
{
	int ret = 0;

	if (sig > _PMIC_AMUX_MAX) {
		return -EINVAL;
	}

	ret = hwlock_acquire(HWLOCK_ADC, pdMS_TO_TICKS(100));
	if (ret < 0) {
		return ret;
	}
	ret = hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(100));
	if (ret < 0) {
		return ret;
	}

	/* Select the correct channel for this measurement.  */
	MAX77650_setMUX_SEL(sig);

	/*
	 * According to the datasheet, the voltage will stabilize within 0.3us.
	 * Just to be sure, we'll wait a little longer.  In the meantime,
	 * release the I2C mutex.
	 */
	hwlock_release(HWLOCK_I2C);
	vTaskDelay(pdMS_TO_TICKS(5));
	ret = hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(100));
	if (ret < 0) {
		return ret;
	}

	uint16_t adc_data;
	ADC_StartConvert(ADC_CH_0, 0, 0);
	ADC_GetData(&adc_data);

	/* Turn MUX back to neutral so it does not waste power.  */
	MAX77650_setMUX_SEL(sig);

	/* Convert ADC measurement to SI Volts */
	float adc_voltage = (float)adc_data / 1023.0f * 1.22f;

	/*
	 * Convert value according to PMIC formulas (Table 7)
	 */
	switch (sig) {
	case PMIC_AMUX_CHGIN_U:
		*result = adc_voltage / 0.167f;
		break;
	case PMIC_AMUX_CHGIN_I:
		*result = adc_voltage / 2.632f;
		break;
	case PMIC_AMUX_BATT_U:
		*result = adc_voltage / 0.272f;
		break;
	case PMIC_AMUX_BATT_CHG_I:
		*result = adc_voltage / 1.25f;
		break;
	case PMIC_AMUX_BATT_NULL_I:
	case PMIC_AMUX_THM_U:
	case PMIC_AMUX_TBIAS_U:
	case PMIC_AMUX_AGND_U:
		*result = adc_voltage;
		break;
	case PMIC_AMUX_SYS_U:
		*result = adc_voltage / 0.26f;
		break;
	default:
		ret = -EINVAL;
	}

	hwlock_release(HWLOCK_I2C);
	hwlock_release(HWLOCK_ADC);
	return ret;
}

void vPmicTask(void *pvParameters)
{
	pmic_task_id = xTaskGetCurrentTaskHandle();

	ADC_Init(0x9, NULL);
	GPIO_Config(&gpio_cfg_adc0);

	TickType_t button_start_tick = 0;

	while (1) {
		if (button_start_tick == 0) {
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		} else {
			ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100));
		}

		TickType_t duration = xTaskGetTickCount() - button_start_tick;

		if (button_start_tick != 0 && duration > pdMS_TO_TICKS(1000)) {
			LOG_WARN("pmic", "Poweroff");
			MAX77650_setSFT_RST(0x2);
		}

		while (hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(500)) < 0) {
			LOG_WARN("pmic", "Failed to acquire I2C. Retrying ...");
			vTaskDelay(pdMS_TO_TICKS(500));
		}

		uint8_t int_flag = MAX77650_getINT_GLBL();
		hwlock_release(HWLOCK_I2C);

		if (int_flag & MAX77650_INT_nEN_F) {
			/* Button was pressed */
			button_start_tick = xTaskGetTickCount();
		}
		if (int_flag & MAX77650_INT_nEN_R) {
			/* Button was released */
			button_start_tick = 0;
			if (duration < pdMS_TO_TICKS(400)) {
				return_to_menu();
			} else {
				LOG_WARN("pmic", "Resetting ...");
				card10_reset();
			}
		}

		/* TODO: Remove when all interrupts are handled */
		if (int_flag & ~(MAX77650_INT_nEN_F | MAX77650_INT_nEN_R)) {
			LOG_WARN(
				"pmic",
				"Unhandled PMIC Interrupt: %x",
				int_flag
			);
		}
	}
}
