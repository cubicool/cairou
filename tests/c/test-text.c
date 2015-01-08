#include "common.h"

const char* test_name = "text";

void test_function(cairo_t* cr) {
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cairo_paint(cr);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

	cairocks_show_text(cr, "Foo\nBar\nBaz", "Lato", 40.0, 256.0, 100.0, (
		CAIROCKS_X_CENTER |
		CAIROCKS_BOLD
	));
}

