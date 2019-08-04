#include "LCD/LCD_Driver.h"
#include "framebuffer.h"
#include "gfx.h"
#include "textbuffer.h"

#include "gpio.h"
#include "portexpander.h"
#include "MAX77650-Arduino-Library.h"

#include <stdint.h>
#include <stdio.h>

/***** Globals *****/
const gpio_cfg_t DEV_DC_PIN = { PORT_1, PIN_6, GPIO_FUNC_OUT, GPIO_PAD_NONE };

struct gfx_region display_screen;
struct txt_buffer display_textb;

/***** Functions *****/
void display_set_reset_pin(uint8_t state)
{
	if (!portexpander_detected()) {
		MAX77650_setDO(state ? true : false);
	} else {
		portexpander_out_put(PIN_4, state);
	}
}

void display_init(void)
{
	if (!portexpander_detected()) {
		// Open-drain
		MAX77650_setDRV(false);
		// Output
		MAX77650_setDIR(false);
	}

	GPIO_Config(&DEV_DC_PIN);

	LCD_SetBacklight(20);
	LCD_Init();

	display_screen = gfx_screen(LCD_framebuffer());
	txt_init(&display_textb, &display_screen, &Font12);
	gfx_clear(&display_screen);
}

void display_init_slim(void)
{
	if (!portexpander_detected()) {
		// Open-drain
		MAX77650_setDRV(false);
		// Output
		MAX77650_setDIR(false);
	}

	GPIO_Config(&DEV_DC_PIN);

	display_screen = gfx_screen(LCD_framebuffer());
	txt_init(&display_textb, &display_screen, &Font12);
}
