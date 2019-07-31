#include "py/mpconfig.h"

#ifndef Q
#define Q(x)
#endif

/* leds */
Q(leds)
Q(BOTTOM_LEFT)
Q(BOTTOM_RIGHT)
Q(TOP_LEFT)
Q(TOP_RIGHT)

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

/* vibra */
Q(vibra)
Q(vibrate)

Q(set_callback)
Q(enable_callback)
Q(disable_callback)
Q(BHI160)

/* display */
Q(sys_display)
Q(display)
Q(print)
Q(pixel)
Q(line)
Q(rect)
Q(circ)
Q(clear)

/* ambient */
Q(light_sensor)
Q(start)
Q(get_reading)
Q(stop)

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

