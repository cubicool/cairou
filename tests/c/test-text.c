#include "common.h"

const char* test_name = "text";

void test_function(int argc, char** argv, cairo_t* cr) {
	cairo_text_extents_t extents;

	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cairo_paint(cr);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_move_to(cr, 256.0, 0.0);
	cairo_line_to(cr, 256.0, 512.0);
	cairo_stroke(cr);
	cairo_move_to(cr, 0.0, 256.0);
	cairo_line_to(cr, 512.0, 256.0);
	cairo_stroke(cr);

	cairocks_show_text(cr, "JEREMY", "Lato", 40.0, 256.0, 256.0, (
		CAIROCKS_X_BASELINE |
		CAIROCKS_Y_BASELINE |
		CAIROCKS_BOLD
	));

	cairocks_text_extents(cr, "JEREMY", "Lato", 40.0, 256.0, 256.0, (
		CAIROCKS_X_BASELINE |
		CAIROCKS_Y_BASELINE |
		CAIROCKS_BOLD
	), &extents, NULL);
}

