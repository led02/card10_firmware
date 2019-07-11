#ifndef _EPICARDIUM_H
#define _EPICARDIUM_H
#include <stdint.h>

#ifndef API
#define API(id, def) def
#endif

/* clang-format off */
#define API_UART_WRITE         0x1
#define API_UART_READ          0x2
#define API_LEDS_SET           0x3
#define API_VIBRA_SET          0x4
/* clang-format on */

/**
 * UART/Serial Interface
 * =====================
 */

/**
 * Write a string to all connected serial devices.  This includes:
 *
 * - Real UART, whose pins are mapped onto USB-C pins.  Accessible via the HW-debugger.
 * - A CDC-ACM device available via USB.
 * - Maybe, in the future, bluetooth serial?
 *
 * :param str:  String to write.  Does not necessarily have to be NULL-terminated.
 * :param length:  Amount of bytes to print.
 */
API(API_UART_WRITE, void epic_uart_write_str(const char *str, intptr_t length));

/**
 * Blocking read a single character from any connected serial device.
 * ``epic_uart_read_chr`` only returns
 * once one byte has been read.
 *
 * :return:  The byte.
 */
API(API_UART_READ, char epic_uart_read_chr(void));

/**
 * LEDs
 * ====
 */

/**
 * Set one of card10's RGB LEDs to a certain color.
 *
 * :param led:  Which led to set.  0-10 are the leds on the top and 11-14 are the 4 "ambient" leds.
 * :param r:  Red component of the color.
 * :param g:  Green component of the color.
 * :param b:  Blue component of the color.
 */
API(API_LEDS_SET, void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b));

/**
 * Misc
 * ====
 */

/**
 * Turn vibration motor on or off
 *
 * :param status: 1 to turn on, 0 to turn off.
 */
API(API_VIBRA_SET, void epic_vibra_set(int status));

#endif /* _EPICARDIUM_H */
