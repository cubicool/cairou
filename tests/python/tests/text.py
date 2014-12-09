from . import common

import cairockscffi as cairocks


font = "Futura"
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
    # cr.set_line_width(1.0)
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
        cr,
        c,
        font,
        size * 0.5,
        0.0,
        0.0,
        flag
    )

    ex, r_ex = cairocks.text_extents(*text_args, rect_extents=True)

    cr.save()

    cairocks.show_text(*text_args)

    cr.restore()

    cr.rectangle(*r_ex)
    cr.set_source_rgba(1.0, 1.0, 0.0, 1.0)
    cr.stroke()


@common.test_function("text")
def test_text(cr, w, h):
    i = 0

    for y in range(2):
        for x in range(4):
            cr.save()
            cr.translate(x * 256, y * 256)

            draw_character(cr, "q", 128, 128, flags[i])

            cr.restore()

            i += 1
