"""
This is an example CFFI-based (specifically using cairocffi) implementation of
Cairocks for use with Python3. It may be helpful as a starting point for your
own bindings, though I personally tend to prefer Cython.
"""

import cairocffi
import contextlib

cairocffi.ffi.cdef(r"""
cairo_bool_t cairocks_rounded_rectangle(
    cairo_t* cr,
    double x,
    double y,
    double width,
    double height,
    double radius,
    const cairo_bool_t* corners
);

cairo_bool_t cairocks_rounded_rectangle_apply(
    cairo_t* cr,
    double x,
    double y,
    double width,
    double height,
    double radius,
    const cairo_bool_t* corners
);

cairo_bool_t cairocks_append_named_path(
    cairo_t* cr,
    const char* named_path
);

cairo_bool_t cairocks_append_named_path_preserve(
    cairo_t* cr,
    const char* named_path
);

cairo_bool_t cairocks_set_named_path(
    cairo_t* cr,
    const char* named_path
);

cairo_bool_t cairocks_set_named_path_preserve(
    cairo_t* cr,
    const char* named_path
);

cairo_bool_t cairocks_remove_named_path(
    cairo_t* cr,
    const char* named_path
);

cairo_bool_t cairocks_map_path_onto(
    cairo_t* cr,
    cairo_path_t* path
);

cairo_surface_t* cairocks_emboss_create(
    cairo_surface_t* surface,
    double azimuth,
    double elevation,
    double height,
    double ambient,
    double diffuse
);

cairo_bool_t cairocks_emboss(
    cairo_surface_t* surface,
    double azimuth,
    double elevation,
    double height,
    double ambient,
    double diffuse
);

cairo_surface_t* cairocks_gaussian_blur_create(
    cairo_surface_t* surface,
    double radius,
    double deviation
);

cairo_bool_t cairocks_gaussian_blur(
    cairo_surface_t* surface,
    double radius,
    double deviation
);

cairo_bool_t cairocks_a8_invert(cairo_surface_t* surface);

cairo_surface_t* cairocks_distance_field_create(
    cairo_surface_t* surface,
    int scan_size,
    int block_size
);

cairo_surface_t* cairocks_surface_from_jpeg(const char* file);
cairo_surface_t* cairocks_surface_from_jpeg_data(
    unsigned char* data,
    unsigned int size
);

cairo_surface_t* cairocks_surface_from_png_data(unsigned char* data);

cairo_surface_t* cairocks_gif_surface_create(const char* file);
cairo_surface_t* cairocks_gif_surface_create_for_data(
    unsigned char* data,
    unsigned int size
);

int cairocks_gif_surface_next(cairo_surface_t* surface);

unsigned int cairocks_gif_surface_get_num_frames(cairo_surface_t* surface);

cairo_bool_t cairocks_show_text(
    cairo_t* cr,
    const char* utf8,
    const char* font,
    double size,
    double x,
    double y,
    int flags
);

cairo_bool_t cairocks_text_path(
    cairo_t* cr,
    const char* utf8,
    const char* font,
    double size,
    double x,
    double y,
    int flags
);

cairo_bool_t cairocks_text_extents(
    cairo_t* cr,
    const char* utf8,
    const char* font,
    double size,
    double x,
    double y,
    int flags,
    cairo_text_extents_t* extents,
    double* rect_extents
);

typedef struct _cairocks_point_t {
    double x;
    double y;
} cairocks_point_t;

cairo_bool_t cairocks_append_spline(
    cairo_t* cr,
    cairocks_point_t* points,
    int num_points,
    cairo_bool_t closed
);

typedef enum _cairocks_icon_t cairocks_icon_t;

cairo_bool_t cairocks_show_icon(
    cairo_t* cr,
    cairocks_icon_t icon,
    double size,
    double x,
    double y,
    unsigned int flags
);

cairo_bool_t cairocks_icon_path(
    cairo_t* cr,
    cairocks_icon_t icon,
    double size,
    double x,
    double y,
    unsigned int flags
);

cairo_bool_t cairocks_icon_extents(
    cairo_t* cr,
    cairocks_icon_t icon,
    double size,
    double x,
    double y,
    unsigned int flags,
    double* extents
);

cairocks_icon_t cairocks_icon_from_string(const char* icon);
""")

_lib = cairocffi.ffi.dlopen("libcairocks.so")

# We need to keep references around to the original methods if we
# choose to "merge" the cairockscffi API with cairocffi.
_CONTEXT_METHODS = {}
_SURFACE_METHODS = {}


def _rounded_rectangle(
    libfunction,
    cr,
    x,
    y,
    width,
    height,
    radius,
    corners=(True, True, True, True)
):
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


