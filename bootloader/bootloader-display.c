#include "bootloader.h"

#include "GUI_Paint.h"
#include "display.h"

/*
 * Initialize the display.
 */
void bootloader_display_init(void)
{
	;
}

/*
 * Show the bootloader version on the display.
 */
void bootloader_display_header(void)
{
	Paint_Clear(0x0000);
	Paint_DrawString_EN(0, 16 * 0, "Bootloader", &Font16, 0x0000, 0xffff);
	Paint_DrawString_EN(0, 16 * 1, __DATE__, &Font16, 0x0000, 0xffff);
	LCD_Update();
}

/*
 * Display a line of text on the display.
 */
void bootloader_display_line(int line, char *string, uint16_t color)
{
	Paint_DrawString_EN(0, 16 * line, string, &Font16, 0x0000, color);
	LCD_Update();
}
