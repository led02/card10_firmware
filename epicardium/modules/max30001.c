#include <stdio.h>
#include <string.h>

#include "gpio.h"
#include "pmic.h"
#include "spi.h"

#include "MAX30003.h"

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
#define LOCK_WAIT pdMS_TO_TICKS(MAX30001_MUTEX_WAIT_MS)

/* Interrupt Pin */
static const gpio_cfg_t max30001_interrupt_pin = {
	PORT_1, PIN_12, GPIO_FUNC_IN, GPIO_PAD_PULL_UP
};

static const gpio_cfg_t analog_switch = {
	PORT_0, PIN_31, GPIO_FUNC_OUT, GPIO_PAD_NONE
};

/* clang-format on */

/* MAX30001 Task ID */
static TaskHandle_t max30001_task_id = NULL;

/* MAX30001 Mutex */
static StaticSemaphore_t max30001_mutex_data;
static SemaphoreHandle_t max30001_mutex = NULL;

/* Stream */
static struct stream_info max30001_stream;
;

/* Active */
static bool max30001_sensor_active = false;

static int ecg_enable(int sample_rate, bool enable_internal_pull);
static int ecg_disable(void);

/* -- API -------------------------------------------------------------- {{{ */
int epic_max30001_enable_sensor(struct max30001_sensor_config *config)
{
	int result = 0;

	result = hwlock_acquire(HWLOCK_SPI_ECG, pdMS_TO_TICKS(100));
	if (result < 0) {
		return result;
	}

	if (xSemaphoreTake(max30001_mutex, LOCK_WAIT) != pdTRUE) {
		result = -EBUSY;
		goto out_free_spi;
	}

	struct stream_info *stream = &max30001_stream;
	;
	stream->item_size = sizeof(uint16_t);
	stream->queue =
		xQueueCreate(config->sample_buffer_len, stream->item_size);
	if (stream->queue == NULL) {
		result = -ENOMEM;
		goto out_free_both;
	}

	result = stream_register(SD_MAX30001_ECG, stream);
	if (result < 0) {
		vQueueDelete(stream->queue);
		goto out_free_both;
	}

	result = ecg_enable(config->sample_rate, config->bias);

	if (result < 0) {
		vQueueDelete(stream->queue);
		goto out_free_both;
	}

	if (config->usb) {
		GPIO_OutSet(&analog_switch); // USB
	} else {
		GPIO_OutClr(&analog_switch); // Wrist
	}

	max30001_sensor_active = true;
	result                 = SD_MAX30001_ECG;

out_free_both:
	xSemaphoreGive(max30001_mutex);
out_free_spi:
	hwlock_release(HWLOCK_SPI_ECG);
	return result;
}

int epic_max30001_disable_sensor(void)
{
	int result = 0;

	result = hwlock_acquire(HWLOCK_SPI_ECG, pdMS_TO_TICKS(100));
	if (result < 0) {
		return result;
	}

	if (xSemaphoreTake(max30001_mutex, LOCK_WAIT) != pdTRUE) {
		result = -EBUSY;
		goto out_free_spi;
	}

	struct stream_info *stream = &max30001_stream;
	result                     = stream_deregister(SD_MAX30001_ECG, stream);
	if (result < 0) {
		goto out_free_both;
	}

	vQueueDelete(stream->queue);
	stream->queue = NULL;
	result        = ecg_disable();
	if (result < 0) {
		goto out_free_both;
	}

	max30001_sensor_active = false;

	result = 0;
out_free_both:
	xSemaphoreGive(max30001_mutex);
out_free_spi:
	hwlock_release(HWLOCK_SPI_ECG);
	return result;
}

/* }}} */

/* -- Driver ----------------------------------------------------------- {{{ */
/*
 * Handle a single packet from the FIFO.  For most sensors this means pushing
 * the sample into its sample queue.
 */
static void max30001_handle_samples(int16_t *sensor_data, int16_t n)
{
	if (max30001_stream.queue == NULL) {
		return;
	}

	while (n--) {
		uint16_t data = -*sensor_data++;
		if (xQueueSend(
			    max30001_stream.queue,
			    &data,
			    MAX30001_MUTEX_WAIT_MS) != pdTRUE) {
			LOG_WARN(
				"max30001",
				"queue full"); // TODO; handle queue full
		}
	}
	api_interrupt_trigger(EPIC_INT_MAX30001_ECG);
}

/***** Functions *****/
static uint32_t ecg_read_reg(uint8_t reg)
{
	spi_req_t req;
	uint8_t tx_data[] = { (reg << 1) | 1, 0, 0, 0 };
	uint8_t rx_data[] = { 0, 0, 0, 0 };
	req.tx_data       = tx_data;
	req.rx_data       = rx_data;
	req.len           = 4;
	req.bits          = 8;
	req.width         = SPI17Y_WIDTH_1;
	req.ssel          = 0;
	req.deass         = 1;
	req.ssel_pol      = SPI17Y_POL_LOW;
	req.tx_num        = 0;
	req.rx_num        = 0;

	SPI_MasterTrans(SPI0, &req);

	return (rx_data[1] << 16) | (rx_data[2] << 8) | rx_data[3];
}

