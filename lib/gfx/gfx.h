#ifndef GFX_H
#define GFX_H
#include "fonts.h"
#include "framebuffer.h"
#include <stddef.h>

struct gfx_region {
	struct framebuffer *fb;
	size_t x;
	size_t y;
	size_t width;
	size_t height;
};

void gfx_setpixel(struct gfx_region *r, int x, int y, Color c);
struct gfx_region gfx_screen(struct framebuffer *fb);
void gfx_putchar(const sFONT *font, struct gfx_region *reg, int x, int y, char ch,
						        Color fg, Color bg);
void gfx_puts(const sFONT *font, struct gfx_region *reg, int x, int y,
			   const char *str, Color fg, Color bg);
Color gfx_color_rgb_f(struct gfx_region *reg, float r, float g, float b);
Color gfx_color_rgb(struct gfx_region *reg, uint8_t r, uint8_t g, uint8_t b);
void gfx_clear_to_color(struct gfx_region *reg, Color c);
void gfx_clear(struct gfx_region *reg);
void gfx_circle(struct gfx_region *reg, int x, int y, int r, int t, Color c);
void gfx_circle_fill(struct gfx_region *reg, int x, int y, int r, Color c);
void gfx_rectangle(struct gfx_region *reg, int x, int y, int w, int h,
						      int t, Color c);
void gfx_rectangle_fill(struct gfx_region *reg, int x, int y, int w, int h,
								  Color c);
void gfx_line(struct gfx_region *reg, int x1, int y1, int x2, int y2,
						     int t, Color c);
void gfx_update(struct gfx_region *reg);

enum gfx_color {
	WHITE,
	BLACK,
	RED,
	GREEN,
	BLUE,
	YELLOW,

	COLORS
};

struct gfx_color_rgb {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} extern const gfx_colors_rgb[COLORS];

Color gfx_color(struct gfx_region *reg, enum gfx_color color);

void gfx_copy_region_raw(struct gfx_region *reg, int x, int y, int w, int h,
					         size_t bpp, const void *p);
void gfx_copy_raw(struct gfx_region *reg, const void *p, size_t size);

#endif
