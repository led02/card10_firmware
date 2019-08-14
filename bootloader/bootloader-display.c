#include "bootloader.h"
/* Autogenerated */
#include "splash-screen.h"
#include "card10-version.h"

#include "gfx.h"
#include "display.h"

/*
 * "Decompress" splash-screen image.  The algorithm works as follows:
 *
 * Each byte encodes up to 127 pixels in either white or black.  The most
 * significant bit determines the color, the remaining 7 bits determine the
 * amount.
 */
static void bootloader_display_splash(void)
{
	int idx = 0;

	Color white = gfx_color(&display_screen, WHITE);
	Color black = gfx_color(&display_screen, BLACK);
	for (int i = 0; i < sizeof(splash); i++) {
		Color color    = (splash[i] & 0x80) ? white : black;
		uint8_t length = splash[i] & 0x7f;

		for (int j = 0; j < length; j++) {
			uint16_t x = idx % 160;
			uint16_t y = idx / 160;
			gfx_setpixel(&display_screen, x, y, color);
			idx++;
		}
	}

	gfx_update(&display_screen);
}

/*
 * Initialize the display.
 */
void bootloader_display_init(void)
{
	bootloader_display_splash();
}

/*
 * Show the bootloader version on the display.
 */
void bootloader_display_header(void)
{
	gfx_clear(&display_screen);

	Color white = gfx_color(&display_screen, WHITE);
	bootloader_display_line(0, "Bootloader", white);
	bootloader_display_line(1, __DATE__, white);
	bootloader_display_line(2, CARD10_VERSION, white);
}

/*
 * Display a line of text on the display.
 */
void bootloader_display_line(int line, char *string, uint16_t color)
{
	Color black = gfx_color(&display_screen, BLACK);
	gfx_puts(&Font16, &display_screen, 0, 16 * line, string, color, black);
	gfx_update(&display_screen);
}
