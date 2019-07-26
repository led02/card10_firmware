#include "LCD/LCD_Driver.h"
#include "GUI_DEV/GUI_Paint.h"

#include "gpio.h"
#include "tmr.h"
#include "portexpander.h"
#include "MAX77650-Arduino-Library.h"

#include <stdint.h>
#include <stdio.h>
/***** Globals *****/
const gpio_cfg_t DEV_DC_PIN = {PORT_1, PIN_6, GPIO_FUNC_OUT, GPIO_PAD_NONE};

// Parameters for PWM output
#define PORT_PWM   PORT_0  // port
#define PIN_PWM    PIN_28   // pin
#define FREQ       200 // (Hz) 
//#define DUTY_CYCLE 75  // (%)  
#define DUTY_CYCLE 20  // (%)  
#define PWM_TIMER  MXC_TMR4  // must change PORT_PWM and PIN_PWM if changed


/***** Functions *****/
void PWM_Output(void)
{
    // Declare variables
    gpio_cfg_t gpio_pwm;    // to configure GPIO
    tmr_cfg_t tmr;          // to congigure timer
    tmr_pwm_cfg_t tmr_pwm;  // for configure PWM
    unsigned int period_ticks = PeripheralClock / FREQ;
    unsigned int duty_ticks = period_ticks * DUTY_CYCLE / 100;

    // Congfigure GPIO port and pin for PWM
    gpio_pwm.func = GPIO_FUNC_ALT4;
    gpio_pwm.port = PORT_PWM;
    gpio_pwm.mask = PIN_PWM; 
    gpio_pwm.pad = GPIO_PAD_PULL_DOWN;

    if (GPIO_Config(&gpio_pwm) != E_NO_ERROR) {
        printf("Failed GPIO_Config for pwm.\n");
    }

    /* 
    Steps for configuring a timer for PWM mode:
    1. Disable the timer
    2. Set the prescale value
    3. Configure the timer for PWM mode
    4. Set polarity, pwm parameters
    5. Enable Timer
    */

    TMR_Disable(PWM_TIMER);

    TMR_Init(PWM_TIMER, TMR_PRES_1, 0);

    tmr.mode = TMR_MODE_PWM;
    tmr.cmp_cnt = period_ticks;
    tmr.pol = 0;
    TMR_Config(PWM_TIMER, &tmr);

    tmr_pwm.pol = 1;
    tmr_pwm.per_cnt = period_ticks;
    tmr_pwm.duty_cnt = duty_ticks;

    if (TMR_PWMConfig(PWM_TIMER, &tmr_pwm) != E_NO_ERROR) {
        printf("Failed TMR_PWMConfig.\n");
    }

    TMR_Enable(PWM_TIMER);

    printf("PWM started.\n");
}

void display_set_reset_pin(uint8_t state)
{
    if(!portexpander_detected()) {
        MAX77650_setDO(state ? true:false);
    } else {
        portexpander_set(4, state);
    }
}

void display_init(void)
{
    if(!portexpander_detected()) {
        // Open-drain
        MAX77650_setDRV(false);
        // Output
        MAX77650_setDIR(false);
    }

    GPIO_Config(&DEV_DC_PIN);

    PWM_Output();
    LCD_SetBacklight(500);
    LCD_Init();
    LCD_Clear(BLACK);

    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT,0,WHITE);

    Paint_Clear(BLACK);
    Paint_SetRotate(180);
}