def rounded_rectangle(
    cr,
    x,
    y,
    width,
    height,
    radius,
    corners=(True, True, True, True)
):
    return _rounded_rectangle(
        _lib.cairocks_rounded_rectangle,
        cr,
        x,
        y,
        width,
        height,
        radius,
        corners
    )


def rounded_rectangle_apply(
    cr,
    x,
    y,
    width,
    height,
    radius,
    corners=(True, True, True, True)
):
    return _rounded_rectangle(
        _lib.cairocks_rounded_rectangle_apply,
        cr,
        x,
        y,
        width,
        height,
        radius,
        corners
    )


def append_named_path(cr, named_path):
    return _lib.cairocks_append_named_path(
        cr._pointer,
        named_path.encode("utf8")
    )


def append_named_path_preserve(cr, named_path):
    return _lib.cairocks_append_named_path_preserve(
        cr._pointer,
        named_path.encode("utf8")
    )


def set_named_path(cr, named_path=""):
    return _lib.cairocks_set_named_path(
        cr._pointer,
        named_path.encode("utf8")
    )


def set_named_path_preserve(cr, named_path=""):
    return _lib.cairocks_set_named_path_preserve(
        cr._pointer,
        named_path.encode("utf8")
    )


def remove_named_path(cr, named_path):
    return _lib.cairocks_remove_named_path(
        cr._pointer,
        named_path.encode("utf8")
    )


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
    return _lib.cairocks_emboss(
        surface._pointer,
        azimuth,
        elevation,
        height,
        ambient,
        diffuse
    )


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


def surface_from_png_data(data):
    address, size = cairocffi.surfaces.from_buffer(data)

    surface = _lib.cairocks_surface_from_png_data(
        cairocffi.ffi.cast("unsigned char*", address),
    )

    if not surface:
        raise ValueError("Couldn't load PNG from data.")

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

    get_data = cairocffi.ImageSurface.get_data
    get_format = cairocffi.ImageSurface.get_format
    get_width = cairocffi.ImageSurface.get_width
    get_height = cairocffi.ImageSurface.get_height
    get_stride = cairocffi.ImageSurface.get_stride


TEXT_BOLD = 1 << 0
TEXT_ITALIC = 1 << 1
TEXT_X_LEFT = 1 << 2
TEXT_X_CENTER = 1 << 3
TEXT_X_RIGHT = 1 << 4
TEXT_X_BASELINE = 1 << 5
TEXT_Y_TOP = 1 << 6
TEXT_Y_CENTER = 1 << 7
TEXT_Y_BOTTOM = 1 << 8
TEXT_Y_BASELINE = 1 << 9
TEXT_ALIGN_LEFT = 1 << 10
TEXT_ALIGN_RIGHT = 1 << 11
TEXT_ALIGN_CENTER = 1 << 12
TEXT_NO_SAVE_RESTORE = 1 << 13
TEXT_PIXEL_ALIGN = 1 << 14

# Our special Python-only flag that tells the wrappers to defer to the Cairo
# default functions, instead of our (potentially merged) wrappers. This isn't
# a part of Cairocks itself, since it's written in C and can't hijack or
# monkey-patch an API.
TEXT_USE_DEFAULT = 1 << 31


def show_text(
    cr,
    utf8,
    font="sans-serif",
    size=10.0,
    x=0.0,
    y=0.0,
    flags=0
):
    if flags & TEXT_USE_DEFAULT:
        return _CONTEXT_METHODS["show_text"](cr, utf8)

    return _lib.cairocks_show_text(
        cr._pointer,
        utf8.encode("utf8"),
        font.encode("utf8"),
        size,
        x,
        y,
        flags
    )


def text_path(
    cr,
    utf8,
    font="sans-serif",
    size=10.0,
    x=0.0,
    y=0.0,
    flags=0
):
    if flags & TEXT_USE_DEFAULT:
        return _CONTEXT_METHODS["text_path"](cr, utf8)

    return _lib.cairocks_text_path(
        cr._pointer,
        utf8.encode("utf8"),
        font.encode("utf8"),
        size,
        x,
        y,
        flags
    )


def text_extents(
    cr,
    utf8,
    font="sans-serif",
    size=10.0,
    x=0.0,
    y=0.0,
    flags=0,
    rect_extents=False
):
    if flags & TEXT_USE_DEFAULT:
        return _CONTEXT_METHODS["text_extents"](cr, utf8)

    extents = cairocffi.ffi.new("cairo_text_extents_t*")
    rect_vals = cairocffi.ffi.new("double[4]")

    _lib.cairocks_text_extents(
        cr._pointer,
        utf8.encode("utf8"),
        font.encode("utf8"),
        size,
        x,
        y,
        flags,
        extents,
        rect_extents and rect_vals or cairocffi.ffi.NULL
    )

    ex = (
        extents.x_bearing,
        extents.y_bearing,
        extents.width,
        extents.height,
        extents.x_advance,
        extents.y_advance
    )

    if rect_extents:
        return ex, (
            rect_vals[0],
            rect_vals[1],
            rect_vals[2],
            rect_vals[3]
        )

    else:
        return ex


