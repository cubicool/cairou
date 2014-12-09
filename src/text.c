#include "cairocks.h"

#include <stdio.h>

static cairo_bool_t cairocks_text_private_init(
	cairo_t*              cr,
	const char*           utf8,
	const char*           font,
	double                size,
	int                   flags,
	cairo_text_extents_t* extents
) {
	cairo_select_font_face(
		cr,
		font,
		flags & CAIROCKS_ITALIC ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
		flags & CAIROCKS_BOLD ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL
	);

	cairo_set_font_size(cr, size);

	/* Get the extents, so we can use them to properly position our text.
	 * TODO: We need to find all \n occurances, and properly break/extent these
	 * individual chunks. */
	cairo_text_extents(cr, utf8, extents);

	/* printf(">> %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n",
		extents->x_bearing,
		extents->y_bearing,
		extents->width,
		extents->height,
		extents->x_advance,
		extents->y_advance
	); */

	return TRUE;
}

static void cairocks_text_private_get_translate(
	int                   flags,
	cairo_text_extents_t* extents,
	double*               tx,
	double*               ty
) {
	/* Perform X alignment; default is BASELINE. */
	if(flags & CAIROCKS_X_LEFT) *tx = -extents->x_bearing;

	else if(flags & CAIROCKS_X_RIGHT) *tx = -(extents->width + extents->x_bearing);

	else if(flags & CAIROCKS_X_CENTER) *tx = -(extents->width / 2.0) - extents->x_bearing;

	/* Now we do Y alignment, who also defaults to BASELINE. */
	if(flags & CAIROCKS_Y_BOTTOM) *ty = -(extents->height + extents->y_bearing);

	else if(flags & CAIROCKS_Y_CENTER) *ty = (extents->height / 2.0) -
		(extents->height + extents->y_bearing)
	;

	else if(flags & CAIROCKS_Y_TOP) *ty = -extents->y_bearing;
}

static cairo_bool_t cairocks_text_private_draw(
	cairo_t*    cr,
	const char* utf8,
	const char* font,
	double      size,
	double      x,
	double      y,
	int         flags,
	void        (*function)(cairo_t*, const char*)
) {
	double tx = 0.0;
	double ty = 0.0;

	cairo_text_extents_t extents;

	cairocks_text_private_init(cr, utf8, font, size, flags, &extents);
	cairocks_text_private_get_translate(flags, &extents, &tx, &ty);

	cairo_save(cr);
	cairo_translate(cr, x + tx, y + ty);

	function(cr, utf8);

	cairo_restore(cr);

	return TRUE;
}

cairo_bool_t cairocks_show_text(
	cairo_t*    cr,
	const char* utf8,
	const char* font,
	double      size,
	double      x,
	double      y,
	int         flags
) {
	return cairocks_text_private_draw(cr, utf8, font, size, x, y, flags, cairo_show_text);
}

cairo_bool_t cairocks_text_path(
	cairo_t*    cr,
	const char* utf8,
	const char* font,
	double      size,
	double      x,
	double      y,
	int         flags
) {
	return cairocks_text_private_draw(cr, utf8, font, size, x, y, flags, cairo_text_path);
}

cairo_bool_t cairocks_text_extents(
	cairo_t*              cr,
	const char*           utf8,
	const char*           font,
	double                size,
	double                x,
	double                y,
	int                   flags,
	cairo_text_extents_t* extents,
	double*               rect_extents
) {
	cairocks_text_private_init(cr, utf8, font, size, flags, extents);

	if(rect_extents) {
		double rx;
		double ry;

		/* Calculate the rect X origin. */
		if(flags & CAIROCKS_X_LEFT) rx = 0.0;

		else if(flags & CAIROCKS_X_CENTER) rx = extents->width / 2.0;

		else if(flags & CAIROCKS_X_RIGHT) rx = extents->width;

		else rx = -extents->x_bearing;

		/* Calculate the rect Y origin. */
		if(flags & CAIROCKS_Y_BOTTOM) ry = extents->height;

		else if(flags & CAIROCKS_Y_CENTER) ry = extents->height / 2.0;

		else if(flags & CAIROCKS_Y_TOP) ry = 0.0;

		else ry = -extents->y_bearing;

		/* cairocks_text_private_get_translate(flags, extents, &tx, &ty);
		cairo_save(cr);
		cairo_translate(cr, x + tx, y + ty); */

		/* Finally, the width and height of the rect always match the extent
		 * width and height. */
		rect_extents[0] = x - rx;
		rect_extents[1] = y - ry;
		rect_extents[2] = extents->width;
		rect_extents[3] = extents->height;
	}

	return TRUE;
}
