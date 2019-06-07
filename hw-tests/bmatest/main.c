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
#include "bma400.h"
#include "bosch.h"

#include "card10.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/
void print_rslt(int8_t rslt)
{
    switch (rslt) {
    case BMA400_OK:
        /* Do nothing */
        break;
    case BMA400_E_NULL_PTR:
        printf("Error [%d] : Null pointer\r\n", rslt);
        break;
    case BMA400_E_COM_FAIL:
        printf("Error [%d] : Communication failure\r\n", rslt);
        break;
    case BMA400_E_DEV_NOT_FOUND:
        printf("Error [%d] : Device not found\r\n", rslt);
        break;
    case BMA400_E_INVALID_CONFIG:
        printf("Error [%d] : Invalid configuration\r\n", rslt);
        break;
    case BMA400_W_SELF_TEST_FAIL:
        printf("Warning [%d] : Self test failed\r\n", rslt);
        break;
    default:
        printf("Error [%d] : Unknown error code\r\n", rslt);
        break;
    }
}
// *****************************************************************************
int main(void)
{
    card10_init();
    card10_diag();

    struct bma400_dev bma;
    struct bma400_int_enable tap_int[2];
    struct bma400_sensor_conf conf[2];
    int8_t rslt;
    uint32_t poll_period = 5, test_dur_ms = 30000;
    uint16_t int_status;


    bma.intf_ptr = NULL; /* To attach your interface device reference */
    bma.delay_ms = card10_bosch_delay;
    bma.dev_id = BMA400_I2C_ADDRESS_SDO_LOW;
    bma.read = card10_bosch_i2c_read_ex;
    bma.write = card10_bosch_i2c_write_ex;
    bma.intf = BMA400_I2C_INTF;

    rslt = bma400_init(&bma);
    if (rslt == BMA400_OK) {
        printf("BMA400 found with chip ID 0x%X\r\n", bma.chip_id);
    }


    print_rslt(rslt);

    rslt = bma400_soft_reset(&bma);
    print_rslt(rslt);

    conf[0].type = BMA400_ACCEL;
    conf[1].type = BMA400_TAP_INT;

    rslt = bma400_get_sensor_conf(conf, 2, &bma);
    print_rslt(rslt);

    conf[0].param.accel.odr = BMA400_ODR_200HZ;
    conf[0].param.accel.range = BMA400_4G_RANGE;
    conf[0].param.accel.data_src = BMA400_DATA_SRC_ACCEL_FILT_1;
    conf[0].param.accel.filt1_bw = BMA400_ACCEL_FILT1_BW_1;

    conf[1].param.tap.int_chan = BMA400_UNMAP_INT_PIN;
    conf[1].param.tap.axes_sel = BMA400_Z_AXIS_EN_TAP;
    conf[1].param.tap.sensitivity = BMA400_TAP_SENSITIVITY_0;

    rslt = bma400_set_sensor_conf(conf, 2, &bma);
    print_rslt(rslt);

    bma.delay_ms(100);

    tap_int[0].type = BMA400_SINGLE_TAP_INT_EN;
    tap_int[0].conf = BMA400_ENABLE;

    tap_int[1].type = BMA400_DOUBLE_TAP_INT_EN;
    tap_int[1].conf = BMA400_ENABLE;

    rslt = bma400_enable_interrupt(tap_int, 2, &bma);
    print_rslt(rslt);

    bma.delay_ms(100);

    rslt = bma400_set_power_mode(BMA400_NORMAL_MODE, &bma);
    print_rslt(rslt);

    bma.delay_ms(100);

    if (rslt == BMA400_OK) {
        printf("Tap configured.\r\n");

        while (test_dur_ms) {
            bma.delay_ms(poll_period);

            rslt = bma400_get_interrupt_status(&int_status, &bma);
            print_rslt(rslt);

            if (int_status & BMA400_S_TAP_INT_ASSERTED) {
                printf("Single tap detected!\r\n");
            }

            if (int_status & BMA400_D_TAP_INT_ASSERTED) {
                printf("Double tap detected!\r\n");
            }

            test_dur_ms -= poll_period;
        }

    }
}
