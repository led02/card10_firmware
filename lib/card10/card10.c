#include "pmic.h"
#include "bosch.h"
#include "display.h"

#include "bhy_uc_driver.h"
#include "Bosch_PCB_7183_di03_BMI160_BMM150-7183_di03.2.1.11696_170103.h"
#include "bme680.h"
#include "bma400.h"
#include "bhy.h"

#include "mxc_config.h"
#include "led.h"
#include "board.h"

#include "tmr_utils.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "gpio.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SPI_SPEED       (15 * 1000 * 1000 * 1)  // Bit Rate. Display has 15 MHz limit

void card10_init(void)
{
    printf("card10 init...\n");

    //Setup the I2CM
    I2C_Shutdown(MXC_I2C0_BUS0);
    I2C_Init(MXC_I2C0_BUS0, I2C_FAST_MODE, NULL);

    I2C_Shutdown(MXC_I2C1_BUS0);
    I2C_Init(MXC_I2C1_BUS0, I2C_FAST_MODE, NULL);

    pmic_init();
    pmic_set_led(0, 0);
    pmic_set_led(1, 0);
    pmic_set_led(2, 0);
    TMR_Delay(MXC_TMR0, MSEC(1000), 0);

    // Enable 32 kHz output
    RTC_SquareWave(MXC_RTC, SQUARE_WAVE_ENABLED, F_32KHZ, NOISE_IMMUNE_MODE, NULL);

    // Enable SPI
    sys_cfg_spi_t spi17y_master_cfg;

    spi17y_master_cfg.map = MAP_A;
    spi17y_master_cfg.ss0 = Enable;
    spi17y_master_cfg.ss1 = Disable;
    spi17y_master_cfg.ss2 = Disable;

    if (SPI_Init(SPI0, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
        printf("Error configuring SPI\n");
        while (1);
    }

    if (SPI_Init(SPI2, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
        printf("Error configuring SPI\n");
        while (1);
    }


    if(bhy_driver_init(bhy1_fw)) {
        printf("Failed to init bhy\n");
    }

    display_init();
}

static uint32_t ecg_read_reg(uint8_t reg)
{
    spi_req_t req;
    uint8_t tx_data[] = {(reg << 1) | 1, 0, 0, 0};
    uint8_t rx_data[] = {0, 0, 0, 0};
    req.tx_data = tx_data;
    req.rx_data = rx_data;
    req.len = 4;
    req.bits = 8;
    req.width = SPI17Y_WIDTH_1;
    req.ssel = 0;
    req.deass = 1;
    req.ssel_pol = SPI17Y_POL_LOW;
    req.tx_num = 0;
    req.rx_num = 0;

    SPI_MasterTrans(SPI0, &req);

    return (rx_data[1] << 16) | (rx_data[2] << 8) | rx_data[3];
}

void card10_diag(void)
{
    uint8_t dummy[1] = {0};
    // "7-bit addresses 0b0000xxx and 0b1111xxx are reserved"
    for (int addr = 0x8; addr < 0x78; ++addr) {
        // A 0 byte write does not seem to work so always send a single byte.
        int res = I2C_MasterWrite(MXC_I2C0_BUS0, addr << 1, dummy, 1, 0);
        if(res == 1) {
            printf("Found (7 bit) address 0x%02x on I2C0\n", addr);
        }

        res = I2C_MasterWrite(MXC_I2C1_BUS0, addr << 1, dummy, 1, 0);
        if(res == 1) {
            printf("Found (7 bit) address 0x%02x on I2C1\n", addr);
        }
    }


    struct bme680_dev gas_sensor;
    gas_sensor.dev_id = BME680_I2C_ADDR_PRIMARY;
    gas_sensor.intf = BME680_I2C_INTF;
    gas_sensor.read = card10_bosch_i2c_read;
    gas_sensor.write = card10_bosch_i2c_write;
    gas_sensor.delay_ms = card10_bosch_delay;
    gas_sensor.amb_temp = 25;

    int8_t rslt = BME680_OK;
    rslt = bme680_init(&gas_sensor);
    if(rslt != BME680_OK) {
        printf("Failed to init BME680\n");
    }

    struct bma400_dev bma;
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


    for(int i=0; i<0x20; i++) {
        uint32_t val = ecg_read_reg(i);
        printf("ECG: %02x: 0x%06x\n", i, val);
    }
}
