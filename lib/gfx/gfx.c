#include "gfx.h"
#include "framebuffer.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

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
	// iterate over the string
	while (*str) {
		// if the current position plus the width of the next character
		// would bring us outside of the display ...
		if ((x + font->Width) > r->width) {
			// ... we move down a line before printing the character
			x = 0;
			y += font->Height;
		}
		// if the line is outside the display we return
		if (y >= r->height)
			return;

		// now print the character
		gfx_putchar(font, r, x, y, *str, fg, bg);
		str++;

		// move along on the x axis to get the position of the next character
		x += font->Width;
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
	int outer = (r + t) * (r + t);
	int inner = r * r;
	for (int y_ = y - r - t; y_ <= y + r + t; y_++) {
		for (int x_ = x - r - t; x_ <= x + r + t; x_++) {
			int dx   = (x_ - x) * (x_ - x);
			int dy   = (y_ - y) * (y_ - y);
			int edge = ((dx + dy) >= inner) && ((dx + dy) <= outer);
			if (edge)
				gfx_setpixel(reg, x_, y_, c);
		}
	}
}

void gfx_circle_fill(struct gfx_region *reg, int x, int y, int r, Color c)
{
	int edge = r * r;
	for (int y_ = y - r; y_ <= y + r; y_++) {
		for (int x_ = x - r; x_ <= x + r; x_++) {
			int dx   = (x_ - x) * (x_ - x);
			int dy   = (y_ - y) * (y_ - y);
			int fill = (dx + dy) <= edge;
			if (fill)
				gfx_setpixel(reg, x_, y_, c);
		}
	}
}

void gfx_rectangle(
	struct gfx_region *reg, int x, int y, int w, int h, int t, Color c
) {
	gfx_line(reg, x, y, x + w, y, t, c);
	gfx_line(reg, x, y + h, x + w, y + h, t, c);
	gfx_line(reg, x, y, x, y + h, t, c);
	gfx_line(reg, x + w, y, x + w, y + h, t, c);
}

void gfx_rectangle_fill(
	struct gfx_region *reg, int x, int y, int w, int h, Color c
) {
	for (int y_ = y; y_ < y + h; y_++) {
		for (int x_ = x; x_ < x + w; x_++)
			gfx_setpixel(reg, x_, y_, c);
	}
}

/*
 * For derivation of the algorithm, see:
 * https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 */
static void plot_line_low(
	struct gfx_region *reg, int x0, int y0, int x1, int y1, int t, Color c
) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;

	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int d = 2 * dy - dx;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (t > 1) {
			gfx_circle_fill(reg, x, y, t, c);
		} else {
			gfx_setpixel(reg, x, y, c);
		}

		if (d > 0) {
			y += yi;
			d -= 2 * dx;
		}
		d += 2 * dy;
	}
}

/*
 * For derivation of the algorithm, see:
 * https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 */
static void plot_line_high(
	struct gfx_region *reg, int x0, int y0, int x1, int y1, int t, Color c
) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;

	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}

	int d = 2 * dx - dy;
	int x = x0;
	for (int y = y0; y <= y1; y++) {
		if (t > 1) {
			gfx_circle_fill(reg, x, y, t, c);
		} else {
			gfx_setpixel(reg, x, y, c);
		}

		if (d > 0) {
			x += xi;
			d -= 2 * dy;
		}
		d += 2 * dx;
	}
}

void gfx_line(
	struct gfx_region *reg, int x0, int y0, int x1, int y1, int t, Color c
) {
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			plot_line_low(reg, x1, y1, x0, y0, t, c);
		} else {
			plot_line_low(reg, x0, y0, x1, y1, t, c);
		}
	} else {
		if (y0 > y1) {
			plot_line_high(reg, x1, y1, x0, y0, t, c);
		} else {
			plot_line_high(reg, x0, y0, x1, y1, t, c);
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
