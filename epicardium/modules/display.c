#include "epicardium.h"
#include "tmr_utils.h"
#include "gpio.h"
#include "GUI_DEV/GUI_Paint.h"
#include "Fonts/fonts.h"
#include "tmr.h"
#include "FreeRTOS.h"
#include "task.h"

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
	uint16_t posx,
	uint16_t posy,
	const char *pString,
	uint16_t fg,
	uint16_t bg
) {
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		Paint_DrawString_EN(posx, posy, pString, &Font20, bg, fg);
		return 0;
	}
}

int epic_disp_clear(uint16_t color)
{
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		LCD_Clear(color);
		return 0;
	}
}

int epic_disp_line(
	uint16_t xstart,
	uint16_t ystart,
	uint16_t xend,
	uint16_t yend,
	uint16_t color,
	enum linestyle_t linestyle,
	uint16_t pixelsize
) {
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		Paint_DrawLine(
			xstart, ystart, xend, yend, color, linestyle, pixelsize
		);
		return 0;
	}
}

int epic_disp_rect(
	uint16_t xstart,
	uint16_t ystart,
	uint16_t xend,
	uint16_t yend,
	uint16_t color,
	enum fillstyle_t fillstyle,
	uint16_t pixelsize
) {
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		Paint_DrawRectangle(
			xstart, ystart, xend, yend, color, fillstyle, pixelsize
		);
		return 0;
	}
}

int epic_disp_circ(
	uint16_t x,
	uint16_t y,
	uint16_t rad,
	uint16_t color,
	enum fillstyle_t fillstyle,
	uint16_t pixelsize
) {
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		Paint_DrawCircle(x, y, rad, color, fillstyle, pixelsize);
		return 0;
	}
}

int epic_disp_update()
{
	int cl = check_lock();
	if (cl < 0) {
		return cl;
	} else {
		LCD_Update();
		return 0;
	}
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
