/* This code is heavily inspired by (and in some parts, borrowed from) the GIF loader in
 * OpenSceneGraph; another project with which I am involved and highly reccommend. :) */

#include "cairocks.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <gif_lib.h>

#if GIFLIB_MAJOR < 5 || GIFLIB_MINOR < 1
#error "You need giflib 5.0 or higher."
#endif

static cairo_user_data_key_t GIF_DATA;

typedef struct _cairocks_gif_private_t {
	unsigned char*   data;
	cairo_bool_t     alloc;
	GifFileType*     gif_file;
	int              bytes_read;
	unsigned int     num_frames;
	cairo_surface_t* surface;
} cairocks_gif_private_t;

static cairocks_gif_private_t* cairocks_gif_private_create() {
	cairocks_gif_private_t* gif_data = (cairocks_gif_private_t*)(malloc(sizeof(cairocks_gif_private_t)));

	gif_data->data       = NULL;
	gif_data->alloc      = FALSE;
	gif_data->gif_file   = NULL;
	gif_data->bytes_read = 0;
	gif_data->num_frames = 0;
	gif_data->surface    = NULL;

	return gif_data;
}

static void cairocks_gif_private_destroy(void* data) {
	int err;

	cairocks_gif_private_t* gif_data = (cairocks_gif_private_t*)(data);

	if(gif_data->alloc && gif_data->data) free(gif_data->data);

	if(gif_data->gif_file) DGifCloseFile(gif_data->gif_file, &err);

	cairo_surface_destroy(gif_data->surface);

	free(data);
}

static cairocks_gif_private_t* cairocks_gif_private_get(cairo_surface_t* surface) {
	cairocks_gif_private_t* gif_data = (cairocks_gif_private_t*)(cairo_surface_get_user_data(surface, &GIF_DATA));

	return gif_data;
}

static int cairocks_gif_private_read(GifFileType* gif_file, GifByteType* data, int length) {
	cairocks_gif_private_t* gif_data = (cairocks_gif_private_t*)(gif_file->UserData);

	memcpy(data, gif_data->data + gif_data->bytes_read, length);

	gif_data->bytes_read += length;

	return length;
}

static void cairocks_gif_private_reset(cairocks_gif_private_t* gif_data) {
	int err;

	if(gif_data->gif_file) DGifCloseFile(gif_data->gif_file, &err);

	gif_data->bytes_read = 0;
	gif_data->gif_file   = DGifOpen(gif_data, cairocks_gif_private_read, &err);
}

static void cairocks_gif_private_decode_row(
	GifFileType*   gif_file,
	unsigned char* buffer,
	unsigned char* rowdata,
	int            x,
	int            y,
	int            len,
	int            transparent
) {
	GifColorType*   cmentry;
	ColorMapObject* colormap;
	int             colormapsize;
	unsigned char   col;
	unsigned char*  ptr;

	/* The following code would load the image upside down:
	y = gif_file->SHeight - (y + 1);
	TODO: Investigate make this an option to simplify usage with OpenGL. */

	ptr          = buffer + (gif_file->SWidth * y + x) * 4;
	colormap     = gif_file->Image.ColorMap ? gif_file->Image.ColorMap : gif_file->SColorMap;
	colormapsize = colormap ? colormap->ColorCount : 255;

	while(len--) {
		col = *rowdata++;

		if(col >= colormapsize) col = 0;

		if(col == transparent) ptr += 3;

		else {
			cmentry = colormap ? &colormap->Colors[col] : NULL;

			if(cmentry) {
				*ptr++ = cmentry->Blue;
				*ptr++ = cmentry->Green;
				*ptr++ = cmentry->Red;
			}

			else {
				*ptr++ = col;
				*ptr++ = col;
				*ptr++ = col;
			}
		}

		*ptr++ = 0xFF;
	}
}

typedef enum _cairocks_gif_private_next_result {
	SUCCESS,
	FAILURE,
	RESET
} cairocks_gif_private_next_result;

