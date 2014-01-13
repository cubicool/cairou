#include "cairocks.h"

#include <math.h>

static const cairo_bool_t _cairocks_default_corners[4] = { TRUE, TRUE, TRUE, TRUE };

cairo_bool_t cairocks_rounded_rectangle(
	cairo_t*            cr,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius,
	const cairo_bool_t* corners
) {
	if(cairo_status(cr)) return FALSE;

	if(!corners) corners = _cairocks_default_corners;

	/* top border */
	cairo_move_to(cr, x + radius, y);
	cairo_line_to(cr, x + width - radius, y);

	/* top-right corner */
	if(corners[1]) cairo_arc(
		cr,
		x + width - radius,
		y + radius,
		radius,
		-90.0f * M_PI / 180.0f,
		0.0f * M_PI / 180.0f
	);

	else {
		cairo_line_to(cr, x + width, y);
		cairo_line_to(cr, x + width, y + radius);
	}

	/* right border */
	cairo_line_to(cr, x + width, y + height - radius);

	/* bottom-right corner */
	if(corners[2]) cairo_arc(
		cr,
		x + width - radius,
		y + height - radius,
		radius,
		0.0f * M_PI / 180.0f,
		90.0f * M_PI / 180.0f
	);

	else {
		cairo_line_to(cr, x + width, y + height);
		cairo_line_to(cr, width - radius, y + height);
	}

	/* bottom border */
	cairo_line_to(cr, x + radius, y + height);

	/* bottom-left corner */
	if(corners[3]) cairo_arc(
		cr,
		x + radius,
		y + height - radius,
		radius,
		90.0f * M_PI / 180.0f,
		180.0f * M_PI / 180.0f
	);

	else cairo_line_to(cr, x, y + height);

	/* top-left corner */
	if(corners[0]) cairo_arc(
		cr,
		x + radius,
		y + radius,
		radius,
		180.0f * M_PI / 180.0f,
		270.0f * M_PI / 180.0f
	);

	else {
		cairo_line_to(cr, x, y);
		cairo_line_to(cr, x + radius, y);
	}

	return TRUE;
}

cairo_bool_t cairocks_rounded_rectangle_apply(
	cairo_t*            cr,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius,
	const cairo_bool_t* corners
) {
	if(cairo_status(cr)) return FALSE;

	if(!corners) corners = _cairocks_default_corners;

	cairo_save(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_DEST_IN);

	cairocks_rounded_rectangle(
		cr,
		x,
		y,
		width,
		height,
		radius,
		corners
	);

	cairo_fill(cr);
	cairo_restore(cr);

	return TRUE;
}
