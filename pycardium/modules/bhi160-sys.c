#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "epicardium.h"
#include "api/common.h"
#include "mphalport.h"

STATIC mp_obj_t mp_bhi160_enable_sensor(size_t n_args, const mp_obj_t *args)
{
	int sensor_type = mp_obj_get_int(args[0]);

	struct bhi160_sensor_config cfg = { 0 };
	cfg.sample_buffer_len           = mp_obj_get_int(args[1]);
	cfg.sample_rate                 = mp_obj_get_int(args[2]);
	cfg.dynamic_range               = mp_obj_get_int(args[3]);

	//cfg.sample_buffer_len = 200;
	//cfg.sample_rate       = 4;
	//cfg.dynamic_range     = 2;

	int sd = epic_bhi160_enable_sensor(sensor_type, &cfg);

	return MP_OBJ_NEW_SMALL_INT(sd);
}

STATIC mp_obj_t mp_bhi160_read_sensor(mp_obj_t stream_id_in)
{
	struct bhi160_data_vector buf[100];
	int sd = mp_obj_get_int(stream_id_in);

	int n = epic_stream_read(sd, buf, sizeof(buf));

	return MP_OBJ_NEW_SMALL_INT(n);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	mp_bhi160_enable_sensor_obj, 4, 4, mp_bhi160_enable_sensor
);

STATIC MP_DEFINE_CONST_FUN_OBJ_1(
	mp_bhi160_read_sensor_obj, mp_bhi160_read_sensor
);

STATIC const mp_rom_map_elem_t bhi160_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sys_bhi160) },
	{ MP_ROM_QSTR(MP_QSTR_enable_sensor),
	  MP_ROM_PTR(&mp_bhi160_enable_sensor_obj) },
	{ MP_ROM_QSTR(MP_QSTR_read_sensor),
	  MP_ROM_PTR(&mp_bhi160_read_sensor_obj) },
};
STATIC MP_DEFINE_CONST_DICT(bhi160_module_globals, bhi160_module_globals_table);

// Define module object.
const mp_obj_module_t bhi160_module = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&bhi160_module_globals,
};

/* clang-format off */
// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR_sys_bhi160, bhi160_module, MODULE_BHI160_ENABLED);
