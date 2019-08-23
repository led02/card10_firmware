#include "py/mpconfig.h"

#ifndef Q
#define Q(x)
#endif

/* leds */
Q(sys_leds)
Q(update)
Q(set)
Q(set_hsv)
Q(prep)
Q(prep_hsv)
Q(set_all)
Q(set_all_hsv)
Q(set_flashlight)
Q(set_rocket)
Q(set_powersave)
Q(set_gamma)
Q(dim_top)
Q(dim_bottom)
Q(BOTTOM_LEFT)
Q(BOTTOM_RIGHT)
Q(TOP_LEFT)
Q(TOP_RIGHT)

/* buttons */
Q(buttons)
Q(read)
Q(BOTTOM_LEFT)
Q(TOP_LEFT)
Q(BOTTOM_RIGHT)
Q(TOP_RIGHT)
Q(RESET)

/* utime */
Q(utime)
Q(alarm)
Q(sleep)
Q(sleep_ms)
Q(sleep_us)
Q(ticks_ms)
Q(ticks_us)
Q(ticks_cpu)
Q(ticks_add)
Q(ticks_diff)
Q(localtime)
Q(mktime)
Q(time)
Q(time_ms)
Q(set_time)
Q(set_unix_time)

/* vibra */
Q(vibra)
Q(vibrate)

Q(set_callback)
Q(enable_callback)
Q(disable_callback)
Q(BHI160_ACCELEROMETER)
Q(BHI160_ORIENTATION)
Q(BHI160_GYROSCOPE)
Q(RTC_ALARM)

/* bhi160 */
Q(sys_bhi160)
Q(enable_sensor)
Q(disable_sensor)
Q(read_sensor)
Q(x)
Q(y)
Q(z)

/* power */
Q(power)
Q(read_battery_voltage)
Q(read_battery_current)
Q(read_chargein_voltage)
Q(read_chargein_current)
Q(read_system_voltage)
Q(read_thermistor_voltage)

/* display */
Q(sys_display)
Q(display)
Q(print)
Q(pixel)
Q(backlight)
Q(line)
Q(rect)
Q(circ)
Q(clear)

/* ambient */
Q(light_sensor)
Q(start)
Q(get_reading)
Q(stop)

/* bme680 */
Q(bme680)
Q(init)
Q(deinit)
Q(get_data)

/* file */
Q(__del__)
Q(__enter__)
Q(__exit__)
Q(close)
Q(encoding)
Q(file)
Q(FileIO)
Q(flush)
Q(mode)
Q(r)
Q(read)
Q(readinto)
Q(readline)
Q(readlines)
Q(seek)
Q(tell)
Q(TextIOWrapper)
Q(write)

/* os */
Q(os)
Q(exit)
Q(reset)
Q(exec)
Q(listdir)
Q(unlink)
Q(mkdir)
Q(rename)
Q(read_battery)
Q(urandom)

/* gpio */
Q(gpio)
Q(set_mode)
Q(get_mode)
Q(read)
Q(write)
Q(WRISTBAND_1)
Q(WRISTBAND_2)
Q(WRISTBAND_3)
Q(WRISTBAND_4)
Q(INPUT)
Q(OUTPUT)
Q(PULL_UP)
Q(PULL_DOWN)

/* path */
Q(/apps)
Q(/lib)

/* personal_state */
Q(personal_state)
Q(get)
Q(NO_STATE)
Q(NO_CONTACT)
Q(CHAOS)
Q(COMMUNICATION)
Q(CAMP)

Q(MAX30001_ECG)