static void ecg_write_reg(uint8_t reg, uint32_t data)
{
	//printf("write %02x %06lx\n", reg, data);
	spi_req_t req;
	uint8_t tx_data[] = {
		(reg << 1) | 0, data >> 16, (data >> 8) & 0xFF, data & 0xFF
	};
	uint8_t rx_data[] = { 0, 0, 0, 0 };
	req.tx_data       = tx_data;
	req.rx_data       = rx_data;
	req.len           = 4;
	req.bits          = 8;
	req.width         = SPI17Y_WIDTH_1;
	req.ssel          = 0;
	req.deass         = 1;
	req.ssel_pol      = SPI17Y_POL_LOW;
	req.tx_num        = 0;
	req.rx_num        = 0;

	SPI_MasterTrans(SPI0, &req);
}

static int ecg_enable(int sample_rate, bool enable_internal_pull)
{
	// Reset ECG to clear registers
	ecg_write_reg(SW_RST, 0);

	// General config register setting
	union GeneralConfiguration_u CNFG_GEN_r;
	CNFG_GEN_r.bits.en_ecg = 1; // Enable ECG channel
	if (enable_internal_pull) {
		CNFG_GEN_r.bits.rbiasn =
			1; // Enable resistive bias on negative input
		CNFG_GEN_r.bits.rbiasp =
			1; // Enable resistive bias on positive input
		CNFG_GEN_r.bits.en_rbias = 1; // Enable resistive bias
	} else {
		CNFG_GEN_r.bits.rbiasn =
			0; // Enable resistive bias on negative input
		CNFG_GEN_r.bits.rbiasp =
			0; // Enable resistive bias on positive input
		CNFG_GEN_r.bits.en_rbias = 0; // Enable resistive bias
	}

	CNFG_GEN_r.bits.imag      = 2; // Current magnitude = 10nA
	CNFG_GEN_r.bits.en_dcloff = 1; // Enable DC lead-off detection
	ecg_write_reg(CNFG_GEN, CNFG_GEN_r.all);

	// ECG Config register setting
	union ECGConfiguration_u CNFG_ECG_r;
	CNFG_ECG_r.bits.dlpf = 1; // Digital LPF cutoff = 40Hz
	CNFG_ECG_r.bits.dhpf = 1; // Digital HPF cutoff = 0.5Hz
	//CNFG_ECG_r.bits.gain = 3;       // ECG gain = 160V/V
	CNFG_ECG_r.bits.gain = 0;
	if (sample_rate == 128) {
		CNFG_ECG_r.bits.rate = 2; // Sample rate = 128 sps
	} else if (sample_rate == 256) {
		CNFG_ECG_r.bits.rate = 1; // Sample rate = 256 sps
	} else {
		return -EINVAL;
	}

	ecg_write_reg(CNFG_ECG, CNFG_ECG_r.all);

	//R-to-R configuration
	union RtoR1Configuration_u CNFG_RTOR_r;
	CNFG_RTOR_r.bits.en_rtor = 1; // Enable R-to-R detection
	ecg_write_reg(CNFG_RTOR1, CNFG_RTOR_r.all);

	//Manage interrupts register setting
	union ManageInterrupts_u MNG_INT_r;
	MNG_INT_r.bits.efit      = 0b00011; // Assert EINT w/ 4 unread samples
	MNG_INT_r.bits.clr_rrint = 0b01; // Clear R-to-R on RTOR reg. read back
	ecg_write_reg(MNGR_INT, MNG_INT_r.all);

	//Enable interrupts register setting
	union EnableInterrupts_u EN_INT_r;
	EN_INT_r.all            = 0;
	EN_INT_r.bits.en_eint   = 1; // Enable EINT interrupt
	EN_INT_r.bits.en_rrint  = 0; // Disable R-to-R interrupt
	EN_INT_r.bits.intb_type = 3; // Open-drain NMOS with internal pullup
	ecg_write_reg(EN_INT, EN_INT_r.all);

	//Dyanmic modes config
	union ManageDynamicModes_u MNG_DYN_r;
	MNG_DYN_r.bits.fast = 0; // Fast recovery mode disabled
	ecg_write_reg(MNGR_DYN, MNG_DYN_r.all);

	// MUX Config
	union MuxConfiguration_u CNFG_MUX_r;
	CNFG_MUX_r.bits.openn = 0; // Connect ECGN to AFE channel
	CNFG_MUX_r.bits.openp = 0; // Connect ECGP to AFE channel
	ecg_write_reg(CNFG_EMUX, CNFG_MUX_r.all);

	ecg_write_reg(SYNCH, 0);

	return 0;
}

static int ecg_disable(void)
{
	// TODO
	return 0;
}
/*
 * Fetch all data available from FIFO buffer and handle all data
 * contained in it.
 */
