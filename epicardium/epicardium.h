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

#endif /* _EPICARDIUM_H */
