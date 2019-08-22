#pragma once

#include <stdint.h>

/* Display */
void bootloader_display_init(void);
void bootloader_display_header(void);
void bootloader_display_error(char *errtype, char *line1, char *line2);
void bootloader_display_line(int line, char *string, uint16_t color);

/* USB */
void run_usbmsc(void);
void bootloader_stop(void);
void bootloader_dirty(void);
void bootloader_clean(void);
