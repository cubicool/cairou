from . import common

import cairocffi as cairo
import cairoucffi as cairou
import math


@common.test_function("emboss")
def test_emboss(cr):
    text = "Cairou"
    text_size = 30
    w, h = 200, 200
    s = cairo.ImageSurface(cairo.FORMAT_A8, w, h)
    c = cairo.Context(s)

    c.select_font_face("Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD)
    c.set_font_size(text_size)

    extents = c.text_extents(text)

    c.move_to(((w - extents[2]) / 2.0) - extents[0], (h - extents[1]) / 2.0)
    c.show_text(text)

    emboss = cairou.emboss_create(
        s,
        130.0 * (math.pi / 180.0),
        45.0 * (math.pi / 180.0),
        100.0,
        0.1,
        0.1
    )

    cr.set_source_surface(emboss, 0, 200)
    cr.paint()
