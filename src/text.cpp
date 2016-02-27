#include "cairou.h"
#include "utf8.h"

#include <math.h>
#include <cstring>
#include <vector>

// Since we use the variable name "utf8" a great deal, we need to re-assign
// the namespace defined by this header above to something more compatible with
// our code.
namespace UTF8 = utf8;

// This structure holds all the data we need to easily account for newlines in
// blocks of text and to use those substrings (as delimited by those newlines)
// with the Cairo API. We take a fairly easy approach wherein we essentially
// break the string up into small strings by storing a start position for each
// substring and replacing the newlines we find in our copy with NULL terminators.
class cairou_text_private_t {
public:
	struct line_t {
		line_t(const char* _utf8):
		utf8(_utf8),
		tx(0.0),
		ty(0.0) {
		}

		const char* utf8;
		cairo_text_extents_t extents;
		double tx;
		double ty;
	};

	typedef std::vector<line_t> lines_t;

	cairou_text_private_t(const char* _utf8, int _flags):
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

				last = p + 1;
			}
		}

		// If the string doesn't end in a newline, go ahead and push the remainder
		// as a new line of text
		if(last < size) lines.push_back(utf8 + last);

		memset(&extents, 0, sizeof(cairo_text_extents_t));
	}

	~cairou_text_private_t() {
		if(utf8) delete[] utf8;
	}

	// TODO: Can I calculate this without knowing the "size" of the font?
	// For example, can I use Y advance?
	void init(cairo_t* cr, double size) {
		unsigned int n = 0;

		for(lines_t::iterator line = lines.begin(); line != lines.end(); line++) {
			cairo_text_extents(cr, line->utf8, &line->extents);

			if(line->extents.width > extents.width) extents.width = line->extents.width;

			if(lines.size() == 1) {
				extents.height = line->extents.height;
				extents.y_bearing = line->extents.y_bearing;
			}

			else {
				// If we're the first line...
				if(!n) {
					extents.height += -line->extents.y_bearing;
					extents.y_bearing += line->extents.y_bearing;
				}

				// If we're the last line...
				else if(n == lines.size() - 1) {
					extents.height += size + (line->extents.height + line->extents.y_bearing);
					extents.y_bearing -= size;
				}

				// Otherwise, we're one of many lines...
				else {
					extents.height += size;
					extents.y_bearing -= size;
				}
			}

			// Get our largest x_bearing.
			if(line->extents.x_bearing < extents.x_bearing) {
				extents.x_bearing = line->extents.x_bearing;
			}

			n++;
		}

		// Perform X alignment; default is BASELINE.
		if(flags & CAIROU_TEXT_X_LEFT) tx = -extents.x_bearing;

		else if(flags & CAIROU_TEXT_X_RIGHT) tx = -(extents.width + extents.x_bearing);

		else if(flags & CAIROU_TEXT_X_CENTER) tx = -(extents.width / 2.0) - extents.x_bearing;

		// Now we do Y alignment, who also defaults to BASELINE.
		if(flags & CAIROU_TEXT_Y_BOTTOM) ty = -(extents.height + extents.y_bearing);

		else if(flags & CAIROU_TEXT_Y_CENTER) ty =
			(extents.height / 2.0) - (extents.height + extents.y_bearing)
		;

		else if(flags & CAIROU_TEXT_Y_TOP) ty = -extents.y_bearing;

		// Adjust our origin to account for multiple lines.
		ty -= (lines.size() - 1) * size;

		// If the user has requested pixel-alignment, do so now. Unfortunately,
		// this doesn't guarantee that every character also occurs on pixel
		// boundaries; that will require using harfbuzz, which will eventually be
		// added to Cairou.
		if(flags & CAIROU_TEXT_PIXEL_ALIGN) {
			tx = round(tx);
			ty = round(ty);
		}

		// Now we'll adjust each line individually, based on what kind of paragraph
		// alignment we're using. We need to do this last; or, more specifically, after
		// having iterated over each line to determine to the master paragraph width.
		for(lines_t::iterator line = lines.begin(); line != lines.end(); line++) {
			double ew = extents.width;
			double lw = line->extents.width;

			if(flags & CAIROU_TEXT_ALIGN_RIGHT) line->tx = ew - lw;

			else if(flags & CAIROU_TEXT_ALIGN_CENTER) line->tx = (ew - lw) / 2.0;

			// The final (untested) option is LEFT, which requires no modification.
			// All we need to do now is handle pixel-alignment.
			if(flags & CAIROU_TEXT_PIXEL_ALIGN) {
				line->tx = round(line->tx);
				line->ty = round(line->ty);
			}
		}
	}

	char* utf8;
	lines_t lines;
	unsigned int flags;
	cairo_text_extents_t extents;
	double tx;
	double ty;
};

