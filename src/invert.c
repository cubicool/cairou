#include "cairocks.h"

#include <stdio.h>

cairo_bool_t cairocks_a8_invert(cairo_surface_t* surface) {
	unsigned char* src    = cairo_image_surface_get_data(surface);
	unsigned int   width  = cairo_image_surface_get_width(surface);
	unsigned int   height = cairo_image_surface_get_height(surface);
	unsigned int   i;

	if(cairo_image_surface_get_format(surface) != CAIRO_FORMAT_A8) {
		printf("This routine only supports inverting A8 surfaces.\n");

		return FALSE;
	}

	for(i = 0; i < width * height; i++) src[i] = 255 - src[i];

	return TRUE;
}

