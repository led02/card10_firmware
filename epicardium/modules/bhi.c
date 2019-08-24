#include <stdio.h>
#include <string.h>

#include "gpio.h"
#include "bhy_uc_driver.h"
#include "bhy.h"
#include "pmic.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "api/interrupt-sender.h"
#include "epicardium.h"
#include "modules/log.h"
#include "modules/modules.h"
#include "modules/stream.h"

/* Ticks to wait when trying to acquire lock */
#define LOCK_WAIT pdMS_TO_TICKS(BHI160_MUTEX_WAIT_MS)

/* BHI160 Firmware Blob.  Contents are defined in libcard10. */
extern uint8_t bhy1_fw[];

/* Interrupt Pin */
static const gpio_cfg_t bhi160_interrupt_pin = {
	PORT_0, PIN_13, GPIO_FUNC_IN, GPIO_PAD_PULL_UP
};

/* Axis remapping matrices */
static int8_t bhi160_mapping_matrix[3 * 3] = { 0, -1, 0, 1, 0, 0, 0, 0, 1 };
static int8_t bmm150_mapping_matrix[3 * 3] = { -1, 0, 0, 0, 1, 0, 0, 0, -1 };

/*
 * From the official docs:
 *
 *    The sic matrix should be calculated for customer platform by logging
 *    uncalibrated magnetometer data.  The sic matrix here is only an example
 *    array (identity matrix). Customer should generate their own matrix.  This
 *    affects magnetometer fusion performance.
 *
 * TODO: Get data for card10
 */
/* clang-format off */
static float bhi160_sic_array[3 * 3] = { 1.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0,
                                         0.0, 0.0, 1.0 };
/* clang-format on */

/* BHI160 Fifo */
static uint8_t bhi160_fifo[BHI160_FIFO_SIZE];
static size_t start_index = 0;

/* BHI160 Task ID */
static TaskHandle_t bhi160_task_id = NULL;

/* BHI160 Mutex */
static StaticSemaphore_t bhi160_mutex_data;
static SemaphoreHandle_t bhi160_mutex = NULL;

/* Streams */
static struct stream_info bhi160_streams[10];

/* Active */
static bool bhi160_sensor_active[10] = { 0 };

/* -- Utilities -------------------------------------------------------- {{{ */
/*
 * Retrieve the data size for a sensor.  This value is needed for the creation
 * of the sensor's sample queue.
 */
static size_t bhi160_lookup_data_size(enum bhi160_sensor_type type)
{
	switch (type) {
	case BHI160_ACCELEROMETER:
	case BHI160_MAGNETOMETER:
	case BHI160_ORIENTATION:
	case BHI160_GYROSCOPE:
		return sizeof(struct bhi160_data_vector);
	default:
		return 0;
	}
}

/*
 * Map a sensor type to the virtual sensor ID used by BHy1.
 */
static bhy_virtual_sensor_t bhi160_lookup_vs_id(enum bhi160_sensor_type type)
{
	switch (type) {
	case BHI160_ACCELEROMETER:
		return VS_ID_ACCELEROMETER;
	case BHI160_ORIENTATION:
		return VS_ID_ORIENTATION;
	case BHI160_GYROSCOPE:
		return VS_ID_GYROSCOPE;
	default:
		return -1;
	}
}

/*
 * Map a sensor type to its stream descriptor.
 */
static int bhi160_lookup_sd(enum bhi160_sensor_type type)
{
	switch (type) {
	case BHI160_ACCELEROMETER:
		return SD_BHI160_ACCELEROMETER;
	case BHI160_ORIENTATION:
		return SD_BHI160_ORIENTATION;
	case BHI160_GYROSCOPE:
		return SD_BHI160_GYROSCOPE;
	default:
		return -1;
	}
}
/* }}} */

