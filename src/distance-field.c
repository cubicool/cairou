/* Adapted from code in the public domain by Rim van Wersch.
 * http://www.xnainfo.com/content.php?content=44
 */

#include "cairocks.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_VALUE 9999
#define MIN_VALUE -9999

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

static double separation(double x1, double y1, double x2, double y2) {
	double dx = x1 - x2;
	double dy = y1 - y2;

	return (double)(sqrt(dx*dx + dy*dy));
}

static double find_signed_distance(
	int              px,
	int              py,
	cairo_surface_t* surface,
	int              scan_width,
	int              scan_height
) {
	int            width         = cairo_image_surface_get_width(surface);
	unsigned char* data          = cairo_image_surface_get_data(surface);
	unsigned char  base          = data[(px * width) + py];
	int            base_solid    = base > 0;
	double         closest       = MAX_VALUE;
	int            closest_valid = 0;
	int            start_x       = px - (scan_width / 2);
	int            end_x         = start_x + scan_width;
	int            start_y       = py - (scan_height / 2);
	int            end_y         = start_y + scan_height;

	int x;
	int y;

	for(x = start_x; x < end_x; x++) {
		if(x < 0 || x >= width) continue;

		for(y = start_y; y < end_y; y++) {
			unsigned char c;

			if(y < 0 || y >= width) continue;

			c = data[(x * width) + y];

			if(base_solid) {
				if(c == 0) {
					double dist = separation(px, py, x, y);

					if (dist < closest) {
						closest       = dist;
						closest_valid = 1;
					}
				}
			}

			else {
				if(c > 0) {
					double dist = separation(px, py, x, y);

					if(dist < closest) {
						closest       = dist;
						closest_valid = 1;
					}
				}
			}
		}
	}

	if(base_solid) {
		if(closest_valid) return closest;

		else return MAX_VALUE;
	}

	else {
		if(closest_valid) return -closest;

		else return MIN_VALUE;
	}
}

cairo_surface_t* cairocks_distance_field_create(
	cairo_surface_t* surface,
	int              scan_size,
	int              block_size
) {
	int surface_width  = cairo_image_surface_get_width(surface);
	int surface_height = cairo_image_surface_get_height(surface);
	int width          = surface_width / block_size;
	int height         = surface_height / block_size;

	double* distances = 0;

	unsigned char* output = 0;

	int x;
	int y;
	int stride;

	double mind  = 0.0f;
	double maxd  = 0.0f;
	double scale = 0.0f;

	/* Image MUST be square (for now). */
	if(surface_width != surface_height) return 0;

	/* Furthermore, the image must be evenly divisible by the block_size. */
	if(surface_width % block_size) return 0;

	distances = (double*)(malloc(width * height * sizeof(double)));

	for(x = 0; x < width; x++) {
		for(y = 0; y < height; y++) {
			distances[(x * width) + y] = find_signed_distance(
				(x * block_size) + (block_size / 2),
				(y * block_size) + (block_size / 2),
				surface,
				scan_size,
				scan_size
			);
		}
	}

	for(x = 0; x < width; x++) {
		for(y = 0; y < height; y++) {
			double d = distances[(x * width) + y];

			if(d != MAX_VALUE && d > maxd) maxd = d;
		}
	}

	for(x = 0; x < width; x++) {
		for(y = 0; y < height; y++) {
			double d = distances[(x * width) + y];

			if(d != MIN_VALUE && d < mind) mind = d;
		}
	}

	scale = max(fabs(mind), fabs(maxd));

	for(x = 0; x < width; x++) {
		for(y = 0; y < height; y++) {
			double d = distances[(x * width) + y];

			if(d == MAX_VALUE) d = 1.0f;

			else if (d == MIN_VALUE) d = 0.0f;

			else {
				d /= scale;
				d /= 2.0f;
				d += 0.5f;
			}

			distances[(x * width) + y] = d;
		}
	}

	/* Here is where things get tricky. Since we use A8 surface, the stride may actually
	be bigger than what we expect, so we'll need to make sure we set the data appropriately. */
	stride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, width);
	output = (unsigned char*)(malloc(stride * height));

	memset(output, 0, stride * height);

	for(x = 0; x < width; x++) {
		for(y = 0; y < height; y++) {
			unsigned int    offset = (x * width) + y;
			volatile double d      = distances[offset];

			if(d != d) d = 0.0f;

			output[offset + (x * (stride - width))] = (unsigned char)(floor(d * 255));
		}
	}

	free(distances);

	return cairo_image_surface_create_for_data(output, CAIRO_FORMAT_A8, width, height, stride);
}

