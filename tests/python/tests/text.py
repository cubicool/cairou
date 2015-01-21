from . import common

import math
import cairocffi as cairo
import cairockscffi as cairocks


cairocks.merge_with_cairocffi()


font = "Glyphicons"
size = 200
size2 = size / 2.0
flags = (
    cairo.TEXT_Y_BOTTOM | cairo.TEXT_X_LEFT,
    cairo.TEXT_Y_BOTTOM | cairo.TEXT_X_CENTER,
    cairo.TEXT_Y_BOTTOM | cairo.TEXT_X_RIGHT,
    cairo.TEXT_Y_TOP | cairo.TEXT_X_LEFT,
    cairo.TEXT_Y_TOP | cairo.TEXT_X_CENTER,
    cairo.TEXT_Y_TOP | cairo.TEXT_X_RIGHT,
    cairo.TEXT_Y_CENTER | cairo.TEXT_X_CENTER,
    cairo.TEXT_Y_BASELINE | cairo.TEXT_X_BASELINE
)


def draw_character(cr, c, x, y, flag):
    cr.translate(x, y)
    cr.rectangle(-size2, -size2, size, size)
    cr.stroke()
    cr.move_to(-size2, 0.0)
    cr.line_to(size2, 0.0)
    cr.stroke()
    cr.move_to(0.0, -size2)
    cr.line_to(0.0, size2)
    cr.stroke()

    # These are the common arguments for each text function.
    # We have them here, rather than respecifying them over
    # and over.
    text_args = (
        c,
        font,
        size * 0.4,
        0.0,
        0.0,
        flag
    )

    ex, r_ex = cr.text_extents(*text_args, rect_extents=True)

    with cr.saved():
        cr.show_text(*text_args)

    cr.rectangle(*r_ex)
    cr.set_source_rgba(1.0, 1.0, 0.0, 1.0)
    cr.stroke()


@common.test_function(
    "text0",
    1024,
    512,
    description="shows basic XY character alignment"
)
def test_text0(cr, w, h):
    i = 0

    for y in range(2):
        for x in range(4):
            cr.save()
            cr.translate(x * 256, y * 256)

            draw_character(cr, "\ue218", 128, 128, flags[i])

            cr.restore()

            i += 1


@common.test_function(
    "text1",
    512,
    512,
    description="tests the rect_extents calculations"
)
def test_text1(cr, w, h):
    cr.set_line_width(1.0)
    cr.move_to(0.0, 100.5)
    cr.line_to(w, 100.5)
    cr.stroke()
    cr.move_to(50.5, 0.0)
    cr.line_to(50.5, h)
    cr.stroke()

    x, y = 50, 100
    size = 50

    # We set these up here so we don't have to duplicate them to both
    # the drawing AND extents calls.
    args = "Jeremy", "Lato", size, x, y, (
        cairo.TEXT_X_BASELINE |
        cairo.TEXT_Y_BASELINE |
        cairo.TEXT_BOLD
    )

    ext, (tx, ty, tw, th) = cr.text_extents(*args, rect_extents=True)
    fe = cr.font_extents()

    cr.show_text(*args)
    cr.arc(x, y, 4.0, 0.0, 2.0 * math.pi)
    cr.fill()

    cr.show_text("Moles", "Lato", size, x, y + fe[2], (
        cairo.TEXT_X_BASELINE |
        cairo.TEXT_Y_BASELINE |
        cairo.TEXT_BOLD
    ))


@common.test_function(
    "text2",
    512,
    512,
    description="tests breaking text up by newline"
)
def test_text2(cr, w, h):
    cr.show_text("Foo\nBar\nBaz", "Lato", 40.0, 256.0, 100.0, (
        cairo.TEXT_X_CENTER |
        cairo.TEXT_BOLD
    ))
