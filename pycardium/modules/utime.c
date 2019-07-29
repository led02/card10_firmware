#include "epicardium.h"

#include "mxc_delay.h"

#include "py/mpconfig.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "extmod/utime_mphal.h"

#include <stdint.h>

// Needs to be after the stdint include ...
#include "lib/timeutils/timeutils.h"

static mp_obj_t time_time(void)
{
	mp_int_t seconds;
	/* MicroPython has its epoch at 2000-01-01. Our RTC is in UTC */
	seconds = epic_rtc_get_seconds() - 946684800UL;
	return mp_obj_new_int(seconds);
}
MP_DEFINE_CONST_FUN_OBJ_0(time_time_obj, time_time);

static mp_obj_t time_localtime(size_t n_args, const mp_obj_t *args)
{
	mp_int_t seconds;

	if (n_args == 0 || args[0] == mp_const_none) {
		seconds = epic_rtc_get_seconds() - 946684800UL;
	} else {
		seconds = mp_obj_get_int(args[0]);
	}

	timeutils_struct_time_t tm;
	timeutils_seconds_since_2000_to_struct_time(seconds, &tm);
	mp_obj_t tuple[8] = {
		tuple[0] = mp_obj_new_int(tm.tm_year),
		tuple[1] = mp_obj_new_int(tm.tm_mon),
		tuple[2] = mp_obj_new_int(tm.tm_mday),
		tuple[3] = mp_obj_new_int(tm.tm_hour),
		tuple[4] = mp_obj_new_int(tm.tm_min),
		tuple[5] = mp_obj_new_int(tm.tm_sec),
		tuple[6] = mp_obj_new_int(tm.tm_wday),
		tuple[7] = mp_obj_new_int(tm.tm_yday),
	};
	return mp_obj_new_tuple(8, tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(
	time_localtime_obj, 0, 1, time_localtime
);

static mp_obj_t time_mktime(mp_obj_t tuple)
{
	size_t len;
	mp_obj_t *elem;
	mp_obj_get_array(tuple, &len, &elem);

	/* localtime generates a tuple of len 8. CPython uses 9, so we accept both. */
	if (len < 8 || len > 9) {
		nlr_raise(mp_obj_new_exception_msg_varg(
			&mp_type_TypeError,
			"mktime needs a tuple of length 8 or 9 (%d given)",
			len)
		);
	}

	return mp_obj_new_int_from_uint(timeutils_mktime(
		mp_obj_get_int(elem[0]),
		mp_obj_get_int(elem[1]),
		mp_obj_get_int(elem[2]),
		mp_obj_get_int(elem[3]),
		mp_obj_get_int(elem[4]),
		mp_obj_get_int(elem[5]))
	);
}
static MP_DEFINE_CONST_FUN_OBJ_1(time_mktime_obj, time_mktime);

static const mp_rom_map_elem_t time_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },
	{ MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&time_time_obj) },
	{ MP_ROM_QSTR(MP_QSTR_localtime), MP_ROM_PTR(&time_localtime_obj) },
	{ MP_ROM_QSTR(MP_QSTR_mktime), MP_ROM_PTR(&time_mktime_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&mp_utime_sleep_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&mp_utime_sleep_ms_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&mp_utime_sleep_us_obj) },
#if 0
	/* TODO: Implement those */
	{MP_ROM_QSTR(MP_QSTR_ticks_ms), MP_ROM_PTR(&mp_utime_ticks_ms_obj)},
	{MP_ROM_QSTR(MP_QSTR_ticks_us), MP_ROM_PTR(&mp_utime_ticks_us_obj)},
	{MP_ROM_QSTR(MP_QSTR_ticks_cpu), MP_ROM_PTR(&mp_utime_ticks_cpu_obj)},
	{MP_ROM_QSTR(MP_QSTR_ticks_add), MP_ROM_PTR(&mp_utime_ticks_add_obj)},
	{MP_ROM_QSTR(MP_QSTR_ticks_diff), MP_ROM_PTR(&mp_utime_ticks_diff_obj)},
#endif
};
static MP_DEFINE_CONST_DICT(time_module_globals, time_module_globals_table);

const mp_obj_module_t mp_module_utime = {
	.base    = { &mp_type_module },
	.globals = (mp_obj_dict_t *)&time_module_globals,
};

/* Register the module to make it available in Python */
/* clang-format off */
MP_REGISTER_MODULE(MP_QSTR_utime, mp_module_utime, MODULE_UTIME_ENABLED);
