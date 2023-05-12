#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
	// TODO Fill in
	int i = round(x);
	int j = round(y);
	return get_pixel(im, i, j, c);
}

image nn_resize(image im, int w, int h)
{
	// TODO Fill in (also fix that first line)
	if (!w || !h)
	{
		return make_image(3, 3, 1);
	}
	else
	{
		image nn_im = make_image(w, h, im.c);
		float value, kx, bx, ky, by, xo, yo;
		kx = (float)im.w / (float)w;
		bx = (kx - 1) * 0.5;
		ky = (float)im.h / (float)h;
		by = (ky - 1) * 0.5;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int c = 0; c < im.c; c++)
				{
					xo = x * kx + bx;
					yo = y * ky + by;
					value = nn_interpolate(im, xo, yo, c);
					set_pixel(nn_im, x, y, c, value);
				}
			}
		}
		return nn_im;
	}
}

float bilinear_interpolate(image im, float x, float y, int c)
{
	// TODO
	float v0, v1, v2, v3, p, q, k0, k1, k2, k3,value;
	v0 = get_pixel(im, floor(x), floor(y),c);
	v1 = get_pixel(im, ceil(x), floor(y),c);
	v2 = get_pixel(im, floor(x), ceil(y),c);
	v3 = get_pixel(im, ceil(x), ceil(y),c);
	p = x - floor(x);
	q = y - floor(y);
	k0 = p * q;
	k1 = (1 - p) * q;
	k2 = (1 - q) * p;
	k3 = (1 - p) * (1 - q);
	value = k0 * v3 + k3 * v0 + k1 * v2 + k2 * v1;
	return value;
}

image bilinear_resize(image im, int w, int h)
{
	// TODO
	if (!w || !h)
	{
		return make_image(3, 3, 1);
	}
	else
	{
		image bl_im = make_image(w, h, im.c);
		float value, kx, bx, ky, by, xo, yo;
		kx = (float)im.w / (float)w;
		bx = (kx - 1) * 0.5;
		ky = (float)im.h / (float)h;
		by = (ky - 1) * 0.5;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int c = 0; c < im.c; c++)
				{
					xo = x * kx + bx;
					yo = y * ky + by;
					value = bilinear_interpolate(im, xo, yo, c);
					set_pixel(bl_im, x, y, c, value);
				}
			}
		}
		return bl_im;
	}
}
