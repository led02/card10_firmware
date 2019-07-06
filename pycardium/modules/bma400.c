#include "py/obj.h"
#include "py/objlist.h"
#include "py/runtime.h"

#include "epicardium.h"

static mp_obj_t mp_bma400_get_accel()
{
	struct acceleration values;
	int ret = epic_bma400_get_accel(&values);

	if (ret < 0) {
		mp_raise_OSError(-ret);
	}

	mp_obj_t values_list[] = {
		mp_obj_new_float(values.x),
		mp_obj_new_float(values.y),
		mp_obj_new_float(values.z),
	};
	return mp_obj_new_tuple(3, values_list);
}
static MP_DEFINE_CONST_FUN_OBJ_0(bma400_get_accel_obj, mp_bma400_get_accel);

static const mp_rom_map_elem_t bma400_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_bma400) },
	{ MP_ROM_QSTR(MP_QSTR_get_accel), MP_ROM_PTR(&bma400_get_accel_obj) },
};
static MP_DEFINE_CONST_DICT(bma400_module_globals, bma400_module_globals_table);

const mp_obj_module_t bma400_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&bma400_module_globals,
};

/* Register the module to make it available in Python */
MP_REGISTER_MODULE(MP_QSTR_bma400, bma400_module, MODULE_BMA400_ENABLED);
