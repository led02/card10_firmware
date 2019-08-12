#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <stdint.h>
#include <string.h>

typedef unsigned int Color;

enum orientation {
	FB_O_0,
	FB_O_90,
	FB_O_180,
	FB_O_270,
};

struct framebuffer {
	void *data;
	size_t width;
	size_t height;
	size_t stride;
	int orientation;

	Color (*encode_color_rgb)(struct framebuffer *fb, uint8_t r, uint8_t g,
								    uint8_t b);
	void (*update)(struct framebuffer *fb);
};

size_t fb_bytes_per_pixel(const struct framebuffer *fb);
void *fb_pixel(struct framebuffer *fb, int x, int y);
void fb_setpixel(struct framebuffer *fb, int x, int y, Color c);
void fb_clear_to_color(struct framebuffer *fb, Color c);
void fb_clear(struct framebuffer *fb);
Color fb_encode_color_rgb(struct framebuffer *fb, int r, int g, int b);
Color fb_encode_color_rgb_f(struct framebuffer *fb, float r, float g,
							    float b);
void fb_copy_raw(struct framebuffer *fb, const void *data,
					     size_t size);
void fb_update(struct framebuffer *fb);

#endif
