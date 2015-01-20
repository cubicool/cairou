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
#include <cstdio>

// This structure holds all the data we need to easily account for newlines in
// blocks of text and to use those substrings (as delimited by those newlines)
// with the Cairo API. We take a fairly easy approach wherein we essentially
// break the string up into small strings by storing a start position for each
// substring and replacing the newlines we find in our copy with NULL terminators.
class cairocks_text_private_t {
public:
	struct line_t {
		line_t(const char* _utf8):
		utf8(_utf8) {
		}

		const char* utf8;
		cairo_text_extents_t extents;
	};

	struct extents_t {
		extents_t():
		width(0.0),
		height(0.0),
		lfo(0.0),
		bfo(0.0) {
		}

		// Converts our own internal extents to something Cairo understands.
		void to_cairo(cairo_text_extents_t* extents) {
			memset(extents, 0, sizeof(cairo_text_extents_t));

			extents->width = width;
			extents->height = height;
			extents->x_bearing = lfo;
			extents->y_bearing = -(height - bfo);
		}

		double width;
		double height;
		double lfo;
		double bfo;
	};

	typedef std::vector<line_t> lines_t;

	cairocks_text_private_t(const char* _utf8, int _flags):
	utf8(0),
	flags(_flags),
	tx(0.0),
	ty(0.0) {
		unsigned int size = std::strlen(_utf8) + 1;

		// Allocate our copy of the UTF8 string.
		utf8 = new char[size];

		std::strcpy(utf8, _utf8);

		unsigned int last = 0;
		const char* c = _utf8;
		const char* end = _utf8 + size;

		// Next through each newline occurance.
		while(c < end) {
			if(UTF8::next(c, end) == '\n') {
				unsigned int p = c - _utf8 - 1;

				utf8[p] = 0;

				lines.push_back(utf8 + last);

				last += p + 1;
			}
		}

		// If the string doesn't end in a newline, go ahead and push the remainder
		// as a new line of text
		if(last < size) lines.push_back(utf8 + last);
	}

	~cairocks_text_private_t() {
		if(utf8) delete[] utf8;
	}

	// TODO: Is size needed, really?
	void init(cairo_t* cr, double size) {
		unsigned int n = 0;

		for(
			lines_t::iterator line = lines.begin();
			line != lines.end();
			line++
		) {
			cairo_text_extents(cr, line->utf8, &line->extents);

			if(line->extents.width > extents.width) extents.width = line->extents.width;

			// If we're the first line, get its height.
			if(!n) extents.height = -line->extents.y_bearing;

			// Otherwise, if we're the final line (which, coincidentally, might also be the
			// first line) get the "bottom-from-origin"
			if(n == lines.size() - 1) {
				if(!n) extents.bfo = line->extents.height + line->extents.y_bearing;

				else extents.bfo = size + line->extents.y_bearing;
			}

			// Get our largest "left-from-origin."
			if(line->extents.x_bearing < extents.lfo) extents.lfo = -line->extents.x_bearing;

			n++;
		}

		// Since extents.height was only used to record the topmost line's height,
		// we need to account for the entire body's height here.
		extents.height += (size * (n - 1)) + extents.bfo;
	}

	char* utf8;
	lines_t lines;
	unsigned int flags;
	extents_t extents;
	double tx;
	double ty;
};

#if 0
	// Perform X alignment; default is BASELINE.
	if(flags & CAIROCKS_TEXT_X_LEFT) *tx = -extents->x_bearing;

	else if(flags & CAIROCKS_TEXT_X_RIGHT) *tx = -(extents->width + extents->x_bearing);

	else if(flags & CAIROCKS_TEXT_X_CENTER) *tx = -(extents->width / 2.0) - extents->x_bearing;

	// Now we do Y alignment, who also defaults to BASELINE.
	if(flags & CAIROCKS_TEXT_Y_BOTTOM) *ty = -(extents->height + extents->y_bearing);

	else if(flags & CAIROCKS_TEXT_Y_CENTER) *ty =
		(extents->height / 2.0) - (extents->height + extents->y_bearing)
	;

	else if(flags & CAIROCKS_TEXT_Y_TOP) *ty = -extents->y_bearing;
