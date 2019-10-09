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

#define BGR_MASK 0b00000111111000001111100000011111
#define TO_BGR(RGB) (((RGB) | ((RGB) << 16)) & BGR_MASK)
#define TO_RGB(BGR) ((uint16_t)((BGR) | ((BGR) >> 16)))

int epic_disp_blit(
	int16_t pos_x,
	int16_t pos_y,
	int16_t width,
	int16_t height,
	size_t size,
	uint16_t *img,
	uint16_t bg,
	uint8_t *alpha
) {
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		int16_t offset_x = (pos_x < 0) ? -pos_x : 0;
		int16_t count_x = width - offset_x - (pos_x + width >= 160) ? (pos_x + width) - 160 : 0;
		int16_t offset_y = (pos_y < 0) ? -pos_y : 0;
		int16_t count_y = height - offset_y - (pos_y + height >= 80) ? (pos_y + height) - 80 : 0;

		if (alpha == NULL
				&& offset_x == 0
				&& offset_y == 0 && count_x == width 
				&& count_y == height) {
			/* Simply copy full image, no cropping or alpha blending */
			gfx_copy_region(&display_screen, pos_x, pos_y, width, height, GFX_RAW, size, img);
		} else if (alpha == NULL) {
			/* Copy cropped image line by line */
			for (int16_t curr_y = offset_y; curr_y < offset_y + count_y; curr_y++) {
				uint16_t *curr_img = img + (curr_y * width + offset_x);
				gfx_copy_region(&display_screen, pos_x + offset_x, pos_y + curr_y, count_x, 1, GFX_RAW, size, curr_img);
			}
		} else {
			/* Draw alpha blended cropped image pixel by pixel */
			uint32_t bg_bgr = TO_BGR(bg);
			for (int16_t curr_y = offset_y; curr_y < offset_y + count_y; curr_y++) {
				for (int16_t curr_x = offset_x; curr_x < offset_x + count_x; curr_x++) {
					uint16_t fg = img[curr_y * width + curr_x];

					uint32_t fg_bgr = TO_BGR(fg);
					uint8_t alpha_bgr = (alpha[curr_y * width + curr_x] + 4) >> 3;
					uint32_t color_bgr = ((((fg_bgr - bg_bgr) * alpha_bgr) >> 5) + bg_bgr) & BGR_MASK;
					uint_fast16_t color = TO_RGB(color_bgr);
					gfx_setpixel(&display_screen, pos_x + curr_x, pos_y + curr_y, color);
				}
			}
		}

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
