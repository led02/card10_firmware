#include "py/obj.h"
#include "py/objlist.h"
#include "py/runtime.h"
#include <stdio.h>

#include "epicardium.h"

static mp_obj_t mp_buttons_read(mp_obj_t mask_in)
{
	uint8_t mask          = mp_obj_get_int(mask_in);
	uint8_t button_states = epic_buttons_read(mask);
	return MP_OBJ_NEW_SMALL_INT(button_states);
}
static MP_DEFINE_CONST_FUN_OBJ_1(buttons_read_obj, mp_buttons_read);

static const mp_rom_map_elem_t buttons_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_buttons) },
	{ MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&buttons_read_obj) },
	{ MP_ROM_QSTR(MP_QSTR_BOTTOM_LEFT),
	  MP_OBJ_NEW_SMALL_INT(BUTTON_LEFT_BOTTOM) },
	{ MP_ROM_QSTR(MP_QSTR_BOTTOM_RIGHT),
	  MP_OBJ_NEW_SMALL_INT(BUTTON_RIGHT_BOTTOM) },
	{ MP_ROM_QSTR(MP_QSTR_TOP_RIGHT),
	  MP_OBJ_NEW_SMALL_INT(BUTTON_RIGHT_TOP) },
	{ MP_ROM_QSTR(MP_QSTR_TOP_LEFT),
	  MP_OBJ_NEW_SMALL_INT(BUTTON_LEFT_TOP) },
	{ MP_ROM_QSTR(MP_QSTR_RESET), MP_OBJ_NEW_SMALL_INT(BUTTON_RESET) },
};
static MP_DEFINE_CONST_DICT(
	buttons_module_globals, buttons_module_globals_table
);

const mp_obj_module_t buttons_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&buttons_module_globals,
};

/* clang-format off */
MP_REGISTER_MODULE(MP_QSTR_buttons, buttons_module, MODULE_BUTTONS_ENABLED);
