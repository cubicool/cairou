import cairo

s    = 40
text = "Cairo"

cr.select_font_face("Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD)
cr.set_font_size(155)

extents = cr.text_extents(text)

cr.move_to(((w - extents[2]) / 2.0) - extents[0], (h - extents[1]) / 2.0)
cr.text_path(text)

# A simpler draw test; off for now.
# cairocks.rounded_rectangle(cr, s, s, w - (s * 2.0), h - (s * 2.0), s)

cr.set_operator(cairo.OPERATOR_SOURCE)

for i in range(s, 1, -1):
        c = 1.0 - i / float(s)

	cr.set_source_rgba(1.0, 1.0, 1.0, c)
	cr.set_line_width(i)
	cr.stroke_preserve()

cr.fill()

