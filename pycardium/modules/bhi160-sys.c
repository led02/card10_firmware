#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "epicardium.h"
#include "api/common.h"
#include "mphalport.h"

extern const mp_obj_type_t mp_type_bhi160_sample;

typedef struct _bhi160_sample_obj_t {
	mp_obj_base_t base;
	int x;
	int y;
	int z;
} bhi160_sample_obj_t;

STATIC mp_obj_t mp_bhi160_enable_sensor(size_t n_args, const mp_obj_t *args)
{
	int sensor_type = mp_obj_get_int(args[0]);

	struct bhi160_sensor_config cfg = { 0 };
	cfg.sample_buffer_len           = mp_obj_get_int(args[1]);
	cfg.sample_rate                 = mp_obj_get_int(args[2]);
	cfg.dynamic_range               = mp_obj_get_int(args[3]);

	int sd = epic_bhi160_enable_sensor(sensor_type, &cfg);

	return MP_OBJ_NEW_SMALL_INT(sd);
}

STATIC mp_obj_t mp_bhi160_read_sensor(mp_obj_t stream_id_in)
{
	struct bhi160_data_vector buf[100];
	int sd = mp_obj_get_int(stream_id_in);

	int n = epic_stream_read(sd, buf, sizeof(buf));

	mp_obj_list_t *list = mp_obj_new_list(0, NULL);
	for (int i = 0; i < n; i++) {
		bhi160_sample_obj_t *o = m_new_obj(bhi160_sample_obj_t);
		o->base.type           = &mp_type_bhi160_sample;
		o->x                   = buf[i].x;
		o->y                   = buf[i].y;
		o->z                   = buf[i].z;

		mp_obj_list_append(list, MP_OBJ_FROM_PTR(o));
	}

	return MP_OBJ_FROM_PTR(list);
}

STATIC mp_obj_t mp_bhi160_x(mp_obj_t type)
{
	bhi160_sample_obj_t *self = MP_OBJ_TO_PTR(type);
	return MP_OBJ_NEW_SMALL_INT(self->x);
}
static MP_DEFINE_CONST_FUN_OBJ_1(mp_bhi160_x_obj, mp_bhi160_x);

STATIC mp_obj_t mp_bhi160_y(mp_obj_t type)
{
	bhi160_sample_obj_t *self = MP_OBJ_TO_PTR(type);
	return MP_OBJ_NEW_SMALL_INT(self->y);
}
static MP_DEFINE_CONST_FUN_OBJ_1(mp_bhi160_y_obj, mp_bhi160_y);

STATIC mp_obj_t mp_bhi160_z(mp_obj_t type)
{
	bhi160_sample_obj_t *self = MP_OBJ_TO_PTR(type);
	return MP_OBJ_NEW_SMALL_INT(self->z);
}
static MP_DEFINE_CONST_FUN_OBJ_1(mp_bhi160_z_obj, mp_bhi160_z);

STATIC const mp_rom_map_elem_t bhi160_sample_locals_dict_table[] = {
	{ MP_ROM_QSTR(MP_QSTR_x), MP_ROM_PTR(&mp_bhi160_x_obj) },
	{ MP_ROM_QSTR(MP_QSTR_y), MP_ROM_PTR(&mp_bhi160_y_obj) },
	{ MP_ROM_QSTR(MP_QSTR_z), MP_ROM_PTR(&mp_bhi160_z_obj) },
};

STATIC MP_DEFINE_CONST_DICT(
	bhi160_sample_locals_dict, bhi160_sample_locals_dict_table
);

const mp_obj_type_t mp_type_bhi160_sample = {
	{ &mp_type_type },
	.name        = MP_QSTR_BHI160Sample,
	.locals_dict = (mp_obj_dict_t *)&bhi160_sample_locals_dict,
};

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
