from . import common

import cairoucffi as cairou
import os


@common.test_function("jpeg_load")
def test_jpeg_load(cr):
    cairou.surface_from_jpeg("../data/test.jpg")

    print("Successfully read test.jpg into a Surface.")


@common.test_function("gif_load")
def test_gif_load(cr):
    surface = cairou.GIFSurface("../data/test.gif")

    print("Successfully read test.gif into a Surface.")

    frames = surface.num_frames

    print("GIF has %d frames." % frames)

    if not os.path.isdir("gif_frames"):
        os.mkdir("gif_frames")

    for i in range(frames):
        file_name = "gif_frames/%03d.png" % i

        surface.write_to_png(file_name)

        print("Wrote:", file_name, surface.next())
