#include "display.h"
#include "Fonts/fonts.h"
#include "FreeRTOS.h"
#include "LCD_Driver.h"
#include "epicardium.h"
#include "gfx.h"
#include "gpio.h"
#include "task.h"
#include "tmr.h"
#include "tmr_utils.h"

static TaskHandle_t lock = NULL;

static int check_lock()
{
	TaskHandle_t task = xTaskGetCurrentTaskHandle();
	if (task != lock) {
		return -EBUSY;
	} else {
		return 0;
	}
}

int epic_disp_print(
	int16_t posx,
	int16_t posy,
	const char *pString,
	uint16_t fg,
	uint16_t bg
) {
	return epic_disp_print_adv(DISP_FONT20, posx, posy, pString, fg, bg);
}

static const sFONT *font_map[] = {
	[DISP_FONT8] = &Font8,   [DISP_FONT12] = &Font12,
	[DISP_FONT16] = &Font16, [DISP_FONT20] = &Font20,
	[DISP_FONT24] = &Font24,
};

int epic_disp_print_adv(
	uint8_t font,
	int16_t posx,
	int16_t posy,
	const char *pString,
	uint16_t fg,
	uint16_t bg
) {
	int cl = check_lock();
	if (font >= (sizeof(font_map) / sizeof(sFONT *))) {
		return -EINVAL;
	}
	if (cl < 0) {
		return cl;
	} else {
		gfx_puts(
			font_map[font],
			&display_screen,
			posx,
			posy,
			pString,
			fg,
			bg
		);
		return 0;
	}
}

int epic_disp_clear(uint16_t color)
{
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		gfx_clear_to_color(&display_screen, color);
		return 0;
	}
}

int epic_disp_pixel(int16_t x, int16_t y, uint16_t color)
{
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		gfx_setpixel(&display_screen, x, y, color);
		return 0;
	}
}

int epic_disp_line(
	int16_t xstart,
	int16_t ystart,
	int16_t xend,
	int16_t yend,
	uint16_t color,
	enum disp_linestyle linestyle,
	uint16_t pixelsize
) {
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		/* TODO add linestyle support to gfx code */
		gfx_line(
			&display_screen,
			xstart,
			ystart,
			xend,
			yend,
			pixelsize,
			color
		);
		return 0;
	}
}

int epic_disp_rect(
	int16_t xstart,
	int16_t ystart,
	int16_t xend,
	int16_t yend,
	uint16_t color,
	enum disp_fillstyle fillstyle,
	uint16_t pixelsize
) {
	int cl = check_lock();
	if (cl < 0)
		return cl;

	switch (fillstyle) {
	case FILLSTYLE_EMPTY:
		gfx_rectangle(
			&display_screen,
			xstart,
			ystart,
			xend - xstart,
			yend - ystart,
			pixelsize,
			color
		);
		break;
	case FILLSTYLE_FILLED:
		gfx_rectangle_fill(
			&display_screen,
			xstart,
			ystart,
			xend - xstart,
			yend - ystart,
			color
		);
		break;
	}
	return 0;
}

int epic_disp_circ(
	int16_t x,
	int16_t y,
	uint16_t rad,
	uint16_t color,
	enum disp_fillstyle fillstyle,
	uint16_t pixelsize
) {
	int cl = check_lock();
	if (cl < 0)
		return cl;

	switch (fillstyle) {
	case FILLSTYLE_EMPTY:
		gfx_circle(&display_screen, x, y, rad, pixelsize, color);
		break;
	case FILLSTYLE_FILLED:
		gfx_circle_fill(&display_screen, x, y, rad, color);
		break;
	}

	return 0;
}

int epic_disp_update()
{
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	}

	gfx_update(&display_screen);
	return 0;
}

int epic_disp_framebuffer(union disp_framebuffer *fb)
{
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	}

	LCD_Set(fb->raw, sizeof(fb->raw));
	return 0;
}

int epic_disp_backlight(uint16_t brightness)
{
	/* TODO: lock? */
	LCD_SetBacklight(brightness);
	return 0;
}

int epic_disp_open()
{
	TaskHandle_t task = xTaskGetCurrentTaskHandle();
	if (lock == task) {
		return 0;
	} else if (lock == NULL) {
		lock = task;
		return 0;
	} else {
		return -EBUSY;
	}
}

int epic_disp_close()
{
	if (check_lock() < 0 && lock != NULL) {
		return -EBUSY;
	} else {
		lock = NULL;
		return 0;
	}
}

void disp_forcelock()
{
	TaskHandle_t task = xTaskGetCurrentTaskHandle();
	lock              = task;
}
