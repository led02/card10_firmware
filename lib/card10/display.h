#ifndef DISPLAY_H
#include "framebuffer.h"
#include "textbuffer.h"

extern struct gfx_region display_screen;
extern struct txt_buffer display_textb;

void display_init(void);
void display_init_slim(void);

#endif
