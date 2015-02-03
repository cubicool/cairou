from . import common

import cairocffi as cairo
import cairockscffi as cairocks


cairocks.merge_with_cairocffi()


FLAGS = (
    cairo.TEXT_Y_BOTTOM | cairo.TEXT_X_LEFT,
    cairo.TEXT_Y_BOTTOM | cairo.TEXT_X_CENTER,
    cairo.TEXT_Y_BOTTOM | cairo.TEXT_X_RIGHT,
    cairo.TEXT_Y_TOP | cairo.TEXT_X_LEFT,
    cairo.TEXT_Y_TOP | cairo.TEXT_X_CENTER,
    cairo.TEXT_Y_TOP | cairo.TEXT_X_RIGHT,
    cairo.TEXT_Y_CENTER | cairo.TEXT_X_CENTER,
    cairo.TEXT_Y_BASELINE | cairo.TEXT_X_BASELINE
)

LOREM_IPSUM = """\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras efficitur dui
risus, ac elementum orci rhoncus ac. Praesent risus lacus, pretium in enim at,
fringilla viverra eros. Ut non tincidunt quam. Donec aliquam fermentum lectus,
eget porttitor metus fringilla quis. Duis nulla augue, commodo mattis eros at,
fermentum pellentesque dolor. Integer convallis nisi nec metus consequat, at
eleifend enim mollis. Sed faucibus, ligula eget cursus maximus, risus diam
finibus arcu, sed tincidunt urna nisi et nisi. Suspendisse cursus nisl
fermentum ex porttitor fermentum. Cras sed feugiat mi. Vivamus congue
condimentum leo vitae auctor. Sed fringilla lobortis leo eleifend aliquet. In
hac habitasse platea dictumst. Maecenas ac arcu et metus luctus malesuada."""


SIZE = 200
SIZE2 = SIZE / 2.0


def draw_character(cr, c, x, y, flag):
    cr.translate(x, y)
    cr.rectangle(-SIZE2, -SIZE2, SIZE, SIZE)
    cr.stroke()
    cr.move_to(-SIZE2, 0.0)
    cr.line_to(SIZE2, 0.0)
    cr.stroke()
    cr.move_to(0.0, -SIZE2)
    cr.line_to(0.0, SIZE2)
    cr.stroke()

    # These are the common arguments for each text function.
    # We have them here, rather than respecifying them over
    # and over.
    text_args = (
        c,
        "Glyphicons",
        SIZE * 0.4,
        0.0,
        0.0,
        flag
    )

    ex, r_ex = cr.text_extents(*text_args, rect_extents=True)

    with cr.saved():
        # cr.show_text(*text_args)
        cr.text_path(*text_args)
        cr.fill()

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

            draw_character(cr, "\ue218", 128, 128, FLAGS[i])

            cr.restore()

            i += 1


@common.test_function(
    "text1",
    1025,
    512,
    description="tests breaking one large text buffer"
)
def test_text2(cr, w, h):
    cr.set_source_rgb(1.0, 0.5, 0.0)
    cr.paint()
    cr.set_source_rgb(1.0, 1.0, 1.0)
    cr.rectangle(1.0, 1.0, w - 2.0, h - 2.0)
    cr.move_to(0.0, h // 2)
    cr.line_to(w, h // 2)
    cr.stroke()
    cr.move_to(w // 2, 0.0)
    cr.line_to(w // 2, h)
    cr.stroke()
    cr.show_text(LOREM_IPSUM, "Lato", 20.0, w // 2, h // 2, (
        cairo.TEXT_X_CENTER |
        cairo.TEXT_Y_CENTER |
        cairo.TEXT_BOLD
    ))