// Initializes the Cairo state for the passed-in context and returns a private
// structure containing the data necessary to properly draw the string.
static cairou_text_private_t* cairou_text_private_init(
	cairo_t* cr,
	const char* utf8,
	const char* font,
	double size,
	unsigned int flags
) {
	cairo_select_font_face(
		cr,
		font,
		flags & CAIROU_TEXT_ITALIC ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
		flags & CAIROU_TEXT_BOLD ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL
	);

	cairo_set_font_size(cr, size);

	cairou_text_private_t* text = 0;

	// Wrap our text alloction in an exception handling block, since they
	// could potentially be thrown here.
	try {
		text = new cairou_text_private_t(utf8, flags);
	}

	catch(...) {
		text = 0;
	}

	if(!text) return 0;

	text->init(cr, size);

	return text;
}

static cairo_bool_t cairou_text_private_draw(
	cairo_t* cr,
	const char* utf8,
	const char* font,
	double size,
	double x,
	double y,
	int flags,
	void (*function)(cairo_t*, const char*)
) {
	cairou_text_private_t* text = cairou_text_private_init(cr, utf8, font, size, flags);

	if(!text) return FALSE;

	if(!(flags & CAIROU_TEXT_NO_SAVE_RESTORE)) cairo_save(cr);

	cairo_translate(cr, x + text->tx, y + text->ty);

	for(
		cairou_text_private_t::lines_t::iterator line = text->lines.begin();
		line != text->lines.end();
		line++
	) {
		cairo_save(cr);
		cairo_translate(cr, line->tx, line->ty);
		cairo_move_to(cr, 0.0, 0.0);

		function(cr, line->utf8);

		cairo_restore(cr);
		cairo_translate(cr, 0.0, size);
		cairo_move_to(cr, 0.0, 0.0);
	}

	if(!(flags & CAIROU_TEXT_NO_SAVE_RESTORE)) cairo_restore(cr);

	delete text;

	return TRUE;
}

extern "C" {

cairo_bool_t cairou_show_text(
	cairo_t* cr,
	const char* utf8,
	const char* font,
	double size,
	double x,
	double y,
	unsigned int flags
) {
	return cairou_text_private_draw(cr, utf8, font, size, x, y, flags, cairo_show_text);
}

cairo_bool_t cairou_text_path(
	cairo_t* cr,
	const char* utf8,
	const char* font,
	double size,
	double x,
	double y,
	int flags
) {
	return cairou_text_private_draw(cr, utf8, font, size, x, y, flags, cairo_text_path);
}

cairo_bool_t cairou_text_extents(
	cairo_t* cr,
	const char* utf8,
	const char* font,
	double size,
	double x,
	double y,
	int flags,
	cairo_text_extents_t* extents,
	double* rect_extents
) {
	cairou_text_private_t* text = cairou_text_private_init(cr, utf8, font, size, flags);

	if(!text) return FALSE;

	std::memcpy(extents, &text->extents, sizeof(cairo_text_extents_t));

	if(rect_extents) {
		double rx = 0.0;
		double ry = 0.0;

		/* Calculate the rect X origin. */
		if(flags & CAIROU_TEXT_X_LEFT) rx = 0.0;

		else if(flags & CAIROU_TEXT_X_CENTER) rx = extents->width / 2.0;

		else if(flags & CAIROU_TEXT_X_RIGHT) rx = extents->width;

		else rx = -extents->x_bearing;

		/* Calculate the rect Y origin. */
		if(flags & CAIROU_TEXT_Y_BOTTOM) ry = extents->height;

		else if(flags & CAIROU_TEXT_Y_CENTER) ry = extents->height / 2.0;

		else if(flags & CAIROU_TEXT_Y_TOP) ry = 0.0;

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

