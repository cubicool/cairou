# This file tests the "merge" feature; that is,
# merging the cairou API with the Context and Surface
# classes in cairocffi.

import cairocffi as cairo
import cairoucffi as cairou

cairou.merge_with_cairocffi()

if __name__ == "__main__":
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 256, 256)
    cr = cairo.Context(surface)

    with cr.saved():
        cr.rounded_rectangle(10, 10, 236, 236, 40)
        cr.set_source_rgba(1.0, 1.0, 0.1, 1.0)
        cr.fill()

    cr.set_source_rgba(0.0, 0.0, 0.9, 1.0)
    cr.show_text("FOO", "Sans", 50.0, 128.0, 128.0)

    surface.write_to_png("foo.png")
