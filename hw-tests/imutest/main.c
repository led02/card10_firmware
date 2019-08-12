/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "mxc_config.h"
#include "led.h"
#include "board.h"
#include "tmr_utils.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "gpio.h"
#include "bhy_uc_driver.h"
#include "pmic.h"
#include "gfx.h"
#include "display.h"

#include "card10.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define M_PI 3.1415
/***** Definitions *****/

/* should be greater or equal to 69 bytes, page size (50) + maximum packet size(18) + 1 */
#define FIFO_SIZE 300
#define ROTATION_VECTOR_SAMPLE_RATE 10
#define MAX_PACKET_LENGTH 18
#define OUT_BUFFER_SIZE 60

/***** Globals *****/
char out_buffer[OUT_BUFFER_SIZE] =
	" W: 0.999  X: 0.999  Y: 0.999  Z: 0.999   \r";
uint8_t fifo[FIFO_SIZE];

void draw_arrow(int angle, int color)
{
	float sin = sinf(angle * 2 * M_PI / 360.);
	float cos = cosf(angle * 2 * M_PI / 360.);

	int x1 = 160 / 2 + 30;
	int y1 = 80 / 2;

	int x2 = x1 - sin * 30;
	int y2 = y1 - cos * 30;

	gfx_thick_line(&display_screen, x1, y1, x2, y2, 2, color);

	sin = sinf((angle - 140) * 2 * M_PI / 360.);
	cos = cosf((angle - 140) * 2 * M_PI / 360.);

	int x3 = x2 - sin * 10;
	int y3 = y2 - cos * 10;

	gfx_thick_line(&display_screen, x2, y2, x3, y3, 2, color);

	sin = sinf((angle + 140) * 2 * M_PI / 360.);
	cos = cosf((angle + 140) * 2 * M_PI / 360.);

	int x4 = x2 - sin * 10;
	int y4 = y2 - cos * 10;

	gfx_thick_line(&display_screen, x2, y2, x4, y4, 2, color);
}

/***** Functions *****/
static void sensors_callback_orientation(
	bhy_data_generic_t *sensor_data, bhy_virtual_sensor_t sensor_id
) {
	static int prev = -1;
	printf("azimuth=%05d, pitch=%05d, roll=%05d status=%d\n",
	       sensor_data->data_vector.x * 360 / 32768,
	       sensor_data->data_vector.y * 360 / 32768,
	       sensor_data->data_vector.z * 360 / 32768,
	       sensor_data->data_vector.status);

	int angle = sensor_data->data_vector.x * 360 / 32768;

	if (angle != prev) {
		gfx_clear(&display_screen);

		int colors[] = { gfx_color(&display_screen, RED),
				 gfx_color(&display_screen, YELLOW),
				 gfx_color(&display_screen, YELLOW),
				 gfx_color(&display_screen, GREEN) };
		int color    = colors[sensor_data->data_vector.status];
		draw_arrow(sensor_data->data_vector.x * 360 / 32768, color);

		char buf[128];
		//sprintf(buf, "Azimuth: %3d", angle);
		//Paint_DrawString_EN(0, 0, buf, &Font12, BLACK, color);

		sprintf(buf, "%3d", angle);
		gfx_puts(
			&Font24,
			&display_screen,
			0,
			30,
			buf,
			color,
			gfx_color(&display_screen, BLACK)
		);
		gfx_circle(&display_screen, 57, 35, 4, 2, color);

		gfx_update(&display_screen);
		prev = angle;
	}
}

static __attribute__((unused)) void sensors_callback_vector(
	bhy_data_generic_t *sensor_data, bhy_virtual_sensor_t sensor_id
) {
	printf("x=%05d, y=%05d, z=%05d status=%d\n",
	       sensor_data->data_vector.x,
	       sensor_data->data_vector.y,
	       sensor_data->data_vector.z,
	       sensor_data->data_vector.status);
}

