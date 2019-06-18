#include "py/obj.h"
#include "epicardium.h"

static mp_obj_t mp_leds_set(size_t n_args, const mp_obj_t *args)
{
	int led   = mp_obj_get_int(args[0]);
	uint8_t r = mp_obj_get_int(args[1]);
	uint8_t g = mp_obj_get_int(args[2]);
	uint8_t b = mp_obj_get_int(args[3]);

	epic_leds_set(led, r, g, b);

	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(leds_set_obj, 4, 4, mp_leds_set);

static const mp_rom_map_elem_t leds_module_globals_table[] = {
	{MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_leds)},
	{MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&leds_set_obj)},
};
static MP_DEFINE_CONST_DICT(leds_module_globals, leds_module_globals_table);

const mp_obj_module_t leds_module = {
	.base = { &mp_type_module },
	.globals = (mp_obj_dict_t*)&leds_module_globals,
};

/* Register the module to make it available in Python */
MP_REGISTER_MODULE(MP_QSTR_leds, leds_module, MODULE_LEDS_ENABLED);
