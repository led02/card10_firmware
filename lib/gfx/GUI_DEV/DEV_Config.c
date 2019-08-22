/*****************************************************************************
* | File        :   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master 
*                and enhance portability
*----------------
* | This version:   V1.0
* | Date        :   2018-11-22
* | Info        :

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "DEV_Config.h"
#include "spi.h"

#define SPI SPI2

//const gpio_cfg_t DEV_CS_PIN = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};
//const gpio_cfg_t DEV_BL_PIN = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};
static spi_req_t req = { .rx_data  = NULL,
			 .bits     = 8,
			 .width    = SPI17Y_WIDTH_1,
			 .ssel     = 0,
			 .deass    = 1,
			 .ssel_pol = SPI17Y_POL_LOW,
			 .tx_num   = 0,
			 .rx_num   = 0 };
#define SPI_SPEED (15 * 1000 * 1000) // Bit Rate. Display has 15 MHz limit

/********************************************************************************/
void lcd_write(uint8_t *data, int size)
{
	sys_cfg_spi_t spi17y_master_cfg;

	spi17y_master_cfg.map = MAP_A;
	spi17y_master_cfg.ss0 = Enable;
	spi17y_master_cfg.ss1 = Disable;
	spi17y_master_cfg.ss2 = Disable;
	if (SPI_Init(SPI2, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
		printf("Error configuring SPI\n");
		while (1)
			;
	}
	req.tx_data = data;
	req.len     = size;
	SPI_MasterTrans(SPI, &req);
}

// Parameters for PWM output
#define PORT_PWM PORT_0    // port
#define PIN_PWM PIN_28     // pin
#define FREQ 1000          // (Hz)
#define PWM_TIMER MXC_TMR4 // must change PORT_PWM and PIN_PWM if changed
void DEV_Set_BL(uint16_t _Value)
{
	// Declare variables
	gpio_cfg_t gpio_pwm;   // to configure GPIO
	tmr_cfg_t tmr;         // to congigure timer
	tmr_pwm_cfg_t tmr_pwm; // for configure PWM
	unsigned int period_ticks = PeripheralClock / FREQ;
	unsigned int duty_ticks   = period_ticks * _Value / 1000;

	// Congfigure GPIO port and pin for PWM
	gpio_pwm.func = GPIO_FUNC_ALT4;
	gpio_pwm.port = PORT_PWM;
	gpio_pwm.mask = PIN_PWM;
	gpio_pwm.pad  = GPIO_PAD_PULL_DOWN;

	GPIO_Config(&gpio_pwm);

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

	tmr.mode    = TMR_MODE_PWM;
	tmr.cmp_cnt = period_ticks;
	tmr.pol     = 0;
	TMR_Config(PWM_TIMER, &tmr);

	tmr_pwm.pol      = 1;
	tmr_pwm.per_cnt  = period_ticks;
	tmr_pwm.duty_cnt = duty_ticks;

	TMR_PWMConfig(PWM_TIMER, &tmr_pwm);
	TMR_Enable(PWM_TIMER);
}
