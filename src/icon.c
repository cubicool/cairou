#include "cairocks.h"

#include <string.h>
#include <ctype.h>

#include <stdio.h>

/* This is used to make sure we don't pass any valid text flags (that don't make sense
 * for our icon API. */
const int ICON_MASK_X =
	CAIROCKS_ICON_X_LEFT |
	CAIROCKS_ICON_X_CENTER |
	CAIROCKS_ICON_X_RIGHT
;

const int ICON_MASK_Y =
	CAIROCKS_ICON_Y_TOP |
	CAIROCKS_ICON_Y_CENTER |
	CAIROCKS_ICON_Y_BOTTOM
;

static unsigned int _cairocks_icon_get_flags(unsigned int flags) {
	if(!(flags & ICON_MASK_X)) flags |= CAIROCKS_ICON_X_CENTER;

	if(!(flags & ICON_MASK_Y)) flags |= CAIROCKS_ICON_Y_CENTER;

	return flags & (ICON_MASK_X | ICON_MASK_Y);
}

cairo_bool_t cairocks_show_icon(
	cairo_t*        cr,
	cairocks_icon_t icon,
	double          size,
	double          x,
	double          y,
	unsigned int    flags
) {
	return cairocks_show_text(
		cr,
		_cairocks_icon_strings[icon],
		"Glyphicons",
		size,
		x,
		y,
		_cairocks_icon_get_flags(flags)
	);
}

cairo_bool_t cairocks_icon_path(
	cairo_t*        cr,
	cairocks_icon_t icon,
	double          size,
	double          x,
	double          y,
	unsigned int    flags
) {
	return cairocks_text_path(
		cr,
		_cairocks_icon_strings[icon],
		"Glyphicons",
		size,
		x,
		y,
		_cairocks_icon_get_flags(flags)
	);
}

cairo_bool_t cairocks_icon_extents(
	cairo_t*        cr,
	cairocks_icon_t icon,
	double          size,
	double          x,
	double          y,
	unsigned int    flags,
	double*         extents
) {
	cairo_text_extents_t text_extents;

	return cairocks_text_extents(
		cr,
		_cairocks_icon_strings[icon],
		"Glyphicons",
		size,
		x,
		y,
		_cairocks_icon_get_flags(flags),
		&text_extents,
		extents
	);
}

cairocks_icon_t cairocks_icon_from_string(const char* icon) {
	char icon_lower[CAIROCKS_LARGEST_ICON_NAME];
	int c;
	int len = strlen(icon);

	if(len > CAIROCKS_LARGEST_ICON_NAME) len = CAIROCKS_LARGEST_ICON_NAME;

	memset(icon_lower, 0, CAIROCKS_LARGEST_ICON_NAME);

	for(c = 0; c < len; c++) icon_lower[c] = tolower(icon[c]);

	for(c = 0; c < CAIROCKS_NUM_ICONS; c++) {
		if(strncmp(icon_lower, _cairocks_icon_names[c], len)) {
			return (cairocks_icon_t)(c);
		}
	}

	return CAIROCKS_ICON_ERROR;
}

