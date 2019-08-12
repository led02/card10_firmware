#include "gfx.h"
#include "framebuffer.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

const struct gfx_color_rgb gfx_colors_rgb[COLORS] = {
	{ 255, 255, 255 }, /* WHITE */
	{ 0, 0, 0 },       /* BLACK */
	{ 255, 0, 0 },     /* RED */
	{ 0, 255, 0 },     /* GREEN */
	{ 0, 0, 255 },     /* BLUE */
	{ 255, 255, 0 }    /* YELLOW */
};

void gfx_setpixel(struct gfx_region *r, int x, int y, Color c)
{
	if (x < 0 || y < 0)
		return;
	if (x >= r->width || y >= r->height)
		return;

	fb_setpixel(r->fb, r->x + x, r->y + y, c);
}

struct gfx_region gfx_screen(struct framebuffer *fb)
{
	struct gfx_region r = { .fb     = fb,
				.x      = 0,
				.y      = 0,
				.width  = fb->width,
				.height = fb->height };
	return r;
}

static inline int letter_bit(sFONT *font, char c, int x, int y)
{
	if (x < 0 || y < 0)
		return 0;
	if (x >= font->Width || y >= font->Height)
		return 0;
	if (c < ' ' || c > '~')
		return 0;

	size_t bytes_per_row      = font->Width / 8 + 1;
	size_t bytes_per_letter   = bytes_per_row * font->Height;
	int letter                = c - ' ';
	const uint8_t *letter_ptr = font->table + bytes_per_letter * letter;
	int horz_byte             = x / 8;
	int horz_bit              = 7 - x % 8;

	return (*(letter_ptr + y * bytes_per_row + horz_byte) >> horz_bit) & 1;
}

void gfx_putchar(
	sFONT *font,
	struct gfx_region *r,
	int x,
	int y,
	char ch,
	Color fg,
	Color bg
) {
	for (int yo = 0; yo < font->Height; yo++) {
		for (int xo = 0; xo < font->Width; xo++) {
			int lb = letter_bit(font, ch, xo, yo);

			if (fg != bg) {
				Color c = lb ? fg : bg;
				gfx_setpixel(r, x + xo, y + yo, c);
			} else {
				if (lb) {
					gfx_setpixel(r, x + xo, y + yo, fg);
				}
			}
		}
	}
}

void gfx_puts(
	sFONT *font,
	struct gfx_region *r,
	int x,
	int y,
	const char *str,
	Color fg,
	Color bg
) {
	while (*str) {
		x += font->Width;
		if (x >= r->width) {
			x = 0;
			y += font->Height;
		}
		if (y >= r->height)
			return;
		gfx_putchar(font, r, x, y, *str, fg, bg);
		str++;
	}
}

Color gfx_color_rgb_f(struct gfx_region *reg, float r, float g, float b)
{
	return fb_encode_color_rgb_f(reg->fb, r, g, b);
}

Color gfx_color_rgb(struct gfx_region *reg, uint8_t r, uint8_t g, uint8_t b)
{
	return fb_encode_color_rgb(reg->fb, r, g, b);
}

void gfx_update(struct gfx_region *reg)
{
	reg->fb->update(reg->fb);
}

void gfx_clear_to_color(struct gfx_region *reg, Color c)
{
	fb_clear_to_color(reg->fb, c);
}

void gfx_clear(struct gfx_region *reg)
{
	gfx_clear_to_color(reg, gfx_color(reg, BLACK));
}

void gfx_circle(struct gfx_region *reg, int x, int y, int r, int t, Color c)
{
	for (int y_ = y - r; y_ <= y + r; y_++) {
		for (int x_ = x - r; x_ <= x + r; x_++) {
			int dx    = (x_ - x) * (x_ - x);
			int dy    = (y_ - y) * (y_ - y);
			int outer = (r + t) * (r + t);
			int inner = r * r;
			int edge = ((dx + dy) >= inner) && ((dx + dy) <= outer);
			if (edge)
				gfx_setpixel(reg, x_, y_, c);
		}
	}
}

void gfx_circle_fill(struct gfx_region *reg, int x, int y, int r, Color c)
{
	for (int y_ = y - r; y_ <= y + r; y_++) {
		for (int x_ = x - r; x_ <= x + r; x_++) {
			int dx   = (x_ - x) * (x_ - x);
			int dy   = (y_ - y) * (y_ - y);
			int edge = r * r;
			int fill = (dx + dy) <= edge;
			if (fill)
				gfx_setpixel(reg, x_, y_, c);
		}
	}
}

void gfx_rectangle(
	struct gfx_region *reg, int x, int y, int w, int h, int t, Color c
) {
	if (t > 1) {
		gfx_thick_line(reg, x, y, x + w, y, t, c);
		gfx_thick_line(reg, x, y + h, x + w, y + h, t, c);
		gfx_thick_line(reg, x, y, x, y + h, t, c);
		gfx_thick_line(reg, x + w, y, x + w, y + h, t, c);
	} else {
		gfx_line(reg, x, y, x + w, y, c);
		gfx_line(reg, x, y + h, x + w, y + h, c);
		gfx_line(reg, x, y, x, y + h, c);
		gfx_line(reg, x + w, y, x + w, y + h, c);
	}
}

void gfx_rectangle_fill(
	struct gfx_region *reg, int x, int y, int w, int h, Color c
) {
	for (int y_ = y; y_ < y + h; y_++) {
		for (int x_ = x; x_ < x + w; x_++)
			gfx_setpixel(reg, x_, y_, c);
	}
}

void gfx_line(struct gfx_region *reg, int x1, int y1, int x2, int y2, Color c)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float de = fabs(dy / dx);
	float e  = .0f;
	int y    = y1;
	for (int x = x1; x < x2; x++) {
		gfx_setpixel(reg, x, y, c);
		e += de;
		if (e >= .5f) {
			y += dy >= .0f ? 1 : -1;
			e -= 1.0f;
		}
	}
}

void gfx_thick_line(
	struct gfx_region *reg, int x1, int y1, int x2, int y2, int t, Color c
) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	float de = fabs(dy / dx);
	float e  = .0f;
	int y    = y1;
	for (int x = x1; x < x2; x++) {
		gfx_circle_fill(reg, x, y, t, c);
		e += de;
		if (e >= .5f) {
			y += dy >= .0f ? 1 : -1;
			e -= 1.0f;
		}
	}
}

Color gfx_color(struct gfx_region *reg, enum gfx_color color)
{
	if ((int)(color) >= COLORS)
		return 0;

	const struct gfx_color_rgb *c = &gfx_colors_rgb[color];
	return gfx_color_rgb(reg, c->r, c->g, c->b);
}

void gfx_copy_region_raw(
	struct gfx_region *reg,
	int x,
	int y,
	int w,
	int h,
	size_t bpp,
	const void *p
) {
	for (int y_ = 0; y_ < h; y_++) {
		for (int x_ = 0; x_ < w; x_++) {
			Color c;

			switch (bpp) {
			default:
			case 2:
				c = *(const uint16_t *)(p);
				break;
			}

			gfx_setpixel(reg, x + x_, y + y_, c);
			p += bpp;
		}
	}
}

void gfx_copy_raw(struct gfx_region *reg, const void *p, size_t size)
{
	fb_copy_raw(reg->fb, p, size);
}
