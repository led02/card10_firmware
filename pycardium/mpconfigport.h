/* Hardware Name */
#define MICROPY_HW_BOARD_NAME "card10"
#define MICROPY_HW_MCU_NAME "max32666"

/* MicroPython Config Options */

/* We raise asynchronously from an interrupt handler */
#define MICROPY_ASYNC_KBD_INTR              (1)
#define MICROPY_KBD_EXCEPTION               (1)

/* Enable precompiled frozen modules */
#define MICROPY_MODULE_FROZEN_MPY           (1)
#define MICROPY_MODULE_FROZEN               (1)
#define MICROPY_QSTR_EXTRA_POOL             mp_qstr_frozen_const_pool

#define MICROPY_ENABLE_DOC_STRING           (1)
#define MICROPY_ENABLE_GC                   (1)
#define MICROPY_FLOAT_IMPL                  (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_HELPER_REPL                 (1)
#define MICROPY_LONGINT_IMPL                (MICROPY_LONGINT_IMPL_LONGLONG)

#define MICROPY_ENABLE_SCHEDULER            (1)

#define MICROPY_ENABLE_SOURCE_LINE          (1)

/* urandom init */
int mp_hal_trng_read_int(void);

/* Builtin function and modules */
#define MICROPY_PY_ALL_SPECIAL_METHODS      (1)
#define MICROPY_PY_BUILTINS_HELP            (1)
#define MICROPY_PY_BUILTINS_HELP_MODULES    (1)
#define MICROPY_PY_UBINASCII                (1)
#define MICROPY_PY_UHEAPQ                   (1)
#define MICROPY_PY_UJSON                    (1)
#define MICROPY_PY_URANDOM                  (1)
#define MICROPY_PY_URANDOM_EXTRA_FUNCS      (1)
#define MICROPY_PY_URANDOM_SEED_INIT_FUNC   (mp_hal_trng_read_int())
#define MICROPY_PY_URE                      (1)
#define MICROPY_PY_URE_MATCH_GROUPS         (1)
#define MICROPY_PY_URE_MATCH_SPAN_START_END (1)
#define MICROPY_PY_URE_SUB                  (1)
#define MICROPY_PY_UTIME_MP_HAL             (1)
#define MICROPY_PY_IO_FILEIO                (1)
#define MICROPY_PY_UERRNO                   (1)

/* Modules */
#define MODULE_BHI160_ENABLED               (1)
#define MODULE_BME680_ENABLED               (1)
#define MODULE_BUTTONS_ENABLED              (1)
#define MODULE_DISPLAY_ENABLED              (1)
#define MODULE_GPIO_ENABLED                 (1)
#define MODULE_INTERRUPT_ENABLED            (1)
#define MODULE_LEDS_ENABLED                 (1)
#define MODULE_LIGHT_SENSOR_ENABLED         (1)
#define MODULE_MAX30001_ENABLED             (1)
#define MODULE_OS_ENABLED                   (1)
#define MODULE_PERSONAL_STATE_ENABLED       (1)
#define MODULE_POWER_ENABLED                (1)
#define MODULE_UTIME_ENABLED                (1)
#define MODULE_VIBRA_ENABLED                (1)

/*
 * This port is intended to be 32-bit, but unfortunately, int32_t for
 * different targets may be defined in different ways - either as int
 * or as long. This requires different printf formatting specifiers
 * to print such value. So, we avoid int32_t and use int directly.
 */
#define UINT_FMT "%u"
#define INT_FMT "%d"
typedef int mp_int_t;       /* must be pointer size */
typedef unsigned mp_uint_t; /* must be pointer size */

typedef long mp_off_t;

/* extra built in names to add to the global namespace */
#define MICROPY_PORT_BUILTINS \
    { MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&mp_builtin_open_obj) },

/*
 * Make a pointer to RAM callable (eg set lower bit for Thumb code)
 * (This scheme won't work if we want to mix Thumb and normal ARM code.)
 */
#define MICROPY_MAKE_POINTER_CALLABLE(p) ((void*)((mp_uint_t)(p) | 1))

/* We need to provide a declaration/definition of alloca() */
#include <alloca.h>

/* TODO: Document this */
#define MP_STATE_PORT MP_STATE_VM

/* For some reason, we need to define readline history manually */
#define MICROPY_PORT_ROOT_POINTERS \
    const char *readline_hist[16];

