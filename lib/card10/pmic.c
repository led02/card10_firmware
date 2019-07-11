#include "i2c.h"
#include "pmic.h"
#include "MAX77650-Arduino-Library.h"
#include <stdint.h>
#include <stdio.h>

static const gpio_cfg_t pmic_interrupt_pin = {
    PORT_0, PIN_12, GPIO_FUNC_IN, GPIO_PAD_PULL_UP
};
static pmic_button_callback_fn pmic_button_callback = NULL;
static volatile bool interrupt_pending;

void pmic_init(void)
{
    uint8_t didm = MAX77650_getDIDM();
    uint8_t cid = MAX77650_getChipID();
    interrupt_pending = false;

    printf("MAX7765x DIDM: 0x%02x CID: 0x%02x\n", didm, cid);

    MAX77650_setIP_SBB0(0b11);  //Limit switch current of SBB0 to 500mA for noise reduction
    //MAX77650_setIP_SBB0(0b00);  //Limit switch current of SBB0 to 1000 mA
    MAX77650_setTV_SBB0(0b101000); //Set output Voltage of SBB0 to 1.8V
    MAX77650_setADE_SBB0(0b0); //Disable Active Discharge at SBB0 Output
    MAX77650_setEN_SBB0(0b110); //Enable SBB0 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

    //MAX77650_setIP_SBB1(0b11);  //Limit output of SBB1 to 500mA
#if BOARD_EVKIT
    MAX77650_setTV_SBB1(0b100000); //Set output Voltage of SBB1 to 1.2V
#else
    MAX77650_setTV_SBB1(0b001001); //Set output Voltage of SBB1 to 3.3V
#endif
    MAX77650_setADE_SBB1(0b0); //Disable Active Discharge at SBB1 Output
    MAX77650_setEN_SBB1(0b110); //Enable SBB1 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

    MAX77650_setIP_SBB2(0b11);  //Limit switch current of SBB2 to 500mA for noise reduction
#if BOARD_EVKIT
    MAX77650_setTV_SBB2(0b110010); //Set output Voltage of SBB2 to 3.3V
#else
    //MAX77650_setTV_SBB2(0b110100); //Set output Voltage of SBB2 to 5.0V
    MAX77650_setTV_SBB2(0b010010); //Set output Voltage of SBB2 to 3.3V
#endif
    MAX77650_setADE_SBB2(0b0); //Disable Active Discharge at SBB2 Output
    MAX77650_setEN_SBB2(0b110); //Enable SBB2 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

    // Prepare the PMIC LEDs
    MAX77650_setLED_FS0(0b11);
    MAX77650_setINV_LED0(false);    //LED red: phase operation
    MAX77650_setBRT_LED0(0b00000);  //LED red: brightness
    MAX77650_setP_LED0(0b1111);     //LED red: LED period
    MAX77650_setD_LED0(0b1111);     //LED red: LED duty-cycle

    MAX77650_setLED_FS1(0b11);
    MAX77650_setINV_LED1(false);    //LED green: phase operation
    MAX77650_setBRT_LED1(0b00000);  //LED green: brightness
    MAX77650_setP_LED1(0b1111);     //LED green: LED period
    MAX77650_setD_LED1(0b1111);     //LED green: LED duty-cycle

    MAX77650_setLED_FS2(0b11);
    MAX77650_setINV_LED2(false);    //LED blue: phase operation
    MAX77650_setBRT_LED2(0b00000);  //LED blue: brightness
    MAX77650_setP_LED2(0b1111);     //LED blue: LED period
    MAX77650_setD_LED2(0b1111);     //LED blue: LED duty-cycle

    MAX77650_setEN_LED_MSTR(true);  //LEDs Master enable

    MAX77650_setCHG_CV(0b11000);   // 4.2 V target battery voltage
    MAX77650_setICHGIN_LIM(1);      // 190 mA limit on USB
    MAX77650_setCHG_CC(0b1011);     // 90 mA fast charge current
    MAX77650_setCHG_EN(1);          // Turn on charger

    MAX77650_setVSYS_REG(0b11000);  // Set VSYS to 4.7 V to reduce voltage across bypass diode

    /* Setup interrupt & callback */
    GPIO_Config(&pmic_interrupt_pin);
    GPIO_RegisterCallback(&pmic_interrupt_pin, pmic_interrupt_callback, NULL);
    GPIO_IntConfig(&pmic_interrupt_pin, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(&pmic_interrupt_pin);
    NVIC_SetPriority((IRQn_Type)MXC_GPIO_GET_IRQ(pmic_interrupt_pin.port), 2);
    NVIC_EnableIRQ((IRQn_Type)MXC_GPIO_GET_IRQ(pmic_interrupt_pin.port));

    /* Setup power button interrupt */
    MAX77650_setINT_M_GLBL(~(MAX77650_INT_nEN_R | MAX77650_INT_nEN_F));
    /* Clear existing interrupts */
    MAX77650_getINT_GLBL();
}

__attribute__((weak)) void pmic_interrupt_callback(void*_)
{
    interrupt_pending = true;
}

void pmic_poll(void)
{
    if(interrupt_pending) {
        /* There might be a race condition here. Don't care ATM. */
        interrupt_pending = false;
        uint8_t int_flag = MAX77650_getINT_GLBL();

        if (int_flag & (MAX77650_INT_nEN_R | MAX77650_INT_nEN_F)) {
            if (pmic_button_callback != NULL) {
                (*pmic_button_callback)(int_flag & MAX77650_INT_nEN_F);
            }
        }
        /* TODO: Other pmic interrupts */
    }
}

void pmic_set_button_callback(pmic_button_callback_fn cb)
{
    pmic_button_callback = cb;
}

void pmic_set_led(uint8_t led, uint8_t val)
{
    if(led == 0) {
        MAX77650_setLED_FS0(val > 0 ? 0b01 : 0);
        MAX77650_setBRT_LED0(val);
    }
    if(led == 1) {
        MAX77650_setLED_FS1(val > 0 ? 0b01 : 0);
        MAX77650_setBRT_LED1(val);
    }
    if(led == 2) {
        MAX77650_setLED_FS2(val > 0 ? 0b01 : 0);
        MAX77650_setBRT_LED2(val);
    }

}