#endif

// Initializes the Cairo state for the passed-in context and returns a private
// structure containing the data necessary to properly draw the string.
static cairocks_text_private_t* cairocks_text_private_init(
	cairo_t*     cr,
	const char*  utf8,
	const char*  font,
	double       size,
	unsigned int flags
) {
	cairo_select_font_face(
		cr,
		font,
		flags & CAIROCKS_TEXT_ITALIC ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
		flags & CAIROCKS_TEXT_BOLD ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL
	);

	cairo_set_font_size(cr, size);

	cairocks_text_private_t* text = 0;

	// Wrap our text alloction in an exception handling block, since they
	// could potentially be thrown here.
	try {
		text = new cairocks_text_private_t(utf8, flags);
	}

	catch(...) {
		text = 0;
	}

	if(!text) return 0;

	text->init(cr, size);

#if 0
	std::cout << "WIDTH: " << text->extents.width << std::endl;
	std::cout << "HEIGHT: " << text->extents.height << std::endl;
	std::cout << "LEFT-FROM-ORIGIN: " << text->extents.lfo << std::endl;
	std::cout << "BOTTOM-FROM-ORIGIN: " << text->extents.bfo << std::endl;
#endif

	return text;
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
	cairocks_text_private_t* text = cairocks_text_private_init(cr, utf8, font, size, flags);

	if(!text) return FALSE;

	if(!(flags & CAIROCKS_TEXT_NO_SAVE_RESTORE)) cairo_save(cr);

	cairo_translate(cr, x, y);

	for(
		cairocks_text_private_t::lines_t::iterator i = text->lines.begin();
		i != text->lines.end();
		i++
	) {
		std::cout << "Handling: " << i->utf8 << std::endl;

		function(cr, i->utf8);

		cairo_translate(cr, 0.0, size);
		cairo_move_to(cr, 0.0, 0.0);
	}

	if(!(flags & CAIROCKS_TEXT_NO_SAVE_RESTORE)) cairo_restore(cr);

	delete text;

	return TRUE;
}

extern "C" {

cairo_bool_t cairocks_show_text(
	cairo_t*     cr,
	const char*  utf8,
	const char*  font,
	double       size,
	double       x,
	double       y,
	unsigned int flags
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
	cairocks_text_private_t* text = cairocks_text_private_init(cr, utf8, font, size, flags);

	if(!text) return FALSE;

	text->extents.to_cairo(extents);

	std::cout
		<< "x_bearing: " << extents->x_bearing << std::endl
		<< "y_bearing: " << extents->y_bearing << std::endl
		<< "width: " << extents->width << std::endl
		<< "height: " << extents->height << std::endl
	;

	if(rect_extents) {
		double rx = 0.0;
		double ry = 0.0;

		/* Calculate the rect X origin. */
		if(flags & CAIROCKS_TEXT_X_LEFT) rx = 0.0;

		else if(flags & CAIROCKS_TEXT_X_CENTER) rx = extents->width / 2.0;

		else if(flags & CAIROCKS_TEXT_X_RIGHT) rx = extents->width;

		else rx = -extents->x_bearing;

		/* Calculate the rect Y origin. */
		if(flags & CAIROCKS_TEXT_Y_BOTTOM) ry = extents->height;

		else if(flags & CAIROCKS_TEXT_Y_CENTER) ry = extents->height / 2.0;

		else if(flags & CAIROCKS_TEXT_Y_TOP) ry = 0.0;

		else ry = -extents->y_bearing;

		/* Finally, the width and height of the rect always match the extent
		 * width and height. */
		rect_extents[0] = x - rx;
		rect_extents[1] = y - ry;
		rect_extents[2] = extents->width;
		rect_extents[3] = extents->height;
	}

	delete text;

	return TRUE;
}

}

