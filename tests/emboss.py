#!/usr/bin/env python

import pygtk; pygtk.require("2.0")
import gtk
import cairocffi as cairo
import math
import cairocks

cairocffi.install_as_pycairo()

class CairoDrawingArea(gtk.DrawingArea):
	def __init__(self, width, height, original=False):
		gtk.DrawingArea.__init__(self)

		self.Azimuth   = 180.0
		self.Elevation = 45.0
		self.Height    = 100.0
		self.Ambient   = 0.1
		self.Diffuse   = 0.1

		self.__original = original

		self.set_size_request(width, height)

		self.connect("expose-event", self.__render)

	def value_changed(self, widget, name):
		setattr(self, name, widget.get_value())

		self.queue_draw()

	def __render(self, widget, event):
		cr   = self.window.cairo_create()
		w, h = self.allocation.width, self.allocation.height

		if not self.__original:
			s = cairo.ImageSurface(cairo.FORMAT_A8, w, h)
			c = cairo.Context(s)

			self.__draw_cairo(c, w, h)

			cairocks.emboss(
				s,
				float(self.Azimuth) * (math.pi / 180.0),
				float(self.Elevation) * (math.pi / 180.0),
				float(self.Height),
				float(self.Ambient),
				float(self.Diffuse)
			)

			cr.set_source_surface(s, 0, 0)
			cr.paint()

		else:
			self.__draw_cairo(cr, w, h)

		return True

	def __draw_cairo_shape(self, cr, w, h):
		# c.set_source_rgba(1.0, 1.0, 1.0, 1.0)
		# c.arc(w / 2.0, h / 2.0, w / 4.0, 0.0, 2.0 * math.pi)
		# c.fill()

		cr.set_source_rgba(1.0, 1.0, 1.0, 0.1)

		strokes = 40

		for i in range(strokes, 0, -4):
			cr.set_line_width(i)
			cr.arc(w / 2.0, h / 2.0, w / 3.0, 0.0, 2.0 * math.pi)
			cr.stroke()

	def __draw_cairo(self, cr, w, h):
		text = "Cairo"

		cr.select_font_face("Droid Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD)
		cr.set_font_size(100)

		extents = cr.text_extents(text)

		cr.move_to(((w - extents[2]) / 2.0) - extents[0], (h - extents[1]) / 2.0)
		cr.show_text(text)

class CairoWindow(gtk.Window):
	def __init__(self, w, h, script=None, autosave=None):
		gtk.Window.__init__(self, gtk.WINDOW_TOPLEVEL)

		vbox  = gtk.VBox()
		draw1 = CairoDrawingArea(w, h)
		draw2 = CairoDrawingArea(w, h, True)
		table = gtk.Table(5, 2, True)

		scale_data = (
			("Azimuth", 0.0, 180.0, 1.0),
			("Elevation", 0.0, 180.0, 1.0),
			("Height", 0.0, 200.0, 5.0),
			("Ambient", 0.1, 1.0, 0.1),
			("Diffuse", 0.1, 1.0, 0.1)
		)

		scales = []

		for name, minv, maxv, inc in scale_data:
			label = gtk.Label(name)
			scale = gtk.HScale()

			label.set_alignment(0.0, 0.5)

			scale.set_range(minv, maxv)
			scale.set_increments(inc, inc)
			scale.set_value(getattr(draw1, name))
			scale.set_value_pos(gtk.POS_LEFT)
			scale.connect("value-changed", draw1.value_changed, name)

			scales.append((label, scale))

		table.attach(scales[0][0], 0, 1, 0, 1)
		table.attach(scales[0][1], 1, 2, 0, 1)
		table.attach(scales[1][0], 0, 1, 1, 2)
		table.attach(scales[1][1], 1, 2, 1, 2)
		table.attach(scales[2][0], 0, 1, 2, 3)
		table.attach(scales[2][1], 1, 2, 2, 3)
		table.attach(scales[3][0], 0, 1, 3, 4)
		table.attach(scales[3][1], 1, 2, 3, 4)
		table.attach(scales[4][0], 0, 1, 4, 5)
		table.attach(scales[4][1], 1, 2, 4, 5)

		vbox.pack_start(draw1)
		vbox.pack_start(gtk.HSeparator(), False, False)
		vbox.pack_start(draw2)
		vbox.pack_start(gtk.HSeparator(), False, False)
		vbox.pack_start(table, False, False)

		self.add(vbox)

if __name__ == "__main__":
	cw = CairoWindow(256, 256)

	cw.connect("destroy", gtk.main_quit)
	cw.show_all()

	gtk.main()
