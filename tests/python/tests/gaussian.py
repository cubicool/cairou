from . import common

import cairocffi as cairo
import cairoucffi as cairou
import math


@common.test_function("gaussian")
def test_gaussian_blur(cr):
    w, h = 200, 200
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, w, h)
    c = cairo.Context(surface)

    c.set_line_width(2.0)

    for i in range(5):
        c.arc(w / 2, h / 2, (i + 1) * 15.0, 0.0, 2.0 * math.pi)
        c.stroke()

    cairou.gaussian_blur(surface, 8.0, 0.0)

    cr.set_source_surface(surface, 200, 200)
    cr.paint()