/* -- API -------------------------------------------------------------- {{{ */
int epic_bhi160_enable_sensor(
	enum bhi160_sensor_type sensor_type,
	struct bhi160_sensor_config *config
) {
	int result = 0;

	bhy_virtual_sensor_t vs_id = bhi160_lookup_vs_id(sensor_type);
	if (vs_id < 0) {
		return -ENODEV;
	}

	result = hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(100));
	if (result < 0) {
		return result;
	}

	if (xSemaphoreTake(bhi160_mutex, LOCK_WAIT) != pdTRUE) {
		result = -EBUSY;
		goto out_free_i2c;
	}

	struct stream_info *stream = &bhi160_streams[sensor_type];
	stream->item_size          = bhi160_lookup_data_size(sensor_type);
	/* TODO: Sanity check length */
	stream->queue =
		xQueueCreate(config->sample_buffer_len, stream->item_size);
	if (stream->queue == NULL) {
		result = -ENOMEM;
		goto out_free_both;
	}

	result = stream_register(bhi160_lookup_sd(sensor_type), stream);
	if (result < 0) {
		goto out_free_both;
	}

	result = bhy_enable_virtual_sensor(
		vs_id,
		VS_WAKEUP,
		config->sample_rate,
		0,
		VS_FLUSH_NONE,
		0,
		config->dynamic_range /* dynamic range is sensor dependent */
	);
	if (result != BHY_SUCCESS) {
		goto out_free_both;
	}

	bhi160_sensor_active[sensor_type] = true;
	result                            = bhi160_lookup_sd(sensor_type);

out_free_both:
	xSemaphoreGive(bhi160_mutex);
out_free_i2c:
	hwlock_release(HWLOCK_I2C);
	return result;
}

int epic_bhi160_disable_sensor(enum bhi160_sensor_type sensor_type)
{
	int result = 0;

	bhy_virtual_sensor_t vs_id = bhi160_lookup_vs_id(sensor_type);
	if (vs_id < 0) {
		return -ENODEV;
	}

	result = hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(100));
	if (result < 0) {
		return result;
	}

	if (xSemaphoreTake(bhi160_mutex, LOCK_WAIT) != pdTRUE) {
		result = -EBUSY;
		goto out_free_i2c;
	}

	struct stream_info *stream = &bhi160_streams[sensor_type];
	result = stream_deregister(bhi160_lookup_sd(sensor_type), stream);
	if (result < 0) {
		goto out_free_both;
	}

	vQueueDelete(stream->queue);
	stream->queue = NULL;
	result        = bhy_disable_virtual_sensor(vs_id, VS_WAKEUP);
	if (result < 0) {
		goto out_free_both;
	}

	bhi160_sensor_active[sensor_type] = false;

	result = 0;
out_free_both:
	xSemaphoreGive(bhi160_mutex);
out_free_i2c:
	hwlock_release(HWLOCK_I2C);
	return result;
}

void epic_bhi160_disable_all_sensors()
{
	for (int i = 0; i < sizeof(bhi160_sensor_active); i++) {
		if (bhi160_sensor_active[i]) {
			epic_bhi160_disable_sensor(i);
		}
	}
}

/* }}} */

/* -- Driver ----------------------------------------------------------- {{{ */
/*
 * Handle a single packet from the FIFO.  For most sensors this means pushing
 * the sample into its sample queue.
 */