static cairocks_gif_private_next_result _cairocks_gif_private_next(
	cairo_surface_t*        surface,
	cairocks_gif_private_t* gif_data,
	cairo_bool_t            set_bg,
	cairo_bool_t            load_data,
	unsigned int*           delay
) {
	int i;

	unsigned char* rowdata = 0;
	unsigned char* buffer  = 0;

	GifRecordType record;

	int transparent = -1;

	cairocks_gif_private_next_result result = FAILURE;

	/* The way an interlaced image should be read - offsets and jumps */
	static int interlacedoffset[] = { 0, 4, 2, 1 };
	static int interlacedjumps[]  = { 8, 8, 4, 2 };

	if(load_data) buffer = cairo_image_surface_get_data(gif_data->surface);

	rowdata = (unsigned char*)(malloc(gif_data->gif_file->SWidth));

	if(load_data && set_bg) {
		unsigned char bg   = gif_data->gif_file->SBackGroundColor;
		int           size = gif_data->gif_file->SWidth * gif_data->gif_file->SHeight;

		if(gif_data->gif_file->SColorMap && bg < gif_data->gif_file->SColorMap->ColorCount) {
			GifColorType* bgcolor = &gif_data->gif_file->SColorMap->Colors[bg];

			for(i = 0; i < size; i++) {
				buffer[(i * 4)]     = bgcolor->Blue;
				buffer[(i * 4) + 1] = bgcolor->Green;
				buffer[(i * 4) + 2] = bgcolor->Red;
				buffer[(i * 4) + 3] = 0xFF;
			}
		}

		else memset(buffer, 0x00, size * 4);
	}

	do {
		if(DGifGetRecordType(gif_data->gif_file, &record) == GIF_ERROR) break;

		if(record == IMAGE_DESC_RECORD_TYPE) {
			int row;
			int col;
			int width;
			int height;

			if(DGifGetImageDesc(gif_data->gif_file) == GIF_ERROR) break;

			row    = gif_data->gif_file->Image.Top;
			col    = gif_data->gif_file->Image.Left;
			width  = gif_data->gif_file->Image.Width;
			height = gif_data->gif_file->Image.Height;

			if(gif_data->gif_file->Image.Interlace) {
				int j;

				for(i = 0; i < 4; i++) {
					for(j = row + interlacedoffset[i]; j < row + height; j += interlacedjumps[i]) {
						if(DGifGetLine(gif_data->gif_file, rowdata, width) == GIF_ERROR) break;

						if(load_data) cairocks_gif_private_decode_row(
							gif_data->gif_file,
							buffer,
							rowdata,
							col,
							j,
							width,
							transparent
						);
					}
				}
			}

			else {
				for (i = 0; i < height; i++, row++) {
					if(DGifGetLine(gif_data->gif_file, rowdata, width) == GIF_ERROR) break;

					if(load_data) cairocks_gif_private_decode_row(
						gif_data->gif_file,
						buffer,
						rowdata,
						col,
						row,
						width,
						transparent
					);
				}
			}

			result = SUCCESS;

			break;
		}

		else if(record == EXTENSION_RECORD_TYPE) {
			int          extcode;
			GifByteType* extension;

			if(DGifGetExtension(gif_data->gif_file, &extcode, &extension) == GIF_ERROR) break;

			else if(extcode == GRAPHICS_EXT_FUNC_CODE) {
				if(extension[0] >= 4 && extension[1] & 0x1) transparent = extension[4];

				else transparent = -1;

				/* minimum unit 1/100s, so 8 here means 8/100s */
				if(delay) *delay = (extension[3] << 8) + extension[2];
			}

			while(extension != NULL) {
				if(DGifGetExtensionNext(gif_data->gif_file, &extension) == GIF_ERROR) break;
			}
		}
	}

	while(record != TERMINATE_RECORD_TYPE);

	if(record == TERMINATE_RECORD_TYPE) result = RESET;

	free(rowdata);

	/* Do the finaly copy from internal, hidden surface to the external surface and
	mark it as dirty. */
	if(load_data) {
		memcpy(
			cairo_image_surface_get_data(surface),
			buffer,
			cairo_image_surface_get_stride(surface) * cairo_image_surface_get_height(surface)
		);

		cairo_surface_mark_dirty(surface);
	}

	return result;
}

