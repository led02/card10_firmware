/*!
  * Copyright (C) 2015 - 2016 Bosch Sensortec GmbH
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  * 
  * Redistributions of source code must retain the above copyright
  * notice, this list of conditions and the following disclaimer.
  * 
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  * 
  * Neither the name of the copyright holder nor the names of the
  * contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  * 
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
  * OR CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
  * OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  * ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
  * 
  * The information provided is believed to be accurate and reliable.
  * The copyright holder assumes no responsibility
  * for the consequences of use
  * of such information nor for any infringement of patents or
  * other rights of third parties which may result from its use.
  * No license is granted by implication or otherwise under any patent or
  * patent rights of the copyright holder.
  */


/********************************************************************************/
/*                                  HEADER FILES                                */
/********************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "asf.h"
#include "task.h"
#include "demo-tasks.h"

#include "bhy.h"
#include "bhy_support.h"
#include "bhy_uc_driver.h"
#include ".\firmware\Bosch_PCB_7183_di03_BMA2x2_Cus-7183_di03.2.1.11703.h"



/********************************************************************************/
/*                                       MACROS                                 */
/********************************************************************************/
/* should be greater or equal to 69 bytes, page size (50) + maximum packet size(18) + 1 */
#define FIFO_SIZE                      300
#define MAX_PACKET_LENGTH              18
#define TICKS_IN_ONE_SECOND            32000.0F

/********************************************************************************/
/*                                GLOBAL VARIABLES                              */
/********************************************************************************/



/********************************************************************************/
/*                                STATIC VARIABLES                              */
/********************************************************************************/
uint8_t fifo[FIFO_SIZE];

static uint32_t bhy_system_timestamp = 0;
static uint32_t bhy_system_timestamp_previous = 0;

struct accel_physical_status_t phy_acc;
struct gyro_physical_status_t phy_gyro;
struct mag_physical_status_t phy_mag;
uint8_t physical_sensor_present_bitmap[8];

/********************************************************************************/
/*                                 FUNCTIONS                                    */
/********************************************************************************/
/*!
 * @brief This function is  callback function for get meta event
 *
 * @param[in]   event_data
 * @param[in]   event_type
 */
static void meta_event_callback(bhy_data_meta_event_t *event_data, bhy_meta_event_type_t event_type)
{
    switch(event_type)
    {
        case BHY_META_EVENT_TYPE_INITIALIZED:
            DEBUG("initialize success!\n");
            break;

        case BHY_META_EVENT_TYPE_SELF_TEST_RESULTS:
            if(event_data->event_specific == BHY_SUCCESS)
                DEBUG("self test result success!  sensor_type=%d \n", event_data->sensor_type);
            else
                DEBUG("self test result fail!  sensor_type=%d  \n", event_data->sensor_type);
            break;
        default:
            DEBUG("unknown meta event\n");
            break;
    }
}

/*!
 * @brief This function is  callback function for acquring sensor datas
 *
 * @param[in]   sensor_data
 * @param[in]   sensor_id
 */
static void sensors_callback(bhy_data_generic_t * sensor_data, bhy_virtual_sensor_t sensor_id)
{
    uint16_t i = 0;
    
    switch(sensor_id)
    {
        case VS_ID_CUS1:
        case VS_ID_CUS1_WAKEUP:
            DEBUG("Cus1 id = %d      len = %d ", sensor_id, bhy_get_cus_evt_size(VS_ID_CUS1));
            for(i = 0; i < (bhy_get_cus_evt_size(VS_ID_CUS1) - 1); i++)
            {
                DEBUG("%2x ", sensor_data->data_custom.data[i]);
            }
            DEBUG("\n");
            break;
        default:
            DEBUG("unknown id = %d\n", sensor_id);
            break;
    }
}

/*!
 * @brief This function is used to run bhy hub
 */
