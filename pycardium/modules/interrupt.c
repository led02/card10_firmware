#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "epicardium.h"
#include "api/common.h"
#include "mphalport.h"

// TODO: these should be intialized as mp_const_none
mp_obj_t callbacks[API_INT_MAX + 1] = {
	0,
};

void api_interrupt_handler_catch_all(api_int_id_t id)
{
	// TODO: check if id is out of rante
	// TOOD: check against mp_const_none
	if (id <= API_INT_MAX) {
		if (callbacks[id]) {
			mp_sched_schedule(
				callbacks[id], MP_OBJ_NEW_SMALL_INT(id)
			);
		}
	}
}

STATIC mp_obj_t mp_interrupt_set_callback(mp_obj_t id_in, mp_obj_t callback_obj)
{
	api_int_id_t id = mp_obj_get_int(id_in);
	if (callback_obj != mp_const_none &&
	    !mp_obj_is_callable(callback_obj)) {
		mp_raise_ValueError("handler must be None or callable");
	}

	// TODO: throw error if id is out of range
	if (id <= API_INT_MAX) {
		callbacks[id] = callback_obj;
	}

	return mp_const_none;
}

STATIC mp_obj_t mp_interrupt_enable_callback(mp_obj_t id_in)
{
	api_int_id_t id = mp_obj_get_int(id_in);

	// TODO: throw error if id is out of range or epic_interrupt_enable failed
	if (epic_interrupt_enable(id) < 0) {
	}

	return mp_const_none;
}

STATIC mp_obj_t mp_interrupt_disable_callback(mp_obj_t id_in)
{
	api_int_id_t id = mp_obj_get_int(id_in);

	// TODO: throw error if id is out of range or epic_interrupt_enable failed
	if (epic_interrupt_disable(id) < 0) {
	}

	return mp_const_none;
}


STATIC MP_DEFINE_CONST_FUN_OBJ_2(
	interrupt_set_callback_obj, mp_interrupt_set_callback
);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(
	interrupt_enable_callback_obj, mp_interrupt_enable_callback
);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(
	interrupt_disable_callback_obj, mp_interrupt_disable_callback
);

STATIC const mp_rom_map_elem_t interrupt_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_interrupt) },
	{ MP_ROM_QSTR(MP_QSTR_set_callback),
	  MP_ROM_PTR(&interrupt_set_callback_obj) },
	{ MP_ROM_QSTR(MP_QSTR_enable_callback),
	  MP_ROM_PTR(&interrupt_enable_callback_obj) },
	{ MP_ROM_QSTR(MP_QSTR_disable_callback),
	  MP_ROM_PTR(&interrupt_disable_callback_obj) },
	{ MP_ROM_QSTR(MP_QSTR_BHI160), MP_OBJ_NEW_SMALL_INT(2) },
};
STATIC MP_DEFINE_CONST_DICT(
	interrupt_module_globals, interrupt_module_globals_table
);

// Define module object.
const mp_obj_module_t interrupt_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&interrupt_module_globals,
};

/* clang-format off */
// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR_interrupt, interrupt_module, MODULE_INTERRUPT_ENABLED);
