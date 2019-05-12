#include "i2c.h"
#include "tmr_utils.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define I2C_DEVICE	    MXC_I2C1_BUS0
/********************************************************************************/
/*                         EXTERN FUNCTION DECLARATIONS                         */
/********************************************************************************/

#if 0
static void hexdump(uint8_t *data, uint8_t len)
{
    for(int i=0; i<len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}
#endif

int8_t card10_bosch_i2c_write(uint8_t addr, uint8_t reg, uint8_t *p_buf, uint16_t size)
{
    uint8_t buf[size + 1];

    //printf("sensor_i2c_write 0x%02x:", reg); hexdump(p_buf, size);
    buf[0] = reg;
    memcpy(buf + 1, p_buf, size);

    int l = I2C_MasterWrite(I2C_DEVICE, addr << 1, buf, size + 1, 0);
    //printf("wrote: %d\n", l);
    return l == size + 1 ? 0 : -1;
    //return l == size + 1 ? BHY_SUCCESS : BHY_ERROR;
}

int8_t card10_bosch_i2c_read(uint8_t addr, uint8_t reg, uint8_t *p_buf, uint16_t size)
{
    //printf("sensor_i2c_read 0x%02x %d\n", reg, size);
    if(I2C_MasterWrite(I2C_DEVICE, addr << 1, &reg, 1, 1) == 1) {
        //printf("setup read ok\n");
        int l = I2C_MasterRead(I2C_DEVICE, addr << 1, p_buf, size, 0);
        //printf("read:"); hexdump(p_buf, l);
        return l == size ? 0 : -1;
        //return l == size ? BHY_SUCCESS : BHY_ERROR;

    }
    return -1;
    //return BHY_ERROR;
}

void card10_bosch_delay(uint32_t msec)
{
    TMR_Delay(MXC_TMR0, MSEC(msec), 0);
}

int8_t card10_bosch_i2c_write_ex(void *_, uint8_t addr, uint8_t reg, uint8_t *p_buf, uint16_t size)
{
    return card10_bosch_i2c_write(addr, reg, p_buf, size);
}

int8_t card10_bosch_i2c_read_ex(void *_, uint8_t addr, uint8_t reg, uint8_t *p_buf, uint16_t size)
{
    return card10_bosch_i2c_read(addr, reg, p_buf, size);
}