void demo_sensor(void)
{
    /* BHY Variable*/
    uint8_t                    *fifoptr           = NULL;
    uint8_t                    bytes_left_in_fifo = 0;
    uint16_t                   bytes_remaining    = 0;
    uint16_t                   bytes_read         = 0;
    uint32_t                   i                  = 0;
    bhy_data_generic_t         fifo_packet;
    bhy_data_type_t            packet_type;
    BHY_RETURN_FUNCTION_TYPE   result;
    struct sensor_information_non_wakeup_t sensor_info_non_wakeup;
    struct sensor_information_wakeup_t sensor_info_wakeup;

    /* To get the customized version number in firmware, it is necessary to read Parameter Page 2, index 125 */
    /* to get this information. This feature is only supported for customized firmware. To get this customized */
    /* firmware, you need to contact your local FAE of Bosch Sensortec. */
    //struct cus_version_t      bhy_cus_version;

    /* If custom sensor is related to IMU sensor, then the remapping matrix for BHA or BHI here should 
    /* be configured according to its placement on customer's PCB. */
    /* for details, please check 'Application Notes Axes remapping of BHA250(B)/BHI160(B)' document. */
    /* for how to configure remapping matrix, please check example of 'acceleromete_remapping_example.c'. */

    bhy_install_meta_event_callback(BHY_META_EVENT_TYPE_INITIALIZED, meta_event_callback);
    bhy_install_meta_event_callback(BHY_META_EVENT_TYPE_SELF_TEST_RESULTS, meta_event_callback);
    bhy_install_sensor_callback(VS_ID_CUS1, VS_WAKEUP, sensors_callback);

    /* init the bhy chip */
    if(bhy_driver_init(&bhy1_fw))
    {
        DEBUG("Fail to init bhy\n");
    }

    /* wait for the bhy trigger the interrupt pin go down and up again */
    while (ioport_get_pin_level(BHY_INT));
    while (!ioport_get_pin_level(BHY_INT));

    /* To get the customized version number in firmware, it is necessary to read Parameter Page 2, index 125 */
    /* to get this information. This feature is only supported for customized firmware. To get this customized */
    /* firmware, you need to contact your local FAE of Bosch Sensortec. */
    //bhy_read_parameter_page(BHY_PAGE_2, PAGE2_CUS_FIRMWARE_VERSION, (uint8_t*)&bhy_cus_version, sizeof(struct cus_version_t));
    //DEBUG("cus version base:%d major:%d minor:%d\n", bhy_cus_version.base, bhy_cus_version.major, bhy_cus_version.minor);


    /* get physical sensor present from sensor hub */
    bhy_read_parameter_page(BHY_PAGE_1, BHY_PARAM_SYSTEM_PHYSICAL_SENSOR_PRESENT, &physical_sensor_present_bitmap[0], 8);
    DEBUG("Physical Sensor Present:\n");
    for(i = 0; i < 8; i++)
        DEBUG("bitmap[%d] = 0x%x\n", i, physical_sensor_present_bitmap[i]);
    DEBUG("");


    /* get physical sensor status from sensor hub */
    bhy_get_physical_sensor_status(&phy_acc, &phy_gyro, &phy_mag);

	/*	Physical Sensor Status:
		Flags[bit 0]:  interrupt enable
		Flags[bits 5-7]: Sensor Power Mode values:
			0: Sensor Not Present
			1: Power Down
			2: Suspend
			3: Self-Test
			4: Interrupt Motion
			5: One Shot
			6: Low Power Active
			7: Active
	*/
    DEBUG("Physical Sensor Status:\n");
    DEBUG("Acc : sample rate %d, range %d, int %d pwr %d\n",\
        phy_acc.accel_sample_rate, phy_acc.accel_dynamic_range, phy_acc.accel_flag & 0x01, (phy_acc.accel_flag & 0xE0) >> 5);
    DEBUG("Gyro: sample rate %d, range %d, int %d pwr %d\n", \
        phy_gyro.gyro_sample_rate, phy_gyro.gyro_dynamic_range, phy_gyro.gyro_flag & 0x01, (phy_gyro.gyro_flag & 0xE0) >> 5);
    DEBUG("Mag : sample rate %d, range %d, int %d pwr %d\n", \
        phy_mag.mag_sample_rate, phy_mag.mag_dynamic_range, phy_mag.mag_flag & 0x01, (phy_mag.mag_flag & 0xE0) >> 5);
    DEBUG("");


    /* read custom sensor event size from hub for later fifo parse */
    bhy_sync_cus_evt_size();

    DEBUG("cus evt size = %d %d %d %d %d\n", bhy_get_cus_evt_size(VS_ID_CUS1), bhy_get_cus_evt_size(VS_ID_CUS2), \
                               bhy_get_cus_evt_size(VS_ID_CUS3), bhy_get_cus_evt_size(VS_ID_CUS4), \
                               bhy_get_cus_evt_size(VS_ID_CUS5));


    /* get virtual sensor information from sensor hub */
    DEBUG("Supported Virtual Sensor Information:\n");
    for(i = 1; i < 32; i++)
    {
        bhy_get_wakeup_sensor_information(i, &sensor_info_wakeup);

        if(sensor_info_wakeup.wakeup_sensor_type == i)
            DEBUG("id=%2d\n", i);
    }

    for(i = 33; i < 64; i++)
    {
        bhy_get_non_wakeup_sensor_information(i, &sensor_info_non_wakeup);

        if(sensor_info_non_wakeup.non_wakeup_sensor_type == i)
            DEBUG("id=%2d\n", i);
    }
    DEBUG("");


    /* enables the virtual sensor */
    bhy_enable_virtual_sensor(VS_ID_CUS1, VS_WAKEUP, 5, 0, VS_FLUSH_NONE, 0, 0);

    while(1)
    {
        /* wait until the interrupt fires */
        /* unless we already know there are bytes remaining in the fifo */
        while (!ioport_get_pin_level(BHY_INT) && !bytes_remaining);

        bhy_read_fifo(fifo + bytes_left_in_fifo, FIFO_SIZE - bytes_left_in_fifo, &bytes_read, &bytes_remaining);
        
        bytes_read           += bytes_left_in_fifo;
        fifoptr              = fifo;
        packet_type          = BHY_DATA_TYPE_PADDING;

        do
        {
            /* this function will call callbacks that are registered */
            result = bhy_parse_next_fifo_packet(&fifoptr, &bytes_read, &fifo_packet, &packet_type);

            /* prints all the debug packets */
            if (packet_type == BHY_DATA_TYPE_PADDING)
            {
                /* padding data only added at the end of each FIFO dump, discard it. */
                DEBUG(">Padding\n");
            }
            else if (packet_type == BHY_DATA_TYPE_DEBUG)
            {
                trace_log(">DebugString       :");
                bhy_print_debug_packet(&fifo_packet.data_debug, bhy_printf);
                trace_log("\n");
            }

            /* the logic here is that if doing a partial parsing of the fifo, then we should not parse  */
            /* the last 18 bytes (max length of a packet) so that we don't try to parse an incomplete   */
            /* packet */
        } while ((result == BHY_SUCCESS) && (bytes_read > (bytes_remaining ? MAX_PACKET_LENGTH : 0)));

        bytes_left_in_fifo = 0;

        if (bytes_remaining)
        {
            /* shifts the remaining bytes to the beginning of the buffer */
            while (bytes_left_in_fifo < bytes_read)
            {
                fifo[bytes_left_in_fifo++] = *(fifoptr++);
            }
        }
    }
}
/** @}*/
