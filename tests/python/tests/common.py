import os
import sys

import cairocffi as cairo

THIS_DIR = os.path.dirname(os.path.abspath(__file__))

"""
LDLP = "LD_LIBRARY_PATH"
LDLP_ENV = os.environ[LDLP]

os.environ[LDLP] = ";".join((
    ".",
    os.path.join(THIS_DIR, "..", "..", ".."),
    os.path.join(THIS_DIR, "..", "..", "..", "build"),
    LDLP_ENV and LDLP_ENV or ""
))
"""

sys.path.extend((
    ".",
    os.path.join(THIS_DIR, "..", "..", "..", "src", "python"),
))

TESTS = []


def test_function(
    name,
    width,
    height,
    surface_format=cairo.FORMAT_ARGB32,
    description=""
):
    def test_function_decorated0(func):
        global TESTS

        def test_function_decorated1(cr, w, h):
            cr.save()

            func(cr, w, h)

            cr.restore()

        f = test_function_decorated1

        # TODO: Can this be replaced with an introspection library?
        # for var in test_function.__code__.co_varnames:
        #    setattr(f, "var", eval(var))

        f.name = name
        f.width = width
        f.height = height
        f.surface_format = surface_format
        f.description = description

        TESTS.append(f)

        return f

    return test_function_decorated0
