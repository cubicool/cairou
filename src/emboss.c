/* Adapted from "Fast Embossing Effects on Raster Image Data"
 *
 * 	Original code by: John Schlag, <jfs@kerner.com>
 * 	Adapted by: Jeremy Moles, <cubicool@gmail.com>
 * 	Further refined by: Andrea Canciani <ranma42@gmail.com>
 */

#include "cairocks.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

unsigned char minc(double d) {
	if(d < 0.0f) return 0;

	else if(d > 1.0) return UCHAR_MAX;

	else return (unsigned char)(d * (double)(UCHAR_MAX));
}

#define OFFSET(x, y, w, h, stride) \
	((x < 0) ? 0 : ((x > w - 1) ? w - 1 : x)) + ((y < 0) ? 0 : ((y > h - 1) ? h - 1 : y)) * stride

static cairo_surface_t* _cairocks_create_embossed_surface(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
) {
	unsigned char* src    = cairo_image_surface_get_data(surface);
	int            w      = cairo_image_surface_get_width(surface);
	int            h      = cairo_image_surface_get_height(surface);
	int            stride = cairo_image_surface_get_stride(surface);

	cairo_surface_t* dst_surface = cairo_image_surface_create(CAIRO_FORMAT_A8, w, h);

	unsigned char* dst = cairo_image_surface_get_data(dst_surface);

	double scale = height / (4.0f * 255.0f);

	double Lx = cos(azimuth) * cos(elevation);
	double Ly = sin(azimuth) * cos(elevation);
	double Lz = sin(elevation);

	int y;
	int x;

	memset(dst, minc(ambient + diffuse * Lz), stride * h);

	/* I'm going to set a minimum requirement size for embossing here for now, since
	the matrix "d" below assumes 3x3. Later, if it's necessary for such small 
	surfaces, we could rectify this. */
	if(w < 4 || h < 4) goto dirty_ret;

	for(y = 0; y < h; y++) {
		for(x = 0; x < w; x++) {
			double       Nx;
			double       Ny;
			double       Nz;
			double       NdotL;
			double       d[3][3];
			int          i;
			int          j;

			for(i = 0; i < 3; i++) {
				for(j = 0; j < 3; j++) {
					d[i][j] = (unsigned char)(src[OFFSET(x + i - 1, y + j - 1, w, h, stride)]);
				}
			}

			/* Compute the normal from the source. */
			Nx  = d[2][0] + (2.0f * d[2][1]) + d[2][2];
			Nx -= d[0][0] + (2.0f * d[0][1]) + d[0][2];
			Nx /= 9;
			Nx *= scale;

			Ny  = d[0][2] + (2.0f * d[1][2]) + d[2][2];
			Ny -= d[0][0] + (2.0f * d[1][0]) + d[2][0];
			Ny /= 9;
			Ny *= scale;

			Nz  = 0.5;

			NdotL = (Nx * Lx) + (Ny * Ly) + (Nz * Lz);
			NdotL = (NdotL > 0.0) ? NdotL : 0.0;

			dst[(y * stride) + x] = minc(ambient + (diffuse * NdotL));
		}
	}

dirty_ret:
	cairo_surface_mark_dirty(dst_surface);

	return dst_surface;
}

/* This function only supports A8 surfaces at the moment, and delegates work to the
// similarly named function above. */
cairo_surface_t* cairocks_emboss_create(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
) {
	if(cairo_image_surface_get_format(surface) != CAIRO_FORMAT_A8) {
		printf("This routine only supports embossing A8 surfaces.\n");

		return 0;
	}

	else return _cairocks_create_embossed_surface(
		surface,
		azimuth,
		elevation,
		height,
		ambient,
		diffuse
	);
}

/* This function only supports A8 surfaces at the moment, and delegates work to the
// similarly named function above. */
cairo_bool_t cairocks_emboss(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
) {
	unsigned int     h      = cairo_image_surface_get_height(surface);
	unsigned int     stride = cairo_image_surface_get_stride(surface);
	cairo_surface_t* tmp;

	if(cairo_image_surface_get_format(surface) != CAIRO_FORMAT_A8) {
		printf("This routine only supports embossing A8 surfaces.\n");

		return FALSE;
	}

	tmp = _cairocks_create_embossed_surface(
		surface,
		azimuth,
		elevation,
		height,
		ambient,
		diffuse
	);

	if(!tmp) return FALSE;

	memcpy(
		cairo_image_surface_get_data(surface),
		cairo_image_surface_get_data(tmp),
		stride * h
	);

	cairo_surface_destroy(tmp);

	return TRUE;
}
