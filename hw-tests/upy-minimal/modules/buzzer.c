#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include <stdio.h>
#include "gpio.h"

static const gpio_cfg_t motor_pin = {
	PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE
};

STATIC mp_obj_t buzzer_set(mp_obj_t state_obj)
{
	if (state_obj == mp_const_true) {
		printf("Buzzer ON!\n");
		GPIO_OutSet(&motor_pin);
	} else if (state_obj == mp_const_false){
		printf("Buzzer OFF!\n");
		GPIO_OutClr(&motor_pin);
	} else {
		mp_raise_TypeError("expected bool");
	}
	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(buzzer_set_obj, buzzer_set);

// Define all properties of the example module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t buzzer_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_buzzer) },
	{ MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&buzzer_set_obj) },
};
STATIC MP_DEFINE_CONST_DICT(buzzer_module_globals, buzzer_module_globals_table);

// Define module object.
const mp_obj_module_t buzzer_module = {
	.base = { &mp_type_module },
	.globals = (mp_obj_dict_t*)&buzzer_module_globals,
};

// Register the module to make it available in Python
MP_REGISTER_MODULE(MP_QSTR_buzzer, buzzer_module, MODULE_BUZZER_ENABLED);
