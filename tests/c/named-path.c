#include <cairocks.h>

int main(int argc, char** argv) {
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 512, 512);
	cairo_t* cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cairo_paint(cr);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

	cairo_arc(cr, 0.0, 0.0, 50.0, 0.0, 2.0 * 3.14159);

	cairocks_append_named_path(cr, "circle");
	cairocks_set_named_path(cr, "circle");

	cairo_set_line_width(cr, 10.0);
	cairo_stroke(cr);
	cairo_surface_write_to_png(surface, "foo.png");
	cairo_surface_destroy(surface);
	cairo_destroy(cr);

	return 0;
}

