import os
import sys


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


def test_function(func_name=None):
    def test_function_decorated0(func):
        global TESTS

        def test_function_decorated1(cr, w, h):
            cr.save()

            func(cr, w, h)

            cr.restore()

        f = test_function_decorated1

        f.name = func_name

        TESTS.append(f)

        return f

    return test_function_decorated0
