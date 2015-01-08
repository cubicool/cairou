from . import common

import math

import cairockscffi as cairocks

cairocks.merge_with_cairocffi()


font = "Glyphicons"
size = 200
size2 = size / 2.0
flags = (
    cairocks.Y_BOTTOM | cairocks.X_LEFT,
    cairocks.Y_BOTTOM | cairocks.X_CENTER,
    cairocks.Y_BOTTOM | cairocks.X_RIGHT,
    cairocks.Y_TOP | cairocks.X_LEFT,
    cairocks.Y_TOP | cairocks.X_CENTER,
    cairocks.Y_TOP | cairocks.X_RIGHT,
    cairocks.Y_CENTER | cairocks.X_CENTER,
    cairocks.Y_BASELINE | cairocks.X_BASELINE
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
        cairocks.X_BASELINE |
        cairocks.Y_BASELINE |
        cairocks.BOLD
    )

    ext, (tx, ty, tw, th) = cr.text_extents(*args, rect_extents=True)
    fe = cr.font_extents()

    cr.show_text(*args)
    cr.arc(x, y, 4.0, 0.0, 2.0 * math.pi)
    cr.fill()

    cr.show_text("Moles", "Lato", size, x, y + fe[2], (
        cairocks.X_BASELINE |
        cairocks.Y_BASELINE |
        cairocks.BOLD
    ))


@common.test_function(
    "text2",
    512,
    512,
    description="tests breaking text up by newline"
)
def test_text2(cr, w, h):
    cr.show_text("Foo\nBar\nBaz", "Lato", 40.0, 256.0, 100.0, (
        cairocks.X_CENTER |
        cairocks.BOLD
    ))
