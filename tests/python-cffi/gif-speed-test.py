import cairocks
import time

def time_test(surface, mult):
	t = time.time()

	for i in range(surface.num_frames * mult):
		surface.next()

	print("Took %.04f seconds for %d full animation cyles." % (
		time.time() - t,
		mult,
	))

if __name__ == "__main__":
	surface = cairocks.GIFSurface("../data/test.gif")

	time_test(surface, 1)

	surface1 = surface

	time_test(surface1, 25)

	surface2 = surface1

	time_test(surface2, 50)

