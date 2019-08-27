#include "epicardium.h"

#include "py/obj.h"
#include "py/objint.h"
#include "py/objstr.h"
#include "py/runtime.h"

#include <stdio.h>

static uint16_t rgb888_to_rgb565(uint8_t *bytes)
{
	return ((bytes[0] & 0b11111000) << 8) | ((bytes[1] & 0b11111100) << 3) |
	       (bytes[2] >> 3);
}

static uint16_t get_color(mp_obj_t color_in)
{
	if (mp_obj_get_int(mp_obj_len(color_in)) < 3) {
		mp_raise_ValueError("color must have 3 elements");
	}

	uint8_t red = mp_obj_get_int(
		mp_obj_subscr(color_in, mp_obj_new_int(0), MP_OBJ_SENTINEL)
	);
	uint8_t green = mp_obj_get_int(
		mp_obj_subscr(color_in, mp_obj_new_int(1), MP_OBJ_SENTINEL)
	);
	uint8_t blue = mp_obj_get_int(
		mp_obj_subscr(color_in, mp_obj_new_int(2), MP_OBJ_SENTINEL)
	);
	uint8_t colors[3] = { red, green, blue };
	return rgb888_to_rgb565(colors);
}

/* print something on the display */
static mp_obj_t mp_display_print(size_t n_args, const mp_obj_t *args)
{
	if (!mp_obj_is_str_or_bytes(args[0])) {
		mp_raise_TypeError("input text must be a string");
	}
	GET_STR_DATA_LEN(args[0], print, print_len);
	uint32_t posx = mp_obj_get_int(args[1]);
	uint32_t posy = mp_obj_get_int(args[2]);
	uint32_t fg   = get_color(args[3]);
	uint32_t bg   = get_color(args[4]);
	int res = epic_disp_print(posx, posy, (const char *)print, fg, bg);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	display_print_obj, 5, 5, mp_display_print
);

/* print something on the display */
static mp_obj_t mp_display_print_adv(size_t n_args, const mp_obj_t *args)
{
	if (!mp_obj_is_str_or_bytes(args[0])) {
		mp_raise_TypeError("input text must be a string");
	}
	GET_STR_DATA_LEN(args[0], print, print_len);
	uint32_t posx    = mp_obj_get_int(args[1]);
	uint32_t posy    = mp_obj_get_int(args[2]);
	uint32_t fg      = get_color(args[3]);
	uint32_t bg      = get_color(args[4]);
	uint8_t fontName = mp_obj_get_int(args[5]);
	int res          = epic_disp_print_adv(
                fontName, posx, posy, (const char *)print, fg, bg
	);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	display_print_adv_obj, 6, 6, mp_display_print_adv
);

/* draw pixel on the display */
static mp_obj_t mp_display_pixel(size_t n_args, const mp_obj_t *args)
{
	uint16_t x   = mp_obj_get_int(args[0]);
	uint16_t y   = mp_obj_get_int(args[1]);
	uint16_t col = get_color(args[2]);

	//TODO: Move sanity checks to epicardium
	if (x > 160 || x < 0) {
		mp_raise_ValueError("X-Coords have to be 0 < x < 160");
	}

	if (y > 80 || y < 0) {
		mp_raise_ValueError("Y-Coords have to be 0 < y < 80");
	}

	int res = epic_disp_pixel(x, y, col);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	display_pixel_obj, 3, 3, mp_display_pixel
);

/* set display backlight brightness */
static mp_obj_t mp_display_backlight(size_t n_args, const mp_obj_t *args)
{
	uint16_t brightness = mp_obj_get_int(args[0]);

	int res = epic_disp_backlight(brightness);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	display_backlight_obj, 1, 1, mp_display_backlight
);

/* draw line on the display */
static mp_obj_t mp_display_line(size_t n_args, const mp_obj_t *args)
{
	uint16_t xs  = mp_obj_get_int(args[0]);
	uint16_t ys  = mp_obj_get_int(args[1]);
	uint16_t xe  = mp_obj_get_int(args[2]);
	uint16_t ye  = mp_obj_get_int(args[3]);
	uint16_t col = get_color(args[4]);
	uint16_t ls  = mp_obj_get_int(args[5]);
	uint16_t ps  = mp_obj_get_int(args[6]);

	//TODO: Move sanity checks to epicardium
	if (xs > 160 || xs < 0 || xe > 160 || xe < 0) {
		mp_raise_ValueError("X-Coords have to be 0 < x < 160");
	}

	if (ys > 80 || ys < 0 || ye > 80 || ye < 0) {
		mp_raise_ValueError("Y-Coords have to be 0 < x < 80");
	}

	if (ls > 1 || ls < 0) {
		mp_raise_ValueError("Line style has to be 0 or 1");
	}

	int res = epic_disp_line(xs, ys, xe, ye, col, ls, ps);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	display_line_obj, 7, 7, mp_display_line
);

