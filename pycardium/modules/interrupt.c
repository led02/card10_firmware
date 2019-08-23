#include "interrupt.h"

#include "api/common.h"
#include "epicardium.h"

#include "py/builtin.h"
#include "py/obj.h"
#include "py/runtime.h"

// TODO: these should be intialized as mp_const_none
mp_obj_t callbacks[EPIC_INT_NUM] = {
	0,
};

void epic_isr_default_handler(api_int_id_t id)
{
	// TODO: check if id is out of rante
	// TOOD: check against mp_const_none
	if (id < EPIC_INT_NUM) {
		if (callbacks[id]) {
			mp_sched_schedule(
				callbacks[id], MP_OBJ_NEW_SMALL_INT(id)
			);
		}
	}
}

mp_obj_t mp_interrupt_set_callback(mp_obj_t id_in, mp_obj_t callback_obj)
{
	api_int_id_t id = mp_obj_get_int(id_in);
	if (callback_obj != mp_const_none &&
	    !mp_obj_is_callable(callback_obj)) {
		mp_raise_ValueError("handler must be None or callable");
	}

	// TODO: throw error if id is out of range
	if (id < EPIC_INT_NUM) {
		callbacks[id] = callback_obj;
	}

	return mp_const_none;
}

mp_obj_t mp_interrupt_enable_callback(mp_obj_t id_in)
{
	api_int_id_t id = mp_obj_get_int(id_in);

	// TODO: throw error if id is out of range or epic_interrupt_enable failed
	if (epic_interrupt_enable(id) < 0) {
	}

	return mp_const_none;
}

mp_obj_t mp_interrupt_disable_callback(mp_obj_t id_in)
{
	api_int_id_t id = mp_obj_get_int(id_in);

	// TODO: throw error if id is out of range or epic_interrupt_enable failed
	if (epic_interrupt_disable(id) < 0) {
	}

	return mp_const_none;
}

static MP_DEFINE_CONST_FUN_OBJ_2(
	interrupt_set_callback_obj, mp_interrupt_set_callback
);
static MP_DEFINE_CONST_FUN_OBJ_1(
	interrupt_enable_callback_obj, mp_interrupt_enable_callback
);
static MP_DEFINE_CONST_FUN_OBJ_1(
	interrupt_disable_callback_obj, mp_interrupt_disable_callback
);

static const mp_rom_map_elem_t interrupt_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_interrupt) },
	{ MP_ROM_QSTR(MP_QSTR_set_callback),
	  MP_ROM_PTR(&interrupt_set_callback_obj) },
	{ MP_ROM_QSTR(MP_QSTR_enable_callback),
	  MP_ROM_PTR(&interrupt_enable_callback_obj) },
	{ MP_ROM_QSTR(MP_QSTR_disable_callback),
	  MP_ROM_PTR(&interrupt_disable_callback_obj) },

	/* Interrupt Numbers */
	{ MP_ROM_QSTR(MP_QSTR_RTC_ALARM),
	  MP_OBJ_NEW_SMALL_INT(EPIC_INT_RTC_ALARM) },
	{ MP_ROM_QSTR(MP_QSTR_BHI160_ACCELEROMETER),
	  MP_OBJ_NEW_SMALL_INT(EPIC_INT_BHI160_ACCELEROMETER) },
	{ MP_ROM_QSTR(MP_QSTR_BHI160_ORIENTATION),
	  MP_OBJ_NEW_SMALL_INT(EPIC_INT_BHI160_ORIENTATION) },
	{ MP_ROM_QSTR(MP_QSTR_BHI160_GYROSCOPE),
	  MP_OBJ_NEW_SMALL_INT(EPIC_INT_BHI160_GYROSCOPE) },
	{ MP_ROM_QSTR(MP_QSTR_MAX30001_ECG),
	  MP_OBJ_NEW_SMALL_INT(EPIC_INT_MAX30001_ECG) },

};
static MP_DEFINE_CONST_DICT(
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