static int max30001_fetch_fifo(void)
{
	int result = 0;

	result = hwlock_acquire(HWLOCK_SPI_ECG, pdMS_TO_TICKS(100));
	if (result < 0) {
		return result;
	}

	if (xSemaphoreTake(max30001_mutex, LOCK_WAIT) != pdTRUE) {
		result = -EBUSY;
		goto out_free_spi;
	}

	uint32_t ecgFIFO, readECGSamples, ETAG[32], status;
	int16_t ecgSample[32];
	const int EINT_STATUS_MASK       = 1 << 23;
	const int FIFO_OVF_MASK          = 0x7;
	const int FIFO_VALID_SAMPLE_MASK = 0x0;
	const int FIFO_FAST_SAMPLE_MASK  = 0x1;
	const int ETAG_BITS_MASK         = 0x7;

	status = ecg_read_reg(STATUS); // Read the STATUS register

	// Check if EINT interrupt asserted
	if ((status & EINT_STATUS_MASK) == EINT_STATUS_MASK) {
		readECGSamples = 0; // Reset sample counter

		do {
			ecgFIFO = ecg_read_reg(ECG_FIFO); // Read FIFO
			ecgSample[readECGSamples] =
				ecgFIFO >> 8; // Isolate voltage data
			ETAG[readECGSamples] =
				(ecgFIFO >> 3) & ETAG_BITS_MASK; // Isolate ETAG
			readECGSamples++; // Increment sample counter

			// Check that sample is not last sample in FIFO
		} while (ETAG[readECGSamples - 1] == FIFO_VALID_SAMPLE_MASK ||
			 ETAG[readECGSamples - 1] == FIFO_FAST_SAMPLE_MASK);

		// Check if FIFO has overflowed
		if (ETAG[readECGSamples - 1] == FIFO_OVF_MASK) {
			ecg_write_reg(FIFO_RST, 0); // Reset FIFO
			LOG_WARN(
				"max30001",
				"fifo overflow"); // TODO; handle fifo full
		}
		max30001_handle_samples(ecgSample, readECGSamples);
	}

	xSemaphoreGive(max30001_mutex);
out_free_spi:
	hwlock_release(HWLOCK_SPI_ECG);
	return result;
}

/*
 * Callback for the MAX30001 interrupt pin.  This callback is called from the
 * SDK's GPIO interrupt driver, in interrupt context.
 */
static void max300001_interrupt_callback(void *_)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (max30001_task_id != NULL) {
		vTaskNotifyGiveFromISR(
			max30001_task_id, &xHigherPriorityTaskWoken
		);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
/* }}} */

void max30001_mutex_init(void)
{
	max30001_mutex = xSemaphoreCreateMutexStatic(&max30001_mutex_data);
}

void vMAX30001Task(void *pvParameters)
{
	max30001_task_id = xTaskGetCurrentTaskHandle();

	int lockret = hwlock_acquire(HWLOCK_SPI_ECG, pdMS_TO_TICKS(100));
	if (lockret < 0) {
		LOG_CRIT("max30001", "Failed to acquire SPI lock!");
		vTaskDelay(portMAX_DELAY);
	}

	/* Take Mutex during initialization, just in case */
	if (xSemaphoreTake(max30001_mutex, 0) != pdTRUE) {
		LOG_CRIT("max30001", "Failed to acquire MAX30001 mutex!");
		vTaskDelay(portMAX_DELAY);
	}

	/* Install interrupt callback */
	GPIO_Config(&max30001_interrupt_pin);
	GPIO_RegisterCallback(
		&max30001_interrupt_pin, max300001_interrupt_callback, NULL
	);
	GPIO_IntConfig(
		&max30001_interrupt_pin, GPIO_INT_EDGE, GPIO_INT_FALLING
	);
	GPIO_IntEnable(&max30001_interrupt_pin);
	NVIC_SetPriority(
		(IRQn_Type)MXC_GPIO_GET_IRQ(max30001_interrupt_pin.port), 2
	);
	NVIC_EnableIRQ(
		(IRQn_Type)MXC_GPIO_GET_IRQ(max30001_interrupt_pin.port)
	);

	GPIO_Config(&analog_switch);
	GPIO_OutClr(&analog_switch); // Wrist

	xSemaphoreGive(max30001_mutex);
	hwlock_release(HWLOCK_SPI_ECG);

	/* ----------------------------------------- */

	while (1) {
		if (max30001_sensor_active) {
			int ret = max30001_fetch_fifo();
			if (ret == -EBUSY) {
				LOG_WARN(
					"max30001", "Could not acquire mutex?"
				);
				continue;
			} else if (ret < 0) {
				LOG_ERR("max30001", "Unknown error: %d", -ret);
			}
		}
		/*
		 * Wait for interrupt.  After two seconds, fetch FIFO anyway
		 *
		 * In the future, reads using epic_stream_read() might also
		 * trigger a FIFO fetch, from outside this task.
		 */
		ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));
	}
}
