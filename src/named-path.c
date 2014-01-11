#include "cairocks.h"

static cairo_user_data_key_t PATH_DATA;

typedef struct _cairocks_named_path cairocks_named_path_t;

struct _cairocks_named_path {
	cairo_path_t* path;

	cairocks_named_path_t* next;
};

static cairocks_named_path_t* cairocks_named_path_get(cairo_surface_t* surface) {
	cairocks_gif_private_t* gif_data = (cairocks_gif_private_t*)(cairo_surface_get_user_data(surface, &GIF_DATA));

	return gif_data;
}

void cairocks_append_named_path(cairo_t* cr, const char* named_path) {
	
}

cairo_bool_t cairocks_set_named_path(ciaro_t* cr, const char* named_path) {
}

