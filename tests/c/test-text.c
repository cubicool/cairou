#include "common.h"

const char* test_name = "text";

void test_function(int argc, char** argv, cairo_t* cr) {
	/* cairo_text_extents_t extents; */

	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cairo_paint(cr);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_set_line_width(cr, 1.0);
	cairo_move_to(cr, 256.5, 0.0);
	cairo_line_to(cr, 256.5, 512.0);
	cairo_stroke(cr);
	cairo_move_to(cr, 0.0, 256.5);
	cairo_line_to(cr, 512.0, 256.5);
	cairo_stroke(cr);

	cairocks_show_text(cr, "abcd\nwxyz\n123456\nfoobary", "Lao", 40.0, 256.0, 256.0, (
		CAIROCKS_TEXT_X_BASELINE |
		CAIROCKS_TEXT_Y_BASELINE |
		CAIROCKS_TEXT_ITALIC |
		CAIROCKS_TEXT_BOLD
	));

	/* cairocks_text_extents(cr, "JEREMY", "Lato", 40.0, 256.0, 256.0, (
		CAIROCKS_TEXT_X_BASELINE |
		CAIROCKS_TEXT_Y_BASELINE |
		CAIROCKS_TEXT_BOLD
	), &extents, NULL); */
}

