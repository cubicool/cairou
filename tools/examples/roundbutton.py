import math
import cairocffi as cairo

lw = 6.0
r  = 60.0
bc = 0.4, 0.4, 0.4

cr.set_source_rgb(*[c * 1.70 for c in bc])
cr.arc((w / 2.0) + (lw / 2.0), (h / 2.0) + (lw / 2.0), r + (lw / 2.0), 0.0, 2.0 * math.pi)
cr.fill()

cr.set_line_width(lw)
cr.arc(w / 2.0, h / 2.0, r, 0.0, 2.0 * math.pi)
cr.set_source_rgb(1.0, 1.0, 1.0)
cr.fill_preserve()

# ---
p = cairo.LinearGradient((w / 2.0) + r, (h / 2.0) + r, (w / 2.0) - r, (h / 2.0) - r)

p.add_color_stop_rgba(0.0, 0.8, 0.4, 0.0, 1.0)
p.add_color_stop_rgba(0.8, 0.8, 0.4, 0.0, 0.0)

cr.set_source(p)
cr.fill_preserve()
# ---

cr.set_source_rgb(*bc)
cr.stroke()

cr.set_line_width(lw  / 2.0)
cr.arc(w / 2.0, h / 2.0, r - (lw / 2.0), 0.0, 2.0 * math.pi)
cr.set_source_rgb(1.0, 1.0, 1.0)
cr.stroke()

cr.set_line_width(lw / 4.0)
cr.arc(w / 2.0, h / 2.0, r + (lw / 2.0), 0.0, 2.0 * math.pi)
cr.set_source_rgb(1.0, 1.0, 1.0)
# cr.set_operator(cairo.OPERATOR_CLEAR)
cr.stroke()