static void
bhi160_handle_packet(bhy_data_type_t data_type, bhy_data_generic_t *sensor_data)
{
	uint8_t sensor_id = sensor_data->data_vector.sensor_id;
	struct bhi160_data_vector data_vector;
	/*
	 * Timestamp of the next samples, counting at 32 kHz.
	 * Currently unused.
	 */
	static uint32_t timestamp           = 0;
	enum bhi160_sensor_type sensor_type = 0;
	int epic_int                        = 0;
	bool wakeup                         = false;

	switch (sensor_id) {
	case VS_ID_TIMESTAMP_MSW_WAKEUP:
		wakeup = true;
		/* fall through */
	case VS_ID_TIMESTAMP_MSW:
		MXC_ASSERT(data_type == BHY_DATA_TYPE_SCALAR_U16);
		timestamp = sensor_data->data_scalar_u16.data << 16;
		break;
	case VS_ID_TIMESTAMP_LSW_WAKEUP:
		wakeup = true;
		/* fall through */
	case VS_ID_TIMESTAMP_LSW:
		MXC_ASSERT(data_type == BHY_DATA_TYPE_SCALAR_U16);
		timestamp = (timestamp & 0xFFFF0000) |
			    sensor_data->data_scalar_u16.data;
		break;
	case VS_ID_ACCELEROMETER_WAKEUP:
	case VS_ID_ORIENTATION_WAKEUP:
	case VS_ID_GYROSCOPE_WAKEUP:
		wakeup = true;
		/* fall through */
	case VS_ID_ACCELEROMETER:
	case VS_ID_ORIENTATION:
	case VS_ID_GYROSCOPE:
		switch (sensor_id) {
		case VS_ID_ACCELEROMETER_WAKEUP:
		case VS_ID_ACCELEROMETER:
			sensor_type = BHI160_ACCELEROMETER;
			epic_int    = EPIC_INT_BHI160_ACCELEROMETER;
			break;
		case VS_ID_ORIENTATION_WAKEUP:
		case VS_ID_ORIENTATION:
			sensor_type = BHI160_ORIENTATION;
			epic_int    = EPIC_INT_BHI160_ORIENTATION;
			break;
		case VS_ID_GYROSCOPE_WAKEUP:
		case VS_ID_GYROSCOPE:
			sensor_type = BHI160_GYROSCOPE;
			epic_int    = EPIC_INT_BHI160_GYROSCOPE;
			break;
		}

		MXC_ASSERT(data_type == BHY_DATA_TYPE_VECTOR);
		if (bhi160_streams[sensor_type].queue == NULL) {
			break;
		}
		data_vector.data_type = BHI160_DATA_TYPE_VECTOR;
		data_vector.x         = sensor_data->data_vector.x;
		data_vector.y         = sensor_data->data_vector.y;
		data_vector.z         = sensor_data->data_vector.z;
		data_vector.status    = sensor_data->data_vector.status;
		xQueueSend(
			bhi160_streams[sensor_type].queue,
			&data_vector,
			BHI160_MUTEX_WAIT_MS
		);
		if (wakeup) {
			api_interrupt_trigger(epic_int);
		}
		break;
	default:
		break;
	}
}

/*
 * Fetch all data available from BHI160's FIFO buffer and handle all packets
 * contained in it.
 */
static int bhi160_fetch_fifo(void)
{
	/*
	 * Warning:  The code from the BHy1 docs has some issues.  This
	 * implementation looks similar, but has a few important differences.
	 * You'll probably be best of leaving it as it is ...
	 */

	int result = 0;
	/* Number of bytes left in BHI160's FIFO buffer */
	uint16_t bytes_left_in_fifo = 1;

	result = hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(100));
	if (result < 0) {
		return result;
	}

	if (xSemaphoreTake(bhi160_mutex, LOCK_WAIT) != pdTRUE) {
		result = -EBUSY;
		goto out_free_i2c;
	}

	while (bytes_left_in_fifo) {
		/* Fill local FIFO buffer with as many bytes as possible */
		uint16_t bytes_read;
		bhy_read_fifo(
			&bhi160_fifo[start_index],
			BHI160_FIFO_SIZE - start_index,
			&bytes_read,
			&bytes_left_in_fifo
		);

		/* Add the bytes left from the last transfer on top */
		bytes_read += start_index;

		/* Handle all full packets received in this transfer */
		uint8_t *fifo_ptr   = bhi160_fifo;
		uint16_t bytes_left = bytes_read;
		while (bytes_left > 0) {
			bhy_data_generic_t sensor_data;
			bhy_data_type_t data_type;
			result = bhy_parse_next_fifo_packet(
				&fifo_ptr,
				&bytes_left,
				&sensor_data,
				&data_type
			);

			if (result == BHY_SUCCESS) {
				bhi160_handle_packet(data_type, &sensor_data);
			} else {
				break;
			}
		}

		/* Shift the remaining bytes to the beginning */
		for (int i = 0; i < bytes_left; i++) {
			bhi160_fifo[i] =
				bhi160_fifo[bytes_read - bytes_left + i];
		}
		start_index = bytes_left;
	}

	xSemaphoreGive(bhi160_mutex);
