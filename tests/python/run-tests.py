import os
import tests.common
import cairocffi as cairo


OUTPUT_DIR = os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "test_output"
)


if __name__ == "__main__":
    for test in tests.common.TESTS:
        output = "Running test: %s" % test.name

        if test.description:
            output += " (%s)" % test.description

        print(output)

        surface = cairo.ImageSurface(
            test.surface_format,
            test.width,
            test.height
        )

        cr = cairo.Context(surface)

        cr.set_source_rgba(0.0, 0.0, 0.0, 1.0)
        cr.paint()
        cr.set_source_rgba(1.0, 1.0, 1.0, 1.0)

        try:
            test(cr, test.width, test.height)

        except Exception as e:
            print("Error in test %s: %s" % (test.name, e))

        surface.write_to_png(os.path.join(
            OUTPUT_DIR,
            test.name + ".png"
        ))
