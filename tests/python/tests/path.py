from . import common

import cairocffi as cairo
import cairockscffi as cairocks
import math


@common.test_function("named_path")
def test_named_path(cr):
    for i in range(5):
        cr.arc(100.0 * i, 100.0 * i, 75.0, 0.0, 2.0 * math.pi)

        cairocks.append_named_path(cr, "circles")

    cairocks.set_named_path(cr, "circles")

    cr.set_line_width(10.0)
    cr.stroke()


@common.test_function("map_path_onto")
def test_map_path_onto(cr):
    cr.arc(400.0, 200.0, 150.0, math.pi, 2.0 * math.pi)

    p = cr.copy_path_flat()

    cr.new_path()
    cr.set_line_width(1.0)
    cr.set_font_size(30.0)
    cr.select_font_face(
        "Sans",
        cairo.FONT_SLANT_NORMAL,
        cairo.FONT_WEIGHT_NORMAL
    )

    cr.text_path("cairocks_map_path_onto")

    cairocks.map_path_onto(cr, p)

    cr.set_source_rgb(0.5, 0.5, 0.5)
    cr.stroke()
