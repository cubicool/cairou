#include "common.h"

int main(int argc, char** argv) {
	char png[1024];

	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 512, 512);
	cairo_t* cr = cairo_create(surface);

	test_function(cr);

	sprintf(png, "%s.png", test_name);

	cairo_surface_write_to_png(surface, png);
	cairo_surface_destroy(surface);
	cairo_destroy(cr);

	return 0;
}
