#include "portexpander.h"

#include "i2c.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void portexpander_init(void)
{
    uint8_t addr = 0x21;

    // Enable pull-ups for buttons
    uint8_t command[] = {0x43, 0x68};
    I2C_MasterWrite(MXC_I2C1_BUS0, addr << 1, command, 2, 0);

    // Set _all_ outputs to open-drain to support the high side p-channel transistors.
    command[0] = 0x4F; command[1] = 0x01;
    I2C_MasterWrite(MXC_I2C1_BUS0, addr << 1, command, 2, 0);

    // Enable outputs for transistors and the LED
    command[0] = 0x03; command[1] = 0x68;
    I2C_MasterWrite(MXC_I2C1_BUS0, addr << 1, command, 2, 0);

    // Set outputs to high (i.e. open-drain)
    command[0] = 0x01; command[1] = 0x97;
    I2C_MasterWrite(MXC_I2C1_BUS0, addr << 1, command, 2, 0);
}

uint8_t portexpander_get(void)
{
    uint8_t addr = 0x21;
    uint8_t command[] = {0x00};
    I2C_MasterWrite(MXC_I2C1_BUS0, addr << 1, command, 1, 1);
    uint8_t buf;
    I2C_MasterRead(MXC_I2C1_BUS0, addr << 1, &buf, 1, 0);
    return buf;
}


