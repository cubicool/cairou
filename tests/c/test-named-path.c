#include "common.h"

const char* test_name = "named-path";

void test_function(int argc, char** argv, cairo_t* cr) {
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cairo_paint(cr);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

	cairo_arc(cr, 0.0, 0.0, 50.0, 0.0, 2.0 * 3.14159);

	cairocks_append_named_path(cr, "circle");
	cairocks_set_named_path(cr, "circle");

	cairo_set_line_width(cr, 10.0);
	cairo_stroke(cr);
}