static __attribute__((unused)) void sensors_callback_vector_uncalib(
	bhy_data_generic_t *sensor_data, bhy_virtual_sensor_t sensor_id
) {
	printf("x=%05d, y=%05d, z=%05d status=%d\n",
	       sensor_data->data_uncalib_vector.x,
	       sensor_data->data_uncalib_vector.y,
	       sensor_data->data_uncalib_vector.z,
	       sensor_data->data_uncalib_vector.status);
}

/*!
 * @brief This function is  callback function for acquring sensor datas
 *
 * @param[in]   sensor_data
 * @param[in]   sensor_id
 */
static __attribute__((unused)) void sensors_callback_rotation_vector(
	bhy_data_generic_t *sensor_data, bhy_virtual_sensor_t sensor_id
) {
#if 0
    float temp;
    uint8_t index;

    temp = sensor_data->data_quaternion.w / 16384.0f; /* change the data unit by dividing 16384 */
    out_buffer[3] = temp < 0 ? '-' : ' ';
    temp = temp < 0 ? -temp : temp;
    out_buffer[4] = floorf(temp) + '0';

    for (index = 6; index <= 8; index++)
    {
        temp = (temp - floorf(temp)) * 10;
        out_buffer[index] = floorf(temp) + '0';
    }

    temp = sensor_data->data_quaternion.x / 16384.0f;
    out_buffer[13] = temp < 0 ? '-' : ' ';
    temp = temp < 0 ? -temp : temp;
    out_buffer[14] = floorf(temp) + '0';

    for (index = 16; index <= 18; index++)
    {
        temp = (temp - floorf(temp)) * 10;
        out_buffer[index] = floorf(temp) + '0';
    }

    temp = sensor_data->data_quaternion.y / 16384.0f;
    out_buffer[23] = temp < 0 ? '-' : ' ';
    temp = temp < 0 ? -temp : temp;
    out_buffer[24] = floorf(temp) + '0';

    for (index = 26; index <= 28; index++)
    {
        temp = (temp - floorf(temp)) * 10;
        out_buffer[index] = floorf(temp) + '0';
    }

    temp = sensor_data->data_quaternion.z / 16384.0f;
    out_buffer[33] = temp < 0 ? '-' : ' ';
    temp = temp < 0 ? -temp : temp;
    out_buffer[34] = floorf(temp) + '0';

    for (index = 36; index <= 38; index++)
    {
        temp = (temp - floorf(temp)) * 10;
        out_buffer[index] = floorf(temp) + '0';
    }
#endif

	printf("x=%d, y=%d, z=%d, w=%d\n",
	       sensor_data->data_quaternion.x,
	       sensor_data->data_quaternion.y,
	       sensor_data->data_quaternion.z,
	       sensor_data->data_quaternion.w);
}

