/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include "DEV_Config.h"
#include "Fonts/fonts.h"
#include "display.h"
#include "gfx.h"
#include "gpio.h"
#include "image/image.h"
#include "tmr.h"
#include "tmr_utils.h"

#include "card10.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// *****************************************************************************
int main(void)
{
	int count = 0;

	card10_init();
	card10_diag();

	Color red    = gfx_color(&display_screen, RED);
	Color green  = gfx_color(&display_screen, GREEN);
	Color yellow = gfx_color(&display_screen, YELLOW);

	gfx_puts(&Font24, &display_screen, 0, 0, "123", 0x000f, 0xfff8);
	gfx_puts(&Font24, &display_screen, 23, 0, "ABC", 0x000f, 0xfff8);
	gfx_rectangle(&display_screen, 70, 10, 100, 40, 2, red);
	gfx_line(&display_screen, 70, 10, 100, 40, 2, green);
	gfx_line(&display_screen, 100, 10, 70, 40, 2, yellow);
	gfx_circle(&display_screen, 85, 25, 22, 2, green);

	gfx_copy_region(
		&display_screen,
		120,
		0,
		40,
		40,
		GFX_RAW,
		40 * 40 * 2,
		(const void *)(gImage_40X40)
	);
	gfx_copy_region(
		&display_screen,
		0,
		0,
		160,
		80,
		GFX_RAW,
		160 * 80 * 2,
		(const void *)(gImage_160X80)
	);
	gfx_update(&display_screen);

	DEV_Delay_ms(3000);

	while (1) {
		TMR_Delay(MXC_TMR0, MSEC(1000), 0);
		printf("count = %d\n", count++);
	}
}
