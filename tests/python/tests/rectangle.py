from . import common

import cairocffi as cairo
import cairockscffi as cairocks
import math


@common.test_function("rounded_rectangle")
def test_rounded_rectangle(cr):
    cr.set_line_width(4.0)
    cr.set_source_rgb(0.5, 0.5, 0.5)

    cairocks.rounded_rectangle(
        cr,
        10.0,
        10.0,
        180.0,
        180.0,
        20.0,
        (True, False, True, False)
    )

    cr.stroke()


@common.test_function("rounded_rectangle_apply")
def test_rounded_rectangle_apply(cr):
    w, h = 200, 200
    s = cairo.ImageSurface(cairo.FORMAT_ARGB32, w, h)
    c = cairo.Context(s)

    # TODO: This DOES NOT work with the code below...
    if False:
        c.save()

        p = cairo.RadialGradient(
            w / 2.0,
            h / 2.0,
            0.0,
            w / 2.0,
            h / 2.0,
            200.0
        )

        p.add_color_stop_rgba(0.0, 0.8, 0.4, 0.0, 0.0)
        p.add_color_stop_rgba(1.0, 0.8, 0.4, 0.0, 1.0)

        c.arc(w / 2.0, h / 2.0, 200.0, 0.0, 2.0 * math.pi)
        c.set_source(p)
        c.paint()
        c.restore()

    # TODO: This DOES work with the code below...
    if True:
        c.set_source_rgb(0.0, 1.0, 0.5)
        c.paint()

    # cairocks.rounded_rectangle_apply(c, 20.0, 20.0, 160.0, 160.0, 30.0)

    # TODO: The code below does the exact same thing as the call
    # to rounded_rectangle_apply.
    p2 = cairo.SolidPattern(0.0, 0.0, 0.0)

    c.save()
    c.set_operator(cairo.OPERATOR_DEST_IN)
    c.set_source(p2)

    cairocks.rounded_rectangle(c, 20.0, 20.0, 160.0, 160.0, 30.0)

    c.fill()
    c.restore()

    cr.set_source_surface(s, 400, 200)
    cr.paint()
