#include <cairocks.h>
#include <stdio.h>

int main(int argc, char** argv) {
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 512, 512);
	cairo_t* cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cairo_paint(cr);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

	cairocks_show_text(cr, "Foo\nBar\nBaz", "Lato", 40.0, 256.0, 100.0, (
		CAIROCKS_X_CENTER |
		CAIROCKS_BOLD
	));

	cairo_surface_write_to_png(surface, "foo.png");
	cairo_surface_destroy(surface);
	cairo_destroy(cr);

	return 0;
}
