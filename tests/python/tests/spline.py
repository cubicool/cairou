from . import common

import math
import cairockscffi as cairocks

cairocks.merge_with_cairocffi()


@common.test_function("spline", 512, 512)
def test_spline(cr, w, h):
    spline = (
        (-10, -10),
        (-5, -10),
        (-5, 0),
        (5, 0),
        (5, -10),
        (10, -10),
        (10, 10),
        (-10, 10)
    )

    cr.translate(w // 2, h // 2)
    cr.scale(10.0, 10.0)

    cr.set_source_rgba(1.0, 1.0, 1.0, 1.0)
    cr.append_spline(spline, True)
    cr.set_line_width(0.5)
    cr.stroke()

    cr.set_source_rgba(1.0, 0.0, 0.0, 1.0)
    
    for s in spline:
        cr.arc(s[0], s[1], 0.75, 0.0, 2.0 * math.pi)
        cr.fill()

