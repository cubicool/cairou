#include "cairocks.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/* This blur function was originally created my MacSlow and published on his website:
 * http://macslow.thepimp.net. I'm not entirely sure he's proud of it, but it has
 * proved immeasurably useful for me. */

static double* create_kernel(double radius, double deviation) {
	int     size    = 2 * (int)(radius) + 1;
	double* kernel  = (double*)(malloc(sizeof(double) * (size + 1)));
	double  radiusf = fabs(radius) + 1.0f;
	double  value   = -radius;
	double  sum     = 0.0f;
	int     i;

	if(!kernel) return 0;

	if(deviation == 0.0f) deviation = sqrt(
		-(radiusf * radiusf) / (2.0f * log(1.0f / 255.0f))
	);

	kernel[0] = size;

	for(i = 0; i < size; i++) {
		kernel[1 + i] = 
			1.0f / (2.506628275f * deviation) *
			exp(-((value * value) / (2.0f * (deviation * deviation))))
		;

		sum   += kernel[1 + i];
		value += 1.0f;
	}

	for(i = 0; i < size; i++) kernel[1 + i] /= sum;

	return kernel;
}

cairo_surface_t* cairocks_gaussian_blur_create(
	cairo_surface_t* surface,
	double           radius,
	double           deviation
) {
	cairo_format_t format = cairo_image_surface_get_format(surface);
	unsigned int   width  = cairo_image_surface_get_width(surface);
	unsigned int   height = cairo_image_surface_get_height(surface);
	unsigned int   stride = cairo_image_surface_get_stride(surface);

	cairo_surface_t* tmp = cairo_image_surface_create(format, width, height);

	if(!tmp) return 0;

	memcpy(
		cairo_image_surface_get_data(surface),
		cairo_image_surface_get_data(tmp),
		height * stride
	);

	if(cairocks_gaussian_blur(surface, radius, deviation)) return tmp;

	return 0;
}

cairo_bool_t cairocks_gaussian_blur(
	cairo_surface_t* surface,
	double           radius,
	double           deviation
) {
	double*        horzBlur = 0;
	double*        vertBlur = 0;
	double*        kernel   = 0;
	unsigned char* data     = 0;
	cairo_format_t format;
	int            width;
	int            height;
	int            stride;
	unsigned int   channels;
	int            iY;
	int            iX;

	if(cairo_surface_status(surface)) return FALSE;

	data   = cairo_image_surface_get_data(surface);
	format = cairo_image_surface_get_format(surface);
	width  = cairo_image_surface_get_width(surface);
	height = cairo_image_surface_get_height(surface);
	stride = cairo_image_surface_get_stride(surface);

	if(format == CAIRO_FORMAT_ARGB32) channels = 4;

	else if(format == CAIRO_FORMAT_RGB24) channels = 3;

	else if(format == CAIRO_FORMAT_A8) channels = 1;

	else return FALSE;

	horzBlur = (double*)(malloc(sizeof(double) * height * stride));
	vertBlur = (double*)(malloc(sizeof(double) * height * stride));
	kernel   = create_kernel(radius, deviation);

	if(!horzBlur || !vertBlur || !kernel) return FALSE;

	/* Horizontal pass. */
	for(iY = 0; iY < height; iY++) {
		for(iX = 0; iX < width; iX++) {
			double red    = 0.0f;
			double green  = 0.0f;
			double blue   = 0.0f;
			double alpha  = 0.0f;
			int    offset = (int)(kernel[0]) / -2;
			int    baseOffset;
			int    i;

			for(i = 0; i < (int)(kernel[0]); i++) {
				unsigned char* dataPtr = 0;
				int            x       = iX + offset;
				double         kernip1;

				if(x < 0 || x >= width) continue;

				dataPtr = &data[iY * stride + x * channels];
				kernip1 = kernel[i + 1];

				if(channels == 1) alpha += kernip1 * dataPtr[0];

				else {
					if(channels == 4) alpha += kernip1 * dataPtr[3];

					red   += kernip1 * dataPtr[2];
					green += kernip1 * dataPtr[1];
					blue  += kernip1 * dataPtr[0];
				}

				offset++;
			}

			baseOffset = iY * stride + iX * channels;

			if(channels == 1) horzBlur[baseOffset] = alpha;

			else {
				if(channels == 4) horzBlur[baseOffset + 3] = alpha;

				horzBlur[baseOffset + 2] = red;
				horzBlur[baseOffset + 1] = green;
				horzBlur[baseOffset]     = blue;
			}
		}
	}

	/* Vertical pass. */
	for(iY = 0; iY < height; iY++) {
		for(iX = 0; iX < width; iX++) {
			double red    = 0.0f;
			double green  = 0.0f;
			double blue   = 0.0f;
			double alpha  = 0.0f;
			int    offset = (int)(kernel[0]) / -2;
			int    baseOffset;
			int    i;

			for(i = 0; i < (int)(kernel[0]); i++) {
				double* dataPtr = 0;
				int     y       = iY + offset;
				double  kernip1;

				if(y < 0 || y >= height) {
					offset++;

					continue;
				}

				dataPtr = &horzBlur[y * stride + iX * channels];
				kernip1 = kernel[i + 1];

				if(channels == 1) alpha += kernip1 * dataPtr[0];

				else {
					if(channels == 4) alpha += kernip1 * dataPtr[3];

					red   += kernip1 * dataPtr[2];
					green += kernip1 * dataPtr[1];
					blue  += kernip1 * dataPtr[0];
				}

				offset++;
			}

			baseOffset = iY * stride + iX * channels;

			if(channels == 1) vertBlur[baseOffset] = alpha;

			else {
				if(channels == 4) vertBlur[baseOffset + 3] = alpha;

				vertBlur[baseOffset + 2] = red;
				vertBlur[baseOffset + 1] = green;
				vertBlur[baseOffset]     = blue;
			}
		}
	}

	free(kernel);

	for(iY = 0; iY < height; iY++) {
		for(iX = 0; iX < width; iX++) {
			int i = iY * stride + iX * channels;

			if(channels == 1) data[i] = (unsigned char)(vertBlur[i]);

			else {
				if(channels == 4) data[i + 3] = (unsigned char)(
					vertBlur[i + 3]
				);

				data[i + 2] = (unsigned char)(vertBlur[i + 2]);
				data[i + 1] = (unsigned char)(vertBlur[i + 1]);
				data[i]     = (unsigned char)(vertBlur[i]);
			}
		}
	}

	free(horzBlur);
	free(vertBlur);

	return TRUE;
}

