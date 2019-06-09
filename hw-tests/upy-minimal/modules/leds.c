#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include <stdio.h>

STATIC mp_obj_t mp_leds_set(size_t n_args, const mp_obj_t *args)
{
	assert (n_args == 4);
	int led = mp_obj_get_int (args[0]);
	int r = mp_obj_get_int (args[1]);
	int g = mp_obj_get_int (args[2]);
	int b = mp_obj_get_int (args[3]);

	if ( (0 > led || 256 < led) &&
	     (0 >   r || 256 <   r) &&
	     (0 >   g || 256 <   g) &&
	     (0 >   b || 256 <   b) )
	{
		mp_raise_ValueError("out of bounds");
	}
	printf("Set %u to (%x %x %x)\n", led, r, g, b);
	leds_set (led, r, g, b);
	leds_update ();

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(leds_set_obj, 4, 4, mp_leds_set);

STATIC mp_obj_t mp_leds_set_dim(mp_obj_t led_obj, mp_obj_t dim_obj)
{
	int led = mp_obj_get_int (led_obj);
	int dim = mp_obj_get_int (dim_obj);

	if ( (0 > led || 256 < led) &&
	     (0 > dim || 256 < dim) )
	{
		mp_raise_ValueError("out of bounds");
	}
	leds_set_dim (led, dim);
	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(leds_set_dim_obj, mp_leds_set_dim);

// Define all properties of the example module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t leds_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_leds) },
	{ MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&leds_set_obj) },
	{ MP_ROM_QSTR(MP_QSTR_set_dim), MP_ROM_PTR(&leds_set_dim_obj) },
};
STATIC MP_DEFINE_CONST_DICT(leds_module_globals, leds_module_globals_table);

// Define module object.
const mp_obj_module_t leds_module = {
	.base = { &mp_type_module },
	.globals = (mp_obj_dict_t*)&leds_module_globals,
};

// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR_leds, leds_module, MODULE_LEDS_ENABLED);
