#include "epicardium.h"
#include "modules/modules.h"
#include "modules/log.h"

#include "card10.h"

#include "bme680.h"
#include "bosch.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define HEATR_TEMP 320
#define HEATR_DUR 150

static bool initialized;
static struct bme680_dev bme;

static int convert_error(int8_t error)
{
	switch (error) {
	case BME680_E_NULL_PTR:
		return EFAULT;
	case BME680_E_COM_FAIL:
		return EIO;
	case BME680_E_DEV_NOT_FOUND:
		return ENODEV;
	case BME680_E_INVALID_LENGTH:
		return EINVAL;
	default:
		return 1;
	}
}

int epic_bme680_init()
{
	int8_t result = BME680_OK;

	if (__builtin_expect(!initialized, 0)) {
		bme.dev_id   = BME680_I2C_ADDR_PRIMARY;
		bme.intf     = BME680_I2C_INTF;
		bme.read     = card10_bosch_i2c_read;
		bme.write    = card10_bosch_i2c_write;
		bme.delay_ms = card10_bosch_delay;

		/*
		 * amb_temp can be set to 25 prior to configuring the gas sensor
		 * or by performing a few temperature readings without operating
		 * the gas sensor.
		 */
		bme.amb_temp = 25;

		result = bme680_init(&bme);
		if (result != BME680_OK) {
			LOG_ERR("bme680", "bme680_init error: %d\n", result);
			return -convert_error(result);
		}

		/*
		 * Select the power mode.  Must be set before writing the sensor
		 * configuration
		 */
		bme.power_mode = BME680_FORCED_MODE;

		/* Set the temperature, pressure and humidity settings */
		bme.tph_sett.os_hum  = BME680_OS_2X;
		bme.tph_sett.os_pres = BME680_OS_4X;
		bme.tph_sett.os_temp = BME680_OS_8X;
		bme.tph_sett.filter  = BME680_FILTER_SIZE_3;

		/* Set the remaining gas sensor settings and link the heating profile */
		bme.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
		/* Create a ramp heat waveform in 3 steps */
		bme.gas_sett.heatr_temp = HEATR_TEMP; /* degree Celsius */
		bme.gas_sett.heatr_dur  = HEATR_DUR;  /* milliseconds */

		/* Set the required sensor settings needed */
		uint16_t settings_sel = BME680_OST_SEL | BME680_OSP_SEL |
					BME680_OSH_SEL | BME680_FILTER_SEL |
					BME680_GAS_SENSOR_SEL;

		result = bme680_set_sensor_settings(settings_sel, &bme);
		if (result != BME680_OK) {
			LOG_ERR("bme680",
				"bme680_set_sensor_settings error: %d\n",
				result);
			return -convert_error(result);
		}

		initialized = true;
	}
	return 0;
}

int epic_bme680_deinit()
{
	if (initialized) {
		int8_t result = bme680_soft_reset(&bme);
		if (result != BME680_OK) {
			LOG_ERR("bme680",
				"bme680_soft_reset error: %d\n",
				result);
			return -convert_error(result);
		}
	}

	initialized = false;
	return 0;
}

int epic_bme680_read_sensors(struct bme680_sensor_data *data)
{
	int8_t result = BME680_OK;

	if (!initialized) {
		LOG_ERR("bme680", "bme680 sensor not initialized");
		return -EINVAL;
	}

	uint16_t profile_dur = 0;
	bme680_get_profile_dur(&profile_dur, &bme);

	result = bme680_set_sensor_mode(&bme); /* Trigger a measurement */
	if (result != BME680_OK) {
		LOG_ERR("bme680", "bme680_set_sensor_mode error: %d\n", result);
		return -convert_error(result);
	}

	vTaskDelay(pdMS_TO_TICKS(
		profile_dur)); /* Wait for the measurement to complete */

	struct bme680_field_data raw_data;
	result = bme680_get_sensor_data(&raw_data, &bme);
	if (result != BME680_OK) {
		LOG_ERR("bme680", "bme680_get_sensor_data error: %d\n", result);
		return -convert_error(result);
	}

	data->temperature    = (float)raw_data.temperature / 100.0f;
	data->humidity       = raw_data.humidity / 1000.0f;
	data->pressure       = raw_data.pressure / 100.0f;
	data->gas_resistance = raw_data.gas_resistance;

	return 0;
}
