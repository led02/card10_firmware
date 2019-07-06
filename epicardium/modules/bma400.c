#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "bma400.h"
#include "bosch.h"
#include "card10.h"

#include "epicardium.h"
#include "modules/log.h"

static bool initialized;
static struct bma400_dev bma400;

#define GRAVITY_EARTH (9.80665f) /* Earth's gravity in m/s^2 */
static float lsb_to_ms2(int16_t val, float g_range, uint8_t bit_width)
{
	float half_scale = (float)(1 << bit_width) / 2.0f;
	return GRAVITY_EARTH * val * g_range / half_scale;
}

static int convert_error(int8_t error)
{
	switch (error) {
	case BMA400_E_NULL_PTR:
		return EFAULT;
	case BMA400_E_COM_FAIL:
		return EIO;
	case BMA400_E_DEV_NOT_FOUND:
		return ENODEV;
	case BMA400_E_INVALID_CONFIG:
		return EINVAL;
	default:
		return 1;
	}
}

int epic_bma400_get_accel(struct acceleration *data)
{
	uint8_t result;

	if (__builtin_expect(!initialized, 0)) {
		bma400.intf_ptr = NULL;
		bma400.delay_ms = card10_bosch_delay;
		bma400.dev_id   = BMA400_I2C_ADDRESS_SDO_LOW;
		bma400.read     = card10_bosch_i2c_read_ex;
		bma400.write    = card10_bosch_i2c_write_ex;
		bma400.intf     = BMA400_I2C_INTF;

		result = bma400_init(&bma400);
		if (result != BMA400_OK) {
			LOG_ERR("bma400", "init error: %d", result);
			return -convert_error(result);
		}

		result = bma400_soft_reset(&bma400);
		if (result != BMA400_OK) {
			LOG_ERR("bma400", "soft_reset error: %d", result);
			return -convert_error(result);
		}

		struct bma400_sensor_conf conf;
		conf.type = BMA400_ACCEL;

		result = bma400_get_sensor_conf(&conf, 1, &bma400);
		if (result != BMA400_OK) {
			LOG_ERR("bma400", "get_sensor_conf error: %d", result);
			return -convert_error(result);
		}

		conf.param.accel.odr      = BMA400_ODR_100HZ;
		conf.param.accel.range    = BMA400_2G_RANGE;
		conf.param.accel.data_src = BMA400_DATA_SRC_ACCEL_FILT_1;

		result = bma400_set_sensor_conf(&conf, 1, &bma400);
		if (result != BMA400_OK) {
			LOG_ERR("bma400", "set_sensor_conf error: %d", result);
			return -convert_error(result);
		}

		result = bma400_set_power_mode(BMA400_LOW_POWER_MODE, &bma400);
		if (result != BMA400_OK) {
			LOG_ERR("bma400", "set_power_mode error: %d", result);
			return -convert_error(result);
		}

		initialized = true;
	}

	struct bma400_sensor_data data_in;
	result = bma400_get_accel_data(
		BMA400_DATA_SENSOR_TIME, &data_in, &bma400
	);
	if (result != BMA400_OK) {
		LOG_ERR("bma400", "get_accel_data error: %d\n", result);
		return -convert_error(result);
	}

	data->x = lsb_to_ms2(data_in.x, 2, 12);
	data->y = lsb_to_ms2(data_in.y, 2, 12);
	data->z = lsb_to_ms2(data_in.z, 2, 12);

	return 0;
}
