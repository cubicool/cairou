import os
import tests.common
import cairocffi as cairo


WIDTH = 1024
HEIGHT = 512
OUTPUT_DIR = os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "test_output"
)


if __name__ == "__main__":
    for test in tests.common.TESTS:
        print("Running test: %s" % test.name)

        surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, WIDTH, HEIGHT)
        cr = cairo.Context(surface)

        cr.set_source_rgba(0.0, 0.0, 0.0, 1.0)
        cr.paint()
        cr.set_source_rgba(1.0, 1.0, 1.0, 1.0)

        try:
            test(cr, WIDTH, HEIGHT)

        except Exception as e:
            print("Error in test %s: %s" % (test.name, e))

        surface.write_to_png(os.path.join(
            OUTPUT_DIR,
            test.name + ".png"
        ))