// *****************************************************************************
int main(void)
{
	/* BHY Variable*/
	uint8_t *fifoptr           = NULL;
	uint8_t bytes_left_in_fifo = 0;
	uint16_t bytes_remaining   = 0;
	uint16_t bytes_read        = 0;
	bhy_data_generic_t fifo_packet;
	bhy_data_type_t packet_type;
	BHY_RETURN_FUNCTION_TYPE result;

	card10_init();
	card10_diag();

#if 0
    /* install the callback function for parse fifo data */
    if(bhy_install_sensor_callback(VS_TYPE_ROTATION_VECTOR, VS_WAKEUP, sensors_callback_rotation_vector))
    {
        printf("Fail to install sensor callback\n");
    }
#endif

	//if(bhy_install_sensor_callback(VS_TYPE_GEOMAGNETIC_FIELD, VS_WAKEUP, sensors_callback_vector))
	//if(bhy_install_sensor_callback(VS_TYPE_GRAVITY, VS_WAKEUP, sensors_callback_vector))
	//if(bhy_install_sensor_callback(VS_TYPE_ACCELEROMETER, VS_WAKEUP, sensors_callback_vector))
	//if(bhy_install_sensor_callback(VS_TYPE_MAGNETIC_FIELD_UNCALIBRATED, VS_WAKEUP, sensors_callback_vector_uncalib))
	if (bhy_install_sensor_callback(
		    VS_TYPE_ORIENTATION,
		    VS_WAKEUP,
		    sensors_callback_orientation)) {
		printf("Fail to install sensor callback\n");
	}

#if 0
    /* enables the virtual sensor */
    if(bhy_enable_virtual_sensor(VS_TYPE_ROTATION_VECTOR, VS_WAKEUP, ROTATION_VECTOR_SAMPLE_RATE, 0, VS_FLUSH_NONE, 0, 0))
    {
        printf("Fail to enable sensor id=%d\n", VS_TYPE_ROTATION_VECTOR);
    }
#endif

	/* enables the virtual sensor */
	//if(bhy_enable_virtual_sensor(VS_TYPE_GEOMAGNETIC_FIELD, VS_WAKEUP, ROTATION_VECTOR_SAMPLE_RATE, 0, VS_FLUSH_NONE, 0, 0))
	//if(bhy_enable_virtual_sensor(VS_TYPE_GRAVITY, VS_WAKEUP, ROTATION_VECTOR_SAMPLE_RATE, 0, VS_FLUSH_NONE, 0, 0))
	//if(bhy_enable_virtual_sensor(VS_TYPE_ACCELEROMETER, VS_WAKEUP, ROTATION_VECTOR_SAMPLE_RATE, 0, VS_FLUSH_NONE, 0, 0))
	//if(bhy_enable_virtual_sensor(VS_TYPE_MAGNETIC_FIELD_UNCALIBRATED, VS_WAKEUP, ROTATION_VECTOR_SAMPLE_RATE, 0, VS_FLUSH_NONE, 0, 0))
	if (bhy_enable_virtual_sensor(
		    VS_TYPE_ORIENTATION,
		    VS_WAKEUP,
		    ROTATION_VECTOR_SAMPLE_RATE,
		    0,
		    VS_FLUSH_NONE,
		    0,
		    0)) {
		printf("Fail to enable sensor id=%d\n",
		       VS_TYPE_GEOMAGNETIC_FIELD);
	}

	while (1) {
		/* wait until the interrupt fires */
		/* unless we already know there are bytes remaining in the fifo */
		while (!GPIO_InGet(&bhi_interrupt_pin) && !bytes_remaining)
			;

		bhy_read_fifo(
			fifo + bytes_left_in_fifo,
			FIFO_SIZE - bytes_left_in_fifo,
			&bytes_read,
			&bytes_remaining
		);
		bytes_read += bytes_left_in_fifo;
		fifoptr     = fifo;
		packet_type = BHY_DATA_TYPE_PADDING;

		do {
			/* this function will call callbacks that are registered */
			result = bhy_parse_next_fifo_packet(
				&fifoptr,
				&bytes_read,
				&fifo_packet,
				&packet_type
			);

			/* prints all the debug packets */
			if (packet_type == BHY_DATA_TYPE_DEBUG) {
				bhy_print_debug_packet(
					&fifo_packet.data_debug, bhy_printf
				);
			}

			/* the logic here is that if doing a partial parsing of the fifo, then we should not parse  */
			/* the last 18 bytes (max length of a packet) so that we don't try to parse an incomplete   */
			/* packet */
		} while ((result == BHY_SUCCESS) &&
			 (bytes_read >
			  (bytes_remaining ? MAX_PACKET_LENGTH : 0)));

		bytes_left_in_fifo = 0;

		if (bytes_remaining) {
			/* shifts the remaining bytes to the beginning of the buffer */
			while (bytes_left_in_fifo < bytes_read) {
				fifo[bytes_left_in_fifo++] = *(fifoptr++);
			}
		}
	}
}
