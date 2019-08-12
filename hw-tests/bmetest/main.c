/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "mxc_config.h"
#include "led.h"
#include "board.h"
#include "tmr_utils.h"
#include "i2c.h"
#include "gpio.h"
#include "bme680.h"
#include "bosch.h"
#include "gfx.h"
#include "framebuffer.h"
#include "display.h"
#include "Fonts/fonts.h"

#include "card10.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/***** Definitions *****/
// *****************************************************************************
int main(void)
{
	card10_init();
	card10_diag();

	struct bme680_dev gas_sensor;
	gas_sensor.dev_id   = BME680_I2C_ADDR_PRIMARY;
	gas_sensor.intf     = BME680_I2C_INTF;
	gas_sensor.read     = card10_bosch_i2c_read;
	gas_sensor.write    = card10_bosch_i2c_write;
	gas_sensor.delay_ms = card10_bosch_delay;
	/* amb_temp can be set to 25 prior to configuring the gas sensor
     * or by performing a few temperature readings without operating the gas sensor.
     */
	gas_sensor.amb_temp = 25;

	int8_t rslt = BME680_OK;
	rslt        = bme680_init(&gas_sensor);

	if (rslt != BME680_OK) {
		printf("Failed to init BME680\n");
	}
	uint8_t set_required_settings;

	/* Set the temperature, pressure and humidity settings */
	gas_sensor.tph_sett.os_hum  = BME680_OS_2X;
	gas_sensor.tph_sett.os_pres = BME680_OS_4X;
	gas_sensor.tph_sett.os_temp = BME680_OS_8X;
	gas_sensor.tph_sett.filter  = BME680_FILTER_SIZE_3;

	/* Set the remaining gas sensor settings and link the heating profile */
	gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
	/* Create a ramp heat waveform in 3 steps */
	gas_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
	gas_sensor.gas_sett.heatr_dur  = 150; /* milliseconds */

	/* Select the power mode */
	/* Must be set before writing the sensor configuration */
	gas_sensor.power_mode = BME680_FORCED_MODE;

	/* Set the required sensor settings needed */
	set_required_settings = BME680_OST_SEL | BME680_OSP_SEL |
				BME680_OSH_SEL | BME680_FILTER_SEL |
				BME680_GAS_SENSOR_SEL;

	/* Set the desired sensor configuration */
	rslt = bme680_set_sensor_settings(set_required_settings, &gas_sensor);

	/* Set the power mode */
	rslt = bme680_set_sensor_mode(&gas_sensor);

	Color white = gfx_color(&display_screen, WHITE);
	Color black = gfx_color(&display_screen, BLACK);
	while (1) {
		TMR_Delay(MXC_TMR0, MSEC(1000), 0);

		struct bme680_field_data data;
		rslt = bme680_get_sensor_data(&data, &gas_sensor);

		printf("T: %.2Lf degC, P: %.2Lf hPa, H %.2Lf %%rH ",
		       data.temperature / 100.0l,
		       data.pressure / 100.0l,
		       data.humidity / 1000.0l);

		char buf[128];
		sprintf(buf, "T: %.2Lf degC", data.temperature / 100.0l);
		gfx_puts(&Font16, &display_screen, 0, 0, buf, white, black);

		sprintf(buf, "P: %.2Lf hPa", data.pressure / 100.0l);
		gfx_puts(&Font16, &display_screen, 0, 16, buf, white, black);

		sprintf(buf, "H: %.2Lf %%rH", data.humidity / 1000.0l);
		gfx_puts(&Font16, &display_screen, 0, 32, buf, white, black);

		//printf("%.2f,%.2f,%.2f\n", data.temperature / 100.0f,
		//    data.pressure / 100.0f, data.humidity / 1000.0f );
		/* Avoid using measurements from an unstable heating setup */
		if (data.status & BME680_GASM_VALID_MSK) {
			printf(", G: %ld ohms", data.gas_resistance);
			sprintf(buf, "G: %ld ohms", data.gas_resistance);
			gfx_puts(
				&Font16,
				&display_screen,
				0,
				48,
				buf,
				white,
				black
			);
		}

		gfx_update(&display_screen);

		printf("\n");

		/* Trigger the next measurement if you would like to read data out continuously */
		if (gas_sensor.power_mode == BME680_FORCED_MODE) {
			rslt = bme680_set_sensor_mode(&gas_sensor);
		}
	}
}
