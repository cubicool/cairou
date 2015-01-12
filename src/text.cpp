#include "cairocks.h"
#include "utf8.h"

#include <cstring>
#include <vector>

// Since we use the variable name "utf8" a great deal, we need to re-assign
// the namespace defined by this header above to something more compatible with
// our code.
namespace UTF8 = utf8;

// TODO: REMOVE ME!
#include <iostream>

// This structure holds all the data we need to easily account for newlines in
// blocks of text and to use those substrings (as delimited by those newlines)
// with the Cairo API. We take a fairly easy approach wherein we essentially
// break the string up into small strings by storing a start position for each
// substring and replacing the newlines we find in our copy with NULL terminators.
//
// TODO: Convert the next()'ing idiom into the standard C++ iterator API.
class cairocks_text_private_t {
public:
	typedef std::vector<unsigned int> starting_points;

	cairocks_text_private_t(const char* utf8_ro):
	utf8(NULL) {
		unsigned int size = std::strlen(utf8_ro) + 1;

		// Allocate our copy of the UTF8 string.
		utf8 = new char[size];

		std::strcpy(utf8, utf8_ro);

		unsigned int pos = 0;
		unsigned int last = 0;
		const char* c = utf8_ro;
		const char* end = utf8_ro + size;

		while(c < end) {
			if(UTF8::next(c, end) == '\n') {
				starts.push_back(last);

				utf8[pos] = 0;

				last = pos + 1;
			}

			pos++;
		}

		n = starts.begin();
	}

	~cairocks_text_private_t() {
		if(utf8) delete[] utf8;
	}

	const char* next() {
		if(n == starts.end()) {
			n = starts.begin();

			return NULL;
		}

		const char* str = static_cast<const char*>(&utf8[*n]);

		n++;

		return str;
	}

private:
	char* utf8;
	starting_points starts;
	starting_points::const_iterator n;
};

// Initializes the Cairo state for the passed-in context and returns a private
// structure containing the data necessary to properly draw the string.
static cairocks_text_private_t* cairocks_text_private_init(
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
	cairo_text_extents(cr, utf8, extents);

	cairocks_text_private_t* data = NULL;

	try {
		data = new cairocks_text_private_t(utf8);
	}

	catch(...) {
		data = NULL;
	}

	return data;
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

	cairocks_text_private_t* text = cairocks_text_private_init(cr, utf8, font, size, flags, &extents);

	cairocks_text_private_get_translate(flags, &extents, &tx, &ty);

	// TODO: Print shit...
	for(const char* str = text->next(); str; str = text->next()) {
		std::cout << str << std::endl;
	}

	if(!(flags & CAIROCKS_NO_SAVE_RESTORE)) cairo_save(cr);

	cairo_translate(cr, x + tx, y + ty);

	function(cr, utf8);

	if(!(flags & CAIROCKS_NO_SAVE_RESTORE)) cairo_restore(cr);

	delete text;

	return TRUE;
}

extern "C" {

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
		double rx = 0.0;
		double ry = 0.0;

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

		/* Finally, the width and height of the rect always match the extent
		 * width and height. */
		rect_extents[0] = x - rx;
		rect_extents[1] = y - ry;
		rect_extents[2] = extents->width;
		rect_extents[3] = extents->height;
	}

	return TRUE;
}

}

