import cairo
import math

lw     = 3
radius = 45
width  = 17
segs   = 40

cr.set_source_rgba(0.2, 0.4, 0.8, 0.5)
cr.set_line_width(lw * 4.0)
cr.arc(w / 2, h / 2, radius - (lw * 3.0), math.pi / 2.0, math.pi + (math.pi / 2.0))
cr.stroke()

# Draw the 2 outer rings.
cr.set_line_width(lw)
cr.set_source_rgb(0.3, 0.3, 0.3)
cr.arc(w / 2, h / 2, radius - width, 0, 2 * math.pi)
cr.stroke()
cr.arc(w / 2, h / 2, radius, 0, 2 * math.pi)
cr.stroke()
cr.save()

# Get ready to draw our inner "spokes."
cr.set_line_cap(cairo.LINE_CAP_ROUND)

for i in range(segs):
	cr.save()
	cr.translate(w / 2, h / 2)
	
	cr.rotate((i / float(segs)) * (math.pi * 2))
	
	cr.move_to(0, -(radius - lw - 1))
	cr.line_to(0, -((radius - width) + lw + 1))
	cr.stroke()
	cr.restore()