static cairocks_gif_private_next_result cairocks_gif_private_next(
	cairo_surface_t*        surface,
	cairocks_gif_private_t* gif_data,
	cairo_bool_t            set_bg,
	unsigned int*           delay
) {
	return _cairocks_gif_private_next(surface, gif_data, set_bg, TRUE, delay);
}

static unsigned int cairocks_gif_private_get_num_frames(cairocks_gif_private_t* gif_data) {
	unsigned int count = 0;

	while(_cairocks_gif_private_next(NULL, gif_data, FALSE, FALSE, NULL) != RESET) count++;

	return count;
}

static cairo_surface_t* cairocks_gif_private_surface_create(cairocks_gif_private_t* gif_data) {
	cairo_surface_t* surface;
	int              width;
	int              height;
	int              err;

	gif_data->gif_file = DGifOpen(gif_data, cairocks_gif_private_read, &err);

	if(!gif_data->gif_file) return NULL;

	width   = gif_data->gif_file->SWidth;
	height  = gif_data->gif_file->SHeight;
	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);

	/* This is where the ACTUAL compositing of GIF frame to frame is done; next() simply copies
	this data into the surface accessible to Cairo. The reason we do this is to allow further
	compositing inside Cairo. */
	gif_data->surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);

	/* We'll do one quick pass through here to determine how many frames we think we have. */
	gif_data->num_frames = cairocks_gif_private_get_num_frames(gif_data);

	if(!gif_data->num_frames) return NULL;

	cairocks_gif_private_reset(gif_data);

	cairo_surface_set_user_data(surface, &GIF_DATA, gif_data, cairocks_gif_private_destroy);

	/* Let our private "nexting" function set the data every time it should. */
	cairocks_gif_private_next(surface, gif_data, TRUE, NULL);

	return surface;
}

cairo_surface_t* cairocks_gif_surface_create(const char* file) {
	cairocks_gif_private_t* gif_data;
	FILE*                   fp;
	size_t                  size;

	fp = fopen(file, "rb");

	if(!fp) return NULL;

	gif_data = cairocks_gif_private_create();

	fseek(fp, 0, SEEK_END);

	size = ftell(fp);

	gif_data->data  = (unsigned char*)(malloc(size));
	gif_data->alloc = TRUE;

	fseek(fp, 0, SEEK_SET);
	fread(gif_data->data, 1, size, fp);

	return cairocks_gif_private_surface_create(gif_data);
}

cairo_surface_t* cairocks_gif_surface_create_for_data(unsigned char* data, unsigned int size) {
	cairocks_gif_private_t* gif_data = cairocks_gif_private_create();

	gif_data->data  = (unsigned char*)(malloc(size));
	gif_data->alloc = TRUE;

	memcpy(gif_data->data, data, size);

	return cairocks_gif_private_surface_create(gif_data);
}

int cairocks_gif_surface_next(cairo_surface_t* surface) {
	cairocks_gif_private_t*          gif_data;
	cairocks_gif_private_next_result r;
	unsigned int                     delay = 0;

	gif_data = cairocks_gif_private_get(surface);

	if(!gif_data) return -1;

	r = cairocks_gif_private_next(surface, gif_data, FALSE, &delay);

	if(r == FAILURE) return -2;

	else if(r == RESET) {
		cairocks_gif_private_reset(gif_data);

		r = cairocks_gif_private_next(surface, gif_data, TRUE, &delay);
	}

	if(r == FAILURE) return -3;

	return delay;
}

unsigned int cairocks_gif_surface_get_num_frames(cairo_surface_t* surface) {
	cairocks_gif_private_t* gif_data = cairocks_gif_private_get(surface);

	if(!gif_data) return 0;

	return gif_data->num_frames;
}

