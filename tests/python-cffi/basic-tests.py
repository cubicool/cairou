# There is no shebang here because you need to manually invoke
# this file with the python interpreter of your choice. :)

import os

os.environ["LD_LIBRARY_PATH"] = "..;../..;../../build"

import sys

sys.path.extend((".", "..", "../build", "../src"))

import os
import cairocffi as cairo
import cairocks
import math

surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 600, 400)
cr      = cairo.Context(surface)

cr.set_source_rgb(1.0, 1.0, 1.0)
cr.paint()
cr.set_source_rgb(1.0, 0.5, 0.0)

TESTS = []

def test_function(func):
	global TESTS

	def test_function_decorator(*args, **kargs):
		cr.save()

		func(*args, **kargs)

		cr.restore()

	f = test_function_decorator

	TESTS.append(f)

	return f

@test_function
def test_rounded_rectangle():
	cr.set_line_width(4.0)
	cr.set_source_rgb(0.5, 0.5, 0.5)

	cairocks.rounded_rectangle(cr, 10.0, 10.0, 180.0, 180.0, 20.0, (True, False, True, False))

	cr.stroke()

# TODO: BROKEN!?
@test_function
def test_rounded_rectangle_apply():
	w, h = 200, 200
	s    = cairo.ImageSurface(cairo.FORMAT_ARGB32, w, h)
	c    = cairo.Context(s)

	# TODO: This DOES NOT work with the code below...
	if False:
		c.save()

		p = cairo.RadialGradient(w / 2.0, h / 2.0, 0.0, w / 2.0, h / 2.0, 200.0)

		p.add_color_stop_rgba(0.0, 0.8, 0.4, 0.0, 0.0)
		p.add_color_stop_rgba(1.0, 0.8, 0.4, 0.0, 1.0)

		c.arc(w / 2.0, h / 2.0, 200.0, 0.0, 2.0 * math.pi)
		c.set_source(p)
		c.paint()
		c.restore()

	# TODO: This DOES work with the code below...
	if True:
		c.set_source_rgb(0.0, 1.0, 0.5)
		c.paint()

	# cairocks.rounded_rectangle_apply(c, 20.0, 20.0, 160.0, 160.0, 30.0)

	# TODO: The code below does the exact same thing as the call to rounded_rectangle_apply.
	p2 = cairo.SolidPattern(0.0, 0.0, 0.0);

	c.save()
	c.set_operator(cairo.OPERATOR_DEST_IN);
	c.set_source(p2);

	cairocks.rounded_rectangle(c, 20.0, 20.0, 160.0, 160.0, 30.0)

	c.fill()
	c.restore()

	cr.set_source_surface(s, 400, 200)
	cr.paint()

# @test_function
def test_named_path():
	for i in range(5):
		cr.arc(100.0 * i, 100.0 * i, 75.0, 0.0, 2.0 * math.pi)

		cairocks.append_named_path(cr, "circles")

	cairocks.set_named_path(cr, "circles")

	cr.set_line_width(10.0)
	cr.stroke()

@test_function
def test_map_path_onto():
	cr.arc(400.0, 200.0, 150.0, math.pi, 2.0 * math.pi)

	p = cr.copy_path_flat()

	cr.new_path()
	cr.set_line_width(1.0)
	cr.select_font_face("Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_NORMAL)
	cr.set_font_size(30.0)

	cr.text_path("cairocks_map_path_onto")

	cairocks.map_path_onto(cr, p)

	cr.set_source_rgb(0.5, 0.5, 0.5)
	cr.stroke()

"""
@test_function
def test_map_path_onto():
	cr.arc(200.0, 200.0, 150.0, 0.0, 2.0 * math.pi)

	p = cr.copy_path_flat()

	cr.new_path()
	cr.set_line_width(1.0)

	cr.move_to(0.0, 0.0)

	for i in range(15):
		x = i * 50.0

		cr.line_to(x, 0.0)
		cr.line_to(x, 5.0)
		cr.line_to(x + 5.0, 0.0)
		cr.line_to(x, -5.0)
		cr.line_to(x, 0.0)

	cairocks.map_path_onto(cr, p)

	cr.set_source_rgb(0.5, 0.5, 0.5)
	cr.stroke()
"""

@test_function
def test_emboss():
	text      = "Cairocks"
	text_size = 30
	w, h      = 200, 200
	s         = cairo.ImageSurface(cairo.FORMAT_A8, w, h)
	c         = cairo.Context(s)

	c.select_font_face("Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD)
	c.set_font_size(text_size)

	extents = c.text_extents(text)

	c.move_to(((w - extents[2]) / 2.0) - extents[0], (h - extents[1]) / 2.0)
	c.show_text(text)

	emboss = cairocks.emboss_create(
		s,
		130.0 * (math.pi / 180.0),
		45.0 * (math.pi / 180.0),
		100.0,
		0.1,
		0.1
	)

	cr.set_source_surface(emboss, 0, 200)
	cr.paint()

@test_function
def test_gaussian_blur():
	w, h    = 200, 200
	surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, w, h);
	c       = cairo.Context(surface)

	c.set_line_width(2.0)

	for i in range(5):
		c.arc(w / 2, h / 2, (i + 1) * 15.0, 0.0, 2.0 * math.pi)
		c.stroke()

	cairocks.gaussian_blur(surface, 8.0, 0.0)

	cr.set_source_surface(surface, 200, 200)
	cr.paint()

@test_function
def test_jpeg_load():
	cairocks.surface_from_jpeg("../data/test.jpg")

	print("Successfully read test.jpg into a Surface.")

@test_function
def test_gif_load():
	surface = cairocks.GIFSurface("../data/test.gif")

	print("Successfully read test.gif into a Surface.")

	frames = surface.num_frames

	print("GIF has %d frames." % frames)

	if not os.path.isdir("gif_frames"):
		os.mkdir("gif_frames")

	for i in range(frames):
		file_name = "gif_frames/%03d.png" % i

		surface.write_to_png(file_name)

		print("Wrote:", file_name, surface.next())

if __name__ == "__main__":
	[t() for t in TESTS]

	print("Writing python-module.png to display all of the results.")

	surface.write_to_png("basic-tests.png")

