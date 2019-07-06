#include "py/obj.h"
#include "py/objlist.h"
#include "py/runtime.h"

#include "epicardium.h"

static mp_obj_t mp_leds_set(mp_obj_t led_in, mp_obj_t color_in)
{
	int led = mp_obj_get_int(led_in);

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

	epic_leds_set(led, red, green, blue);

	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(leds_set_obj, mp_leds_set);

static const mp_rom_map_elem_t leds_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_leds) },
	{ MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&leds_set_obj) },
	{ MP_ROM_QSTR(MP_QSTR_BOTTOM_RIGHT), MP_OBJ_NEW_SMALL_INT(11) },
	{ MP_ROM_QSTR(MP_QSTR_BOTTOM_LEFT), MP_OBJ_NEW_SMALL_INT(12) },
	{ MP_ROM_QSTR(MP_QSTR_TOP_RIGHT), MP_OBJ_NEW_SMALL_INT(13) },
	{ MP_ROM_QSTR(MP_QSTR_TOP_LEFT), MP_OBJ_NEW_SMALL_INT(14) },
};
static MP_DEFINE_CONST_DICT(leds_module_globals, leds_module_globals_table);

const mp_obj_module_t leds_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&leds_module_globals,
};

/* Register the module to make it available in Python */
MP_REGISTER_MODULE(MP_QSTR_leds, leds_module, MODULE_LEDS_ENABLED);
