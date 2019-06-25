#include "py/obj.h"
#include "py/objlist.h"
#include "py/runtime.h"

#include "epicardium.h"

static mp_obj_t mp_leds_set(mp_obj_t led_in, mp_obj_t color_in)
{
	mp_check_self(mp_obj_is_type(color_in, &mp_type_list));
	mp_obj_list_t*color = MP_OBJ_TO_PTR(color_in);
	int led = mp_obj_get_int(led_in);

	if (color->len < 3) {
		mp_raise_ValueError("color list must have 3 elements");
	}

	epic_leds_set(
		led,
		mp_obj_get_int(color->items[0]),
		mp_obj_get_int(color->items[1]),
		mp_obj_get_int(color->items[2])
	);

	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(leds_set_obj, mp_leds_set);

static const mp_rom_map_elem_t leds_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_leds) },
	{ MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&leds_set_obj) },
};
static MP_DEFINE_CONST_DICT(leds_module_globals, leds_module_globals_table);

const mp_obj_module_t leds_module = {
	.base = { &mp_type_module },
	.globals = (mp_obj_dict_t*)&leds_module_globals,
};

/* Register the module to make it available in Python */
MP_REGISTER_MODULE(MP_QSTR_leds, leds_module, MODULE_LEDS_ENABLED);
