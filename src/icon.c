#include "cairou.h"

#include <string.h>
#include <ctype.h>

#include <stdio.h>

/* This is used to make sure we don't pass any valid text flags (that don't make sense
 * for our icon API. */
const int ICON_MASK_X =
	CAIROU_ICON_X_LEFT |
	CAIROU_ICON_X_CENTER |
	CAIROU_ICON_X_RIGHT
;

const int ICON_MASK_Y =
	CAIROU_ICON_Y_TOP |
	CAIROU_ICON_Y_CENTER |
	CAIROU_ICON_Y_BOTTOM
;

static unsigned int _cairou_icon_get_flags(unsigned int flags) {
	if(!(flags & ICON_MASK_X)) flags |= CAIROU_ICON_X_CENTER;

	if(!(flags & ICON_MASK_Y)) flags |= CAIROU_ICON_Y_CENTER;

	return flags & (ICON_MASK_X | ICON_MASK_Y);
}

cairo_bool_t cairou_show_icon(
	cairo_t*        cr,
	cairou_icon_t icon,
	double          size,
	double          x,
	double          y,
	unsigned int    flags
) {
	return cairou_show_text(
		cr,
		_cairou_icon_strings[icon],
		"Glyphicons",
		size,
		x,
		y,
		_cairou_icon_get_flags(flags)
	);
}

cairo_bool_t cairou_icon_path(
	cairo_t*        cr,
	cairou_icon_t icon,
	double          size,
	double          x,
	double          y,
	unsigned int    flags
) {
	return cairou_text_path(
		cr,
		_cairou_icon_strings[icon],
		"Glyphicons",
		size,
		x,
		y,
		_cairou_icon_get_flags(flags)
	);
}

cairo_bool_t cairou_icon_extents(
	cairo_t*        cr,
	cairou_icon_t icon,
	double          size,
	double          x,
	double          y,
	unsigned int    flags,
	double*         extents
) {
	cairo_text_extents_t text_extents;

	return cairou_text_extents(
		cr,
		_cairou_icon_strings[icon],
		"Glyphicons",
		size,
		x,
		y,
		_cairou_icon_get_flags(flags),
		&text_extents,
		extents
	);
}

cairou_icon_t cairou_icon_from_string(const char* icon) {
	char icon_lower[CAIROU_LARGEST_ICON_NAME];
	int c;
	int len = strlen(icon);

	if(len > CAIROU_LARGEST_ICON_NAME) len = CAIROU_LARGEST_ICON_NAME;

	memset(icon_lower, 0, CAIROU_LARGEST_ICON_NAME);

	for(c = 0; c < len; c++) icon_lower[c] = tolower(icon[c]);

	for(c = 0; c < CAIROU_NUM_ICONS; c++) {
		if(strncmp(icon_lower, _cairou_icon_names[c], len)) {
			return (cairou_icon_t)(c);
		}
	}

	return CAIROU_ICON_ERROR;
}

