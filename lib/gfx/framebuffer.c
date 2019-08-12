#include "framebuffer.h"

void fb_clear_to_color(struct framebuffer *fb, Color c)
{
	for (int y = 0; y < fb->height; y++) {
		for (int x = 0; x < fb->width; x++)
			fb_setpixel(fb, x, y, c);
	}
}

void fb_clear(struct framebuffer *fb)
{
	Color c = fb->encode_color_rgb(fb, 0, 0, 0);
	fb_clear_to_color(fb, c);
}

Color fb_encode_color_rgb(struct framebuffer *fb, int r, int g, int b)
{
	return fb->encode_color_rgb(fb, r, g, b);
}

Color fb_encode_color_rgb_f(struct framebuffer *fb, float r, float g, float b)
{
	float r8 = r > 1.0f ? 1.0f : (uint8_t)(r * 255.0f);
	float g8 = g > 1.0f ? 1.0f : (uint8_t)(g * 255.0f);
	float b8 = b > 1.0f ? 1.0f : (uint8_t)(b * 255.0f);

	r8 = r8 < .0f ? .0f : r8;
	g8 = g8 < .0f ? .0f : g8;
	b8 = b8 < .0f ? .0f : b8;

	return fb->encode_color_rgb(fb, r8, g8, b8);
}

void fb_copy_raw(struct framebuffer *fb, const void *data, size_t size)
{
	size_t to_copy = size < fb->stride ? size : fb->stride;
	memcpy(fb->data, data, to_copy);
}

void fb_update(struct framebuffer *fb)
{
	fb->update(fb);
}

size_t fb_bytes_per_pixel(const struct framebuffer *fb)
{
	const int pixels = fb->height * fb->width;
	return fb->stride / pixels;
}

void *fb_pixel(struct framebuffer *fb, int x, int y)
{
	int xo;
	int yo;

	switch (fb->orientation) {
	case FB_O_0:
		xo = x;
		yo = y;
		break;
	case FB_O_90:
		xo = fb->width - y - 1;
		yo = x;
		break;
	case FB_O_180:
		xo = fb->width - x - 1;
		yo = fb->height - y - 1;
		break;
	case FB_O_270:
		xo = y;
		yo = fb->height - x - 1;
		break;
	default:
		return NULL;
	}

	if (xo < 0 || yo < 0)
		return NULL;
	if (xo >= fb->width || yo >= fb->height)
		return NULL;

	const size_t bpp = fb_bytes_per_pixel(fb);
	return (void *)(fb->data) + yo * fb->width * bpp + xo * bpp;
}

void fb_setpixel(struct framebuffer *fb, int x, int y, Color c)
{
	uint8_t *pixel = fb_pixel(fb, x, y);
	if (pixel == NULL)
		return;

	const uint8_t *color = (const uint8_t *)(&c);
	const size_t bpp     = fb_bytes_per_pixel(fb);
	switch (bpp) {
	default:
	case 2:
		pixel[1] = color[0];
		pixel[0] = color[1];
	}
}