def append_spline(cr, spline, closed=False):
    points = cairocffi.ffi.new("cairocks_point_t[%d]" % len(spline))

    for i, p in enumerate(spline):
        points[i].x = p[0]
        points[i].y = p[1]

    _lib.cairocks_append_spline(cr._pointer, points, len(spline), closed)


ICON_X_LEFT = 1 << 2
ICON_X_CENTER = 1 << 3
ICON_X_RIGHT = 1 << 4
ICON_Y_TOP = 1 << 6
ICON_Y_CENTER = 1 << 7
ICON_Y_BOTTOM = 1 << 8


def show_icon(
    cr,
    icon,
    size=10.0,
    x=0.0,
    y=0.0,
    flags=0
):
    return _lib.cairocks_show_icon(
        cr._pointer,
        icon,
        size,
        x,
        y,
        flags
    )


def icon_path(
    cr,
    icon,
    size=10.0,
    x=0.0,
    y=0.0,
    flags=0
):
    return _lib.cairocks_icon_path(
        cr._pointer,
        icon,
        size,
        x,
        y,
        flags
    )


def icon_extents(
    cr,
    icon,
    size=10.0,
    x=0.0,
    y=0.0,
    flags=0
):
    extents = cairocffi.ffi.new("double[4]")

    return _lib.cairocks_icon_extents(
        cr._pointer,
        icon,
        size,
        x,
        y,
        flags,
        extents
    )

    return (
        extents[0],
        extents[1],
        extents[2],
        extents[3]
    )


def icon_from_string(icon):
    return _lib.cairocks_icon_from_string(icon.encode("ascii"))


# From: http://preshing.com/20110920/the-python-with-statement-by-example
# Wraps the subsequent code in save/restore calls.
@contextlib.contextmanager
def saved(cr):
    cr.save()

    try:
        yield cr

    finally:
        cr.restore()


# Wraps the subsequent code in save/restore calls, and appends the current
# path to the specified named path.
@contextlib.contextmanager
def named_path(cr, name=""):
    cr.save()

    try:
        yield cr

    finally:
        cr.append_named_path(name)

        cr.restore()


# Monkey-patches the Cairockscffi API into the Cairocffi namespace.
def merge_with_cairocffi():
    def method_wrap(func):
        def method_wrapped_func(self, *args, **kwargs):
            return func(self, *args, **kwargs)

        return method_wrapped_func

    for method in (
        rounded_rectangle,
        rounded_rectangle_apply,
        append_named_path,
        append_named_path_preserve,
        set_named_path,
        set_named_path_preserve,
        remove_named_path,
        map_path_onto,
        show_text,
        text_path,
        text_extents,
        append_spline,
        show_icon,
        icon_path,
        icon_extents,
        icon_from_string,
        saved,
        named_path
    ):
        mn = method.__name__

        if hasattr(cairocffi.Context, mn):
            _CONTEXT_METHODS[mn] = getattr(cairocffi.Context, mn)

        setattr(cairocffi.Context, mn, method_wrap(method))

    for method in (
        emboss,
        gaussian_blur_create,
        gaussian_blur,
        a8_invert,
        distance_field_create
    ):
        mn = method.__name__

        if hasattr(cairocffi.Surface, mn):
            _SURFACE_METHODS[mn] = getattr(cairocffi.Surface, mn)

        setattr(cairocffi.Surface, mn, method_wrap(method))

    for const in (
        "TEXT_BOLD",
        "TEXT_ITALIC",
        "TEXT_X_LEFT",
        "TEXT_X_CENTER",
        "TEXT_X_RIGHT",
        "TEXT_X_BASELINE",
        "TEXT_Y_TOP",
        "TEXT_Y_CENTER",
        "TEXT_Y_BOTTOM",
        "TEXT_Y_BASELINE",
        "TEXT_ALIGN_LEFT",
        "TEXT_ALIGN_RIGHT",
        "TEXT_ALIGN_CENTER",
        "TEXT_NO_SAVE_RESTORE",
        "TEXT_PIXEL_ALIGN",
        "TEXT_USE_DEFAULT",
        "ICON_X_LEFT",
        "ICON_X_CENTER",
        "ICON_X_RIGHT",
        "ICON_Y_TOP",
        "ICON_Y_CENTER",
        "ICON_Y_BOTTOM"
    ):
        setattr(cairocffi, const, globals()[const])