out_free_i2c:
	hwlock_release(HWLOCK_I2C);
	return result;
}

/*
 * Callback for the BHI160 interrupt pin.  This callback is called from the
 * SDK's GPIO interrupt driver, in interrupt context.
 */
static void bhi160_interrupt_callback(void *_)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (bhi160_task_id != NULL) {
		vTaskNotifyGiveFromISR(
			bhi160_task_id, &xHigherPriorityTaskWoken
		);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
/* }}} */

void vBhi160Task(void *pvParameters)
{
	int ret;

	bhi160_task_id = xTaskGetCurrentTaskHandle();
	bhi160_mutex   = xSemaphoreCreateMutexStatic(&bhi160_mutex_data);

	/*
	 * Wait a little before initializing BHI160.
	 */
	vTaskDelay(pdMS_TO_TICKS(3));

	int lockret = hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(100));
	if (lockret < 0) {
		LOG_CRIT("bhi160", "Failed to acquire I2C lock!");
		vTaskDelay(portMAX_DELAY);
	}

	/* Take Mutex during initialization, just in case */
	if (xSemaphoreTake(bhi160_mutex, 0) != pdTRUE) {
		LOG_CRIT("bhi160", "Failed to acquire BHI160 mutex!");
		vTaskDelay(portMAX_DELAY);
	}

	memset(bhi160_streams, 0x00, sizeof(bhi160_streams));

	/* Install interrupt callback */
	GPIO_Config(&bhi160_interrupt_pin);
	GPIO_RegisterCallback(
		&bhi160_interrupt_pin, bhi160_interrupt_callback, NULL
	);
	GPIO_IntConfig(&bhi160_interrupt_pin, GPIO_INT_EDGE, GPIO_INT_RISING);
	GPIO_IntEnable(&bhi160_interrupt_pin);
	NVIC_SetPriority(
		(IRQn_Type)MXC_GPIO_GET_IRQ(bhi160_interrupt_pin.port), 2
	);
	NVIC_EnableIRQ((IRQn_Type)MXC_GPIO_GET_IRQ(bhi160_interrupt_pin.port));

	/* Upload firmware */
	ret = bhy_driver_init(bhy1_fw);
	if (ret) {
		LOG_CRIT("bhi160", "BHy1 init failed!");
		vTaskDelay(portMAX_DELAY);
	}

	/* Wait for first interrupt */
	hwlock_release(HWLOCK_I2C);
	ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100));
	lockret = hwlock_acquire(HWLOCK_I2C, pdMS_TO_TICKS(100));
	if (lockret < 0) {
		LOG_CRIT("bhi160", "Failed to acquire I2C lock!");
		vTaskDelay(portMAX_DELAY);
	}

	/* Remap axes to match card10 layout */
	bhy_mapping_matrix_set(
		PHYSICAL_SENSOR_INDEX_ACC, bhi160_mapping_matrix
	);
	bhy_mapping_matrix_set(
		PHYSICAL_SENSOR_INDEX_MAG, bmm150_mapping_matrix
	);
	bhy_mapping_matrix_set(
		PHYSICAL_SENSOR_INDEX_GYRO, bhi160_mapping_matrix
	);

	/* Set "SIC" matrix.  TODO: Find out what this is about */
	bhy_set_sic_matrix(bhi160_sic_array);

	xSemaphoreGive(bhi160_mutex);
	hwlock_release(HWLOCK_I2C);

	/* ----------------------------------------- */

	while (1) {
		int ret = bhi160_fetch_fifo();
		if (ret == -EBUSY) {
			LOG_WARN("bhi160", "Could not acquire mutex for FIFO?");
			continue;
		} else if (ret < 0) {
			LOG_ERR("bhi160", "Unknown error: %d", -ret);
		}

		/*
		 * Wait for interrupt.  After two seconds, fetch FIFO anyway in
		 * case there are any diagnostics or errors.
		 *
		 * In the future, reads using epic_stream_read() might also
		 * trigger a FIFO fetch, from outside this task.
		 */
		ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));
	}
}
