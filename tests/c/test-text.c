#include "common.h"

const char* test_name = "text";
const char* test_string = "ABCD\nwxyz\n1 2 3 4 5\n\nCairocks!";

void test_function(int argc, char** argv, cairo_t* cr) {
	cairo_text_extents_t extents;
	double rect_extents[4];

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

	cairocks_show_text(cr, test_string, "Sans", 40.0, 256.0, 256.0, (
		CAIROCKS_TEXT_X_CENTER |
		CAIROCKS_TEXT_Y_CENTER |
		CAIROCKS_TEXT_ITALIC |
		CAIROCKS_TEXT_BOLD
	));

	cairocks_text_extents(cr, "JEREMY", "Lato", 40.0, 256.0, 256.0, (
		CAIROCKS_TEXT_X_CENTER |
		CAIROCKS_TEXT_Y_CENTER |
		CAIROCKS_TEXT_ITALIC |
		CAIROCKS_TEXT_BOLD
	), &extents, rect_extents);

	printf("extents: x_bearing=%3.1f y_bearing=%3.1f width=%3.1f height=%3.1f\n",
		extents.x_bearing,
		extents.y_bearing,
		extents.width,
		extents.height
	);

	printf("rect_extents: x=%3.1f y=%3.1f width=%3.1f height=%3.1f\n",
		rect_extents[0],
		rect_extents[1],
		rect_extents[2],
		rect_extents[3]
	);
}

