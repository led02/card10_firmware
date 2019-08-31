#include "epicardium.h"

#include "py/obj.h"

#include <stdlib.h>
#include <stdio.h>

/* Define the pixel set_all function in this module */
static mp_obj_t mp_ws2812_set_all(mp_obj_t pin, mp_obj_t color_in)
{
	mp_int_t pin_int    = mp_obj_get_int(pin);
	mp_int_t len        = mp_obj_get_int(mp_obj_len(color_in));
	mp_int_t pixels_len = len * 3;
	uint8_t *pixels_arr = alloca(pixels_len * sizeof(uint8_t));

	for (int i = 0; i < len; i++) {
		mp_obj_t color = mp_obj_subscr(
			color_in, mp_obj_new_int(i), MP_OBJ_SENTINEL
		);

		pixels_arr[i * 3]     = mp_obj_get_int(mp_obj_subscr(
                        color, mp_obj_new_int(1), MP_OBJ_SENTINEL)
		);
		pixels_arr[i * 3 + 1] = mp_obj_get_int(mp_obj_subscr(
			color, mp_obj_new_int(0), MP_OBJ_SENTINEL)
		);
		pixels_arr[i * 3 + 2] = mp_obj_get_int(mp_obj_subscr(
			color, mp_obj_new_int(2), MP_OBJ_SENTINEL)
		);
	}

	/* call epicardium to be fast enough */
	epic_ws2812_write(pin_int, pixels_arr, pixels_len);

	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(ws2812_set_all_obj, mp_ws2812_set_all);

/* The globals table for this module */
static const mp_rom_map_elem_t ws2812_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ws2812) },
	{ MP_ROM_QSTR(MP_QSTR_set_all), MP_ROM_PTR(&ws2812_set_all_obj) },
};
static MP_DEFINE_CONST_DICT(ws2812_module_globals, ws2812_module_globals_table);

const mp_obj_module_t ws2812_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&ws2812_module_globals,
};

/* This is a special macro that will make MicroPython aware of this module */
/* clang-format off */
MP_REGISTER_MODULE(MP_QSTR_ws2812, ws2812_module, MODULE_WS2812_ENABLED);
