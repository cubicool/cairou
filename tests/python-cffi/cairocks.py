"""
This is an example CFFI-based (specifically using cairocffi) implementation of
Cairocks for use with Python3. It may be helpful as a starting point for your
own bindings, though I personally tend to prefer Cython.
"""

import cairocffi

cairocffi.ffi.cdef(r"""
cairo_bool_t cairocks_rounded_rectangle(
	cairo_t*            cr,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius,
	const cairo_bool_t* corners
);

cairo_bool_t cairocks_rounded_rectangle_apply(
	cairo_t*            cr,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius,
	const cairo_bool_t* corners
);

void cairocks_append_named_path(cairo_t* cr, const char* named_path);
cairo_bool_t cairocks_set_named_path(cairo_t* cr, const char* named_path);
void cairocks_named_path_destroy(cairo_t* cr, const char* named_path);

cairo_bool_t cairocks_map_path_onto(
	cairo_t*      cr,
	cairo_path_t* path
);

cairo_surface_t* cairocks_emboss_create(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
);

cairo_bool_t cairocks_emboss(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
);

cairo_surface_t* cairocks_gaussian_blur_create(
	cairo_surface_t* surface,
	double           radius,
	double           deviation
);

cairo_bool_t cairocks_gaussian_blur(
	cairo_surface_t* surface,
	double           radius,
	double           deviation
);

cairo_bool_t cairocks_a8_invert(cairo_surface_t* surface);

cairo_surface_t* cairocks_distance_field_create(
	cairo_surface_t* surface,
	int              scan_size,
	int              block_size
);

cairo_surface_t* cairocks_surface_from_jpeg(const char* file);
cairo_surface_t* cairocks_surface_from_jpeg_data(unsigned char* data, unsigned int size);

cairo_surface_t* cairocks_gif_surface_create(const char* file);
cairo_surface_t* cairocks_gif_surface_create_for_data(unsigned char* data, unsigned int size);

int cairocks_gif_surface_next(cairo_surface_t* surface);

unsigned int cairocks_gif_surface_get_num_frames(cairo_surface_t* surface);
""")

_lib = cairocffi.ffi.dlopen("libcairocks.so")

def _rounded_rectangle(libfunction, cr, x, y, width, height, radius, corners=(True, True, True, True)):
	corners_vals = cairocffi.ffi.new("cairo_bool_t[4]")

	for i, c in enumerate(corners):
		corners_vals[i] = c

	return libfunction(
		cr._pointer,
		x,
		y,
		width,
		height,
		radius,
		corners_vals
	)

def rounded_rectangle(cr, x, y, width, height, radius, corners=(True, True, True, True)):
	return _rounded_rectangle(_lib.cairocks_rounded_rectangle, cr, x, y, width, height, radius, corners)

def rounded_rectangle_apply(cr, x, y, width, height, radius, corners=(True, True, True, True)):
	return _rounded_rectangle(_lib.cairocks_rounded_rectangle_apply, cr, x, y, width, height, radius, corners)

def append_named_path(cr, named_path):
	_lib.cairocks_append_named_path(cr._pointer, named_path.encode("utf8"))

def set_named_path(cr, named_path):
	return _lib.cairocks_set_named_path(cr._pointer, named_path.encode("utf8"))

def map_path_onto(cr, path):
	path_ptr, path_data = cairocffi.context._encode_path(path)

	return _lib.cairocks_map_path_onto(cr._pointer, path_ptr)

def emboss_create(surface, azimuth, elevation, height, ambient, diffuse):
	return cairocffi.Surface._from_pointer(_lib.cairocks_emboss_create(
		surface._pointer,
		azimuth,
		elevation,
		height,
		ambient,
		diffuse
	), True)

def emboss(surface, azimuth, elevation, height, ambient, diffuse):
	return _lib.cairocks_emboss(surface._pointer, azimuth, elevation, height, ambient, diffuse)

def gaussian_blur_create(surface, radius, deviation):
	return cairocffi.Surface._from_pointer(_lib.cairocks_gaussian_blur_create(
		surface._pointer,
		radius,
		deviation
	), True)

def gaussian_blur(surface, radius, deviation):
	return _lib.cairocks_gaussian_blur(surface._pointer, radius, deviation)

def a8_invert(surface):
	return _lib.cairocks_a8_invert(surface._pointer)

def distance_field_create(surface, scan_size, block_size):
	return cairocffi.Surface._from_pointer(_lib.cairocks_distance_field_create(
		surface._pointer,
		scan_size,
		block_size
	), True)

def surface_from_jpeg(path):
	surface = _lib.cairocks_surface_from_jpeg(path.encode("utf8"))

	if not surface:
		raise IOError("Couldn't find file: %s" % path)

	return cairocffi.Surface._from_pointer(surface, True)

def surface_from_jpeg_data(data):
	address, size = cairocffi.surfaces.from_buffer(data)

	surface = _lib.cairocks_surface_from_jpeg_data(
		cairocffi.ffi.cast("unsigned char*", address),
		size
	)

	if not surface:
		raise ValueError("Couldn't load JPEG from data.")

	return cairocffi.Surface._from_pointer(surface, True)

class GIFSurface(cairocffi.Surface):
	def __init__(self, path=None, data=None):
		surface = None

		if not data:
			surface = _lib.cairocks_gif_surface_create(path.encode("utf8"))

			if not surface:
				raise IOError("Couldn't find file: %s" % path)

		else:
			address, size = cairocffi.surfaces.from_buffer(data)

			surface = _lib.cairocks_gif_surface_create_for_data(
				cairocffi.ffi.cast("unsigned char*", address),
				size
			)

			if not surface:
				raise ValueError("Couldn't load GIF from data.")

		cairocffi.Surface.__init__(self, surface)

	@staticmethod
	def create_for_data(data):
		return GIFSurface(data=data)

	def next(self):
		return _lib.cairocks_gif_surface_next(self._pointer)

	@property
	def num_frames(self):
		return _lib.cairocks_gif_surface_get_num_frames(self._pointer)

	get_data   = cairocffi.ImageSurface.get_data
	get_format = cairocffi.ImageSurface.get_format
	get_width  = cairocffi.ImageSurface.get_width
	get_height = cairocffi.ImageSurface.get_height
	get_stride = cairocffi.ImageSurface.get_stride

