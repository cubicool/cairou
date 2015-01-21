#include "common.h"

const char* test_name = "icon";

void test_function(int argc, char** argv, cairo_t* cr) {
	/* double extents[4]; */

	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cairo_paint(cr);
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_move_to(cr, 256.0, 0.0);
	cairo_line_to(cr, 256.0, 512.0);
	cairo_stroke(cr);
	cairo_move_to(cr, 0.0, 256.0);
	cairo_line_to(cr, 512.0, 256.0);
	cairo_stroke(cr);

	if(!cairocks_show_icon(cr, CAIROCKS_ICON_LEAF, 200.0, 256.0, 256.0, 0)) {
		printf("There was ome funky error!\n");

		return;
	}

	/* cairocks_icon_extents(cr, CAIROCKS_ICON_LEAF, 200.0, 256.0, 256.0, 0, extents); */
}

