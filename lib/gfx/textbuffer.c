#include "textbuffer.h"
#include "gfx.h"
#include <string.h>

void txt_init(struct txt_buffer *txtb, struct gfx_region *reg, sFONT *f)
{
	txtb->reg           = reg;
	txtb->font          = f;
	txtb->cursor_column = 0;
	txtb->cursor_row    = 0;
	txtb->fg_color      = gfx_color_rgb_f(reg, 1.0f, 1.0f, 1.0f);
	txtb->bg_color      = gfx_color_rgb_f(reg, .0f, .0f, .0f);
	txtb->draw_cursor   = 1;
	txtb->auto_update   = 1;
}

static inline size_t width_(struct txt_buffer *tm)
{
	return tm->reg->width / tm->font->Width;
}

static inline size_t height_(struct txt_buffer *tm)
{
	return tm->reg->height / tm->font->Height;
}

size_t txt_width(struct txt_buffer *tm)
{
	return width_(tm);
}

size_t txt_height(struct txt_buffer *tm)
{
	return height_(tm);
}

static void scrollup(struct txt_buffer *tm)
{
	const int last_row     = height_(tm) - 1;
	const size_t line_size = width_(tm) * sizeof(struct txt_glyph);

	for (int row = 0; row < last_row; row++)
		memcpy(&tm->text[row][0], &tm->text[row + 1][0], line_size);
	for (int col = 0; col < width_(tm); col++) {
		struct txt_glyph *g = &tm->text[last_row][col];
		g->ch               = ' ';
		g->fg_color         = tm->fg_color;
		g->bg_color         = tm->bg_color;
	}
}

static void newline(struct txt_buffer *tm)
{
	const int last_row = height_(tm) - 1;

	scrollup(tm);
	tm->cursor_row    = last_row;
	tm->cursor_column = 0;
}

static inline void advance_cursor(struct txt_buffer *tm)
{
	const int last_row = height_(tm) - 1;

	tm->cursor_column++;
	if (tm->cursor_column >= width_(tm)) {
		tm->cursor_column = 0;
		tm->cursor_row++;
		if (tm->cursor_row > last_row)
			newline(tm);
	}
}

static void tab(struct txt_buffer *tm)
{
	while (tm->cursor_column % 5)
		txt_putchar(tm, ' ');
}

static inline struct txt_glyph *getchar_(struct txt_buffer *tm, int x, int y)
{
	if (x < 0 || x >= TEXTBUFFER_MAX_WIDTH)
		return NULL;
	if (y < 0 || y >= TEXTBUFFER_MAX_HEIGHT)
		return NULL;
	return &tm->text[y][x];
}

void txt_clear(struct txt_buffer *tm)
{
	int w = width_(tm);
	int h = height_(tm);
	for (int r = 0; r < h; r++) {
		for (int c = 0; c < w; c++) {
			struct txt_glyph *g = getchar_(tm, c, r);
			if (g == NULL)
				continue;

			g->ch       = ' ';
			g->fg_color = tm->fg_color;
			g->bg_color = tm->bg_color;
		}
	}

	tm->cursor_column = 0;
	tm->cursor_row    = 0;

	if (tm->auto_update)
		txt_update(tm);
}

void txt_putchar(struct txt_buffer *tm, char ch)
{
	struct txt_glyph *g = getchar_(tm, tm->cursor_column, tm->cursor_row);
	if (g == NULL)
		return;

	switch (ch) {
	case '\n':
		newline(tm);
		break;
	case '\t':
		tab(tm);
		break;
	default:
		g->ch       = ch;
		g->fg_color = tm->fg_color;
		g->bg_color = tm->bg_color;
		advance_cursor(tm);
	}

	if (tm->auto_update)
		txt_update(tm);
}

void txt_puts(struct txt_buffer *tm, const char *str)
{
	while (*str) {
		txt_putchar(tm, *str);
		str++;
	}
}

static inline int cursor_px_column(struct txt_buffer *tm)
{
	return tm->font->Width * tm->cursor_column;
}

static inline int cursor_px_row(struct txt_buffer *tm)
{
	return tm->font->Height * tm->cursor_row;
}

static void draw_cursor_(struct txt_buffer *tm)
{
	const int x = cursor_px_column(tm);
	const int y = cursor_px_row(tm);
	const int w = tm->font->Width;
	const int h = tm->font->Height;
	gfx_rectangle_fill(tm->reg, x, y, w, h, tm->fg_color);
}

void txt_draw(struct txt_buffer *tm)
{
	const int w   = width_(tm);
	const int h   = height_(tm);
	int px_column = 0;
	int px_row    = 0;

	for (int r = 0; r < h; r++) {
		px_column = 0;
		for (int c = 0; c < w; c++) {
			struct txt_glyph *g = getchar_(tm, c, r);
			if (g == NULL)
				continue;

			gfx_putchar(
				tm->font,
				tm->reg,
				px_column,
				px_row,
				g->ch,
				g->fg_color,
				g->bg_color
			);
			px_column += tm->font->Width;
		}

		px_row += tm->font->Height;
	}

	if (tm->draw_cursor)
		draw_cursor_(tm);
}

void txt_set_color_f(
	struct txt_buffer *tm, enum txt_color sw, float r, float g, float b
) {
	Color c = gfx_color_rgb_f(tm->reg, r, g, b);

	switch (c) {
	case TEXT_FOREGROUND:
		tm->fg_color = c;
		break;
	case TEXT_BACKGROUND:
		tm->bg_color = c;
		break;
	}
}

void txt_set_color(struct txt_buffer *tm, enum txt_color sw, int r, int g, int b)
{
	Color c = gfx_color_rgb(tm->reg, r, g, b);

	switch (c) {
	case TEXT_FOREGROUND:
		tm->fg_color = c;
		break;
	case TEXT_BACKGROUND:
		tm->bg_color = c;
		break;
	}
}

void txt_set_cursor(struct txt_buffer *tm, int x, int y, int draw_cursor)
{
	tm->draw_cursor = draw_cursor;

	if (x < 0 || x >= width_(tm))
		return;
	if (y < 0 || y >= height_(tm))
		return;

	tm->cursor_column = x;
	tm->cursor_row    = y;

	if (tm->auto_update)
		txt_update(tm);
}

void txt_set_transparent(struct txt_buffer *tm)
{
	tm->bg_color = tm->fg_color;
}

void txt_update(struct txt_buffer *tm)
{
	gfx_update(tm->reg);
}