/* draw rectangle on the display */
static mp_obj_t mp_display_rect(size_t n_args, const mp_obj_t *args)
{
	uint16_t xs  = mp_obj_get_int(args[0]);
	uint16_t ys  = mp_obj_get_int(args[1]);
	uint16_t xe  = mp_obj_get_int(args[2]);
	uint16_t ye  = mp_obj_get_int(args[3]);
	uint16_t col = get_color(args[4]);
	uint16_t fs  = mp_obj_get_int(args[5]);
	uint16_t ps  = mp_obj_get_int(args[6]);

	//TODO: Move sanity checks to epicardium
	if (xs > 160 || xs < 0 || xe > 160 || xe < 0) {
		mp_raise_ValueError("X-Coords have to be 0 < x < 160");
	}

	if (ys > 80 || ys < 0 || ye > 80 || ye < 0) {
		mp_raise_ValueError("Y-Coords have to be 0 < x < 80");
	}

	if (fs > 1 || fs < 0) {
		mp_raise_ValueError("Fill style has to be 0 or 1");
	}

	int res = epic_disp_rect(xs, ys, xe, ye, col, fs, ps);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	display_rect_obj, 7, 7, mp_display_rect
);

/* draw rectangle on the display */
static mp_obj_t mp_display_circ(size_t n_args, const mp_obj_t *args)
{
	uint16_t x   = mp_obj_get_int(args[0]);
	uint16_t y   = mp_obj_get_int(args[1]);
	uint16_t rad = mp_obj_get_int(args[2]);
	uint16_t col = get_color(args[3]);
	uint16_t fs  = mp_obj_get_int(args[4]);
	uint16_t ps  = mp_obj_get_int(args[5]);

	if (fs > 1 || fs < 0) {
		mp_raise_ValueError("Fill style has to be 0 or 1");
	}

	int res = epic_disp_circ(x, y, rad, col, fs, ps);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	display_circ_obj, 6, 6, mp_display_circ
);

/* clear the display */
static mp_obj_t mp_display_clear(mp_obj_t col)
{
	uint16_t color = get_color(col);
	int res        = epic_disp_clear(color);
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(display_clear_obj, mp_display_clear);

static mp_obj_t mp_display_update()
{
	int res = epic_disp_update();
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(display_update_obj, mp_display_update);

static mp_obj_t mp_display_open()
{
	int res = epic_disp_open();
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(display_open_obj, mp_display_open);

static mp_obj_t mp_display_close()
{
	int res = epic_disp_close();
	if (res < 0) {
		mp_raise_OSError(-res);
	}
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(display_close_obj, mp_display_close);

/* The globals table for this module */
static const mp_rom_map_elem_t display_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sys_display) },
	{ MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&display_open_obj) },
	{ MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&display_close_obj) },
	{ MP_ROM_QSTR(MP_QSTR_print), MP_ROM_PTR(&display_print_obj) },
	{ MP_ROM_QSTR(MP_QSTR_print_adv), MP_ROM_PTR(&display_print_adv_obj) },
	{ MP_ROM_QSTR(MP_QSTR_pixel), MP_ROM_PTR(&display_pixel_obj) },
	{ MP_ROM_QSTR(MP_QSTR_backlight), MP_ROM_PTR(&display_backlight_obj) },
	{ MP_ROM_QSTR(MP_QSTR_line), MP_ROM_PTR(&display_line_obj) },
	{ MP_ROM_QSTR(MP_QSTR_rect), MP_ROM_PTR(&display_rect_obj) },
	{ MP_ROM_QSTR(MP_QSTR_circ), MP_ROM_PTR(&display_circ_obj) },
	{ MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&display_clear_obj) },
	{ MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&display_update_obj) },
};
static MP_DEFINE_CONST_DICT(
	display_module_globals, display_module_globals_table
);

const mp_obj_module_t display_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&display_module_globals,
};

/* clang-format off */
MP_REGISTER_MODULE(MP_QSTR_sys_display, display_module, MODULE_DISPLAY_ENABLED);
