#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H
#include "gfx.h"
#include "fonts.h"
#include <stdint.h>

#define TEXTBUFFER_MAX_WIDTH 40
#define TEXTBUFFER_MAX_HEIGHT 20

struct txt_glyph {
	char ch;
	Color fg_color;
	Color bg_color;
};

struct txt_buffer {
	struct gfx_region *reg;
	sFONT *font;
	int cursor_column;
	int cursor_row;
	Color fg_color;
	Color bg_color;
	int draw_cursor;
	int auto_update;

	struct txt_glyph text[TEXTBUFFER_MAX_HEIGHT][TEXTBUFFER_MAX_WIDTH];
};

enum txt_color {
	TEXT_FOREGROUND,
	TEXT_BACKGROUND
};

void txt_init(struct txt_buffer *txtb, struct gfx_region *reg, sFONT *f);
size_t txt_width(struct txt_buffer *tm);
size_t txt_height(struct txt_buffer *tm);
void txt_clear(struct txt_buffer *tm);
void txt_putchar(struct txt_buffer *tm, char ch);
void txt_puts(struct txt_buffer *tm, const char *str);
void txt_draw(struct txt_buffer *tm);
void txt_set_color_f(struct txt_buffer *tm, enum txt_color sw, float r, float g,
								     float b);
void txt_set_color(struct txt_buffer *tm, enum txt_color sw, int r, int g,
								 int b);
void txt_set_transparent(struct txt_buffer *tm);
void txt_set_cursor(struct txt_buffer *tm, int x, int y, int draw_cursor);
void txt_update(struct txt_buffer *tm);

#endif
