import cairocffi as cairo
import cairocks
import math

def create_arc(w, h):
	surface = cairo.ImageSurface(cairo.FORMAT_A8, w, h)
	cr      = cairo.Context(surface)

	cr.arc(w / 2, h / 2, w / 3.0, 0.0, 2.0 * math.pi)
	cr.fill()

	return surface

if __name__ == "__main__":
	arc = create_arc(608, 608)
	df  = cairocks.distance_field_create(arc, 150, 16)

	arc.write_to_png("arc.png")
	df.write_to_png("distance-field.png")

