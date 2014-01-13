#include "cairocks.h"

#include <stdio.h>
#include <stdlib.h>

#include <jpeglib.h>

static cairo_surface_t* cairocks_surface_from_jpeg_private(struct jpeg_decompress_struct* cinfo) {
	cairo_surface_t* surface = 0;
	unsigned char*   data    = 0;
	unsigned char*   rgb     = 0;

	jpeg_read_header(cinfo, TRUE);
	jpeg_start_decompress(cinfo);

	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, cinfo->image_width, cinfo->image_height);
	data    = cairo_image_surface_get_data(surface);
	rgb     = (unsigned char*)(malloc(cinfo->output_width * cinfo->output_components));

	while(cinfo->output_scanline < cinfo->output_height) {
		unsigned int i;
		int          scanline = cinfo->output_scanline * cairo_image_surface_get_stride(surface);

		jpeg_read_scanlines(cinfo, &rgb, 1);

		for(i = 0; i < cinfo->output_width; i++) {
			int offset = scanline + (i * 4);

			data[offset + 3] = 255;
			data[offset + 2] = rgb[(i * 3)];
			data[offset + 1] = rgb[(i * 3) + 1];
			data[offset    ] = rgb[(i * 3) + 2];
		}
	}

	free(rgb);

	jpeg_finish_decompress(cinfo);
	jpeg_destroy_decompress(cinfo);

	cairo_surface_mark_dirty(surface);

	return surface;
}

cairo_surface_t* cairocks_surface_from_jpeg(const char* file) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr         jerr;
	cairo_surface_t*              surface;
	FILE*                         infile;

	if((infile = fopen(file, "rb")) == NULL) return NULL;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);

	surface = cairocks_surface_from_jpeg_private(&cinfo);

	fclose(infile);

	return surface;
}

cairo_surface_t* cairocks_surface_from_jpeg_data(unsigned char* inbuffer, unsigned int insize) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr         jerr;
	cairo_surface_t*              surface;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, inbuffer, insize);

	surface = cairocks_surface_from_jpeg_private(&cinfo);

	return surface;
}

