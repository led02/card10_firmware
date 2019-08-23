#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "epicardium.h"
#include "api/common.h"
#include "mphalport.h"

STATIC mp_obj_t mp_max30001_enable_sensor(size_t n_args, const mp_obj_t *args)
{
	struct max30001_sensor_config cfg = { 0 };
	cfg.usb                           = mp_obj_is_true(args[0]);
	cfg.bias                          = mp_obj_is_true(args[1]);
	cfg.sample_rate                   = mp_obj_get_int(args[2]);
	cfg.sample_buffer_len             = mp_obj_get_int(args[3]);

	int stream_id = epic_max30001_enable_sensor(&cfg);

	return MP_OBJ_NEW_SMALL_INT(stream_id);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	mp_max30001_enable_sensor_obj, 4, 4, mp_max30001_enable_sensor
);

STATIC mp_obj_t mp_max30001_read_sensor(mp_obj_t stream_id_in)
{
	int16_t buf[256];
	int stream_id = mp_obj_get_int(stream_id_in);

	int n = epic_stream_read(stream_id, buf, sizeof(buf));

	mp_obj_list_t *list = mp_obj_new_list(0, NULL);
	for (int i = 0; i < n; i++) {
		mp_obj_list_append(list, mp_obj_new_int(buf[i]));
	}

	return MP_OBJ_FROM_PTR(list);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(
	mp_max30001_read_sensor_obj, mp_max30001_read_sensor
);

STATIC mp_obj_t mp_max30001_disable_sensor(void)
{
	int ret = epic_max30001_disable_sensor();

	return MP_OBJ_NEW_SMALL_INT(ret);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(
	mp_max30001_disable_sensor_obj, mp_max30001_disable_sensor
);

STATIC const mp_rom_map_elem_t max30001_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sys_max30001) },
	{ MP_ROM_QSTR(MP_QSTR_enable_sensor),
	  MP_ROM_PTR(&mp_max30001_enable_sensor_obj) },
	{ MP_ROM_QSTR(MP_QSTR_read_sensor),
	  MP_ROM_PTR(&mp_max30001_read_sensor_obj) },
	{ MP_ROM_QSTR(MP_QSTR_disable_sensor),
	  MP_ROM_PTR(&mp_max30001_disable_sensor_obj) },
};
STATIC MP_DEFINE_CONST_DICT(
	max30001_module_globals, max30001_module_globals_table
);

// Define module object.
const mp_obj_module_t max30001_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&max30001_module_globals,
};

/* clang-format off */
// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR_sys_max30001, max30001_module, MODULE_MAX30001_ENABLED);
