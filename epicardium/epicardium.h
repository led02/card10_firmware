#ifndef _EPICARDIUM_H
#define _EPICARDIUM_H
#include <stdint.h>

#ifndef API
#  define API(id, def) def
#endif

#define API_UART_WRITE 0x1
API(API_UART_WRITE, void epic_uart_write_str(char*str, intptr_t length));

#define API_UART_READ  0x2
API(API_UART_READ, char epic_uart_read_chr(void));

#define API_LEDS_SET  0x3
API(API_LEDS_SET, void epic_leds_set(int led, uint8_t r, uint8_t g, uint8_t b));

#endif /* _EPICARDIUM_H */
