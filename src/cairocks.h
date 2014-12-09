#include <cairo.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define CAIROCKS_VERSION_MAJOR  0
#define CAIROCKS_VERSION_MINOR  4
#define CAIROCKS_VERSION_BUGFIX 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * cairocks_rounded_rectangle:
 * @cr: a cairo context
 * @x: the x coordinate
 * @y: the y coordinate
 * @width: the width value
 * @height: the height value
 * @radius: the radius of each arc
 * @corners: a 4-value cairo_bool_t array, or NULL
 *
 * This function creates a common rounded-rectangle shape, with the caveat that each
 * round corner can be optionally turned off by using an array of 4 cairo_bool_t values.
 * These values correspond to: top-left, top-right, bottom-right, and bottom-left corners.
 * By default (when using NULL), all four corners are rounded.
 **/
cairo_bool_t cairocks_rounded_rectangle(
	cairo_t*            cr,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius,
	const cairo_bool_t* corners
);

/**
 * cairocks_rounded_rectangle_apply:
 * @cr: a cairo context
 * @x: the x coordinate
 * @y: the y coordinate
 * @width: the width value
 * @height: the height value
 * @radius: the radius of each arc
 * @corners: a 4-value cairo_bool_t array, or NULL
 *
 * This function, whose signature is identical to cairocks_rounded_rectangle,
 * applies the resultant rounded rectangle and "punches out" the edges of the
 * context passed in as @cr. This can be thougt of as applying a "border" to an
 * existing context/surface, almost like a traditional picture.
 **/
cairo_bool_t cairocks_rounded_rectangle_apply(
	cairo_t*            cr,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius,
	const cairo_bool_t* corners
);

/**
 * cairocks_append_named_path:
 * @cr: a cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * This function appends the current path (in its entirety) to the
 * specified named path. After calling this routine, the current path
 * on the context is cleared.
 **/
cairo_bool_t cairocks_append_named_path(cairo_t* cr, const char* named_path);

/**
 * cairocks_append_named_path_preserve:
 * @cr: a cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * This function appends the current path (in its entirety) to the
 * specified named path. Unlike @cairocks_append_named_path, the path
 * is NOT cleared after a call to this function.
 **/
cairo_bool_t cairocks_append_named_path_preserve(cairo_t* cr, const char* named_path);

/**
 * cairocks_set_named_path:
 * @cr: a cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * This function clears the current path and makes the named path
 * specified by @named_path the current path on the passed-in context.
 * The named path is not deleted, and be reused until either the
 * context to which it is bound is destroyed or the user explicitly calls
 * @cairocks_remove_named_path.
 **/
cairo_bool_t cairocks_set_named_path(cairo_t* cr, const char* named_path);

/**
 * cairocks_remove_named_path:
 * @cr: a cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * Deletes the named path bound the specified context.
 **/
cairo_bool_t cairocks_remove_named_path(cairo_t* cr, const char* named_path);

/**
 * cairocks_map_path_onto:
 * @cr: a cairo context
 * @corners: a cairo path onto which the CURRENT cr path will be mapped
 *
 * This function (taken from Behdad Esfahbod's cairotwisted example) will attempt to map
 * the current path on @cr to the path provided by @path. In the common case you can
 * create an arc or curve, quickly issue a copy_path/copy_path_flat, create a NEW path on @cr
 * and then pass the previously saved path into @path.
 **/
cairo_bool_t cairocks_map_path_onto(
	cairo_t*      cr,
	cairo_path_t* path
);

/**
 * cairocks_create_emboss_create:
 * @surface: a CAIRO_FORMAT_A8 surface
 * @azimuth: the azimuth value, in radians
 * @elevation: the light elevation value, in radians
 * @height: ???
 * @ambient: the ambient light component
 * @diffuse: the diffuse light componnet
 *
 * This function (taken mostly from "Graphics Gems IV") will return a newly-created
 * Cairo image surface created by applying the emboss algorightm on @surface. This
 * new surface can be used to create a new pattern, or used directly with set_source_surface.
 * This function was further adapted by Andrea Canciani to use a Sobel operator to perform
 * the matrix convolution.
 **/
cairo_surface_t* cairocks_emboss_create(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
);

/**
 * cairocks_emboss:
 * @surface: a CAIRO_FORMAT_A8 surface
 * @azimuth: the azimuth value, in radians
 * @elevation: the light elevation value, in radians
 * @height: ???
 * @ambient: the ambient light component
 * @diffuse: the diffuse light componnet
 *
 * This function is identical to the similarly named function @cairocks_emboss_surface_create,
 * except that instead of returning a new surface it will do an IN-PLACE emboss of the passed
 * in @surface.
 **/
cairo_bool_t cairocks_emboss(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
);

/**
 * cairocks_gaussian_blur_create:
 * @surface: a cairo surface (of any format)
 * @radius: the blur radius
 * @deviation: the deviation value used when creating the blur kernel
 *
 * Returns a newly created Cairo image surface with the gaussian_blur algorigthm applied.
 * See the documentation for @cairocks_gaussian_blur.
 **/
cairo_surface_t* cairocks_gaussian_blur_create(
	cairo_surface_t* surface,
	double           radius,
	double           deviation
);

/**
 * cairocks_gaussian_blur:
 * @surface: a cairo surface (of any format)
 * @radius: the blur radius
 * @deviation: the deviation value used when creating the blur kernel
 *
 * Performs an IN-PLACE gaussian blur on @surface. This function was taken from an
 * example provided by MacSlow (Mirco Mueller), though the original unmodified code
 * has been lost in the ether.
 **/
cairo_bool_t cairocks_gaussian_blur(
	cairo_surface_t* surface,
	double           radius,
	double           deviation
);

/**
 * cairocks_a8_invert:
 * @surface: a CAIRO_FORMAT_A8 surface to be inverted
 *
 * Performs an IN-PLACE invert on @surface. This function can be handy when you
 * need to invert an alpha mask that is easy to stroke in one way, but hard to
 * stroke another (for example, with font glyphs).
 **/
cairo_bool_t cairocks_a8_invert(cairo_surface_t* surface);

/**
 * cairocks_distance_field_create:
 * @surface: a CAIRO_FORMAT_A8 surface
 * @scan_size: size of the "effect" region
 * @block_size: the surface divisor
 *
 * This routine will create an unsigned distance field surface in a CAIRO_FORMAT_A8
 * surface. These surfaces can be used in various calculations, particularly when
 * combined with an appropriate GLSL shader. For the best results, it is recommended that
 * you rasterize on a large surface type (like 4096x4096) and use a block_size large enough
 * to reduce it to something sensible like 128x128. The scan_size argument will determine
 * how much variation you have in your aliased edges.
 **/
cairo_surface_t* cairocks_distance_field_create(
	cairo_surface_t* surface,
	int              scan_size,
	int              block_size
);

/**
 * cairocks_surface_from_jpeg:
 * @file: path to a JPEG file on disk
 *
 * This routine will return a new cairo_surface_t* initialized from a JPEG file on disk.
 * Current, the only supported format is RGB24.
 **/
cairo_surface_t* cairocks_surface_from_jpeg(const char* file);

/**
 * cairocks_surface_from_jpeg_data:
 * @data: buffer of JPEG data in memory
 * @size: size of JPEG buffer data in bytes
 *
 * This routine will return a new cairo_surface_t* initialized from JPEG data previously
 * loaded into memory. Only the RGB24 Cairo format is suported.
 * TODO: Why is the size needed?
 **/
cairo_surface_t* cairocks_surface_from_jpeg_data(unsigned char* data, unsigned int size);

/**
 * cairocks_surface_from_png_data:
 * @data: buffer of PNG data in memory
 * @size: size of PNG buffer data in bytes
 *
 * This routine will return a new cairo_surface_t* initialized from PNG data previously
 * loaded into memory. It is nothing more (or less) than a simple wrapper over the standard
 * cairo_image_surface_create_from_png_stream.
 **/
cairo_surface_t* cairocks_surface_from_png_data(unsigned char* data);

/**
 * cairocks_gif_surface_create:
 * @file: path to a GIF file on disk
 *
 * This function creates a new image surface with special "user data" attached that
 * represents the internal state of the GIF library. Each new frame is loaded "on
 * demand" with @cairo_gif_surface_next.
 **/
cairo_surface_t* cairocks_gif_surface_create(const char* file);

/**
 * cairocks_gif_surface_create_for_data:
 * @data: buffer of GIF data in memory
 * @size: size of GIF buffer data in bytes
 *
 * This function behaves exactly as @cairocks_gif_surface_create, and differs only in that it
 * allows you specify the memory buffer of GIF data rather than loading it from a file on disk.
 * A copy of the data is made and managed internally; the user may free their own buffer
 * after calling this function.
 **/
cairo_surface_t* cairocks_gif_surface_create_for_data(unsigned char* data, unsigned int size);

/**
 * cairocks_gif_next:
 * @surface: a surface created from @cairocks_gif_surface_create
 *
 * For GIF surfaces which contain multiple frames, advance the frame by one, rewinding to
 * frame 0 in a "loop-like manner" when necessary. This routine returns the amount of time
 * this frame should be displayed in 1/100ths of a second, or 0 if unspecified. If a value
 * less than 0 is returned, some error occurred internally.
 **/
int cairocks_gif_surface_next(cairo_surface_t* surface);

/**
 * cairocks_gif_surface_get_num_frames:
 * @surface: a surface created from @cairocks_gif_surface_create
 *
 * Returns the number of frames inside the GIF image. This is possible not because the
 * GIF format makes it easy but because cairocks performs a very fast "quick pass" over
 * the image during initialization that doesn't actually load any data, but determines
 * how many frames (or records) are in the file.
 **/
unsigned int cairocks_gif_surface_get_num_frames(cairo_surface_t* surface);

/**
 * cairocks_text_flags_t:
 * @CAIROCKS_BOLD: enables the bold Cairo font weight.
 * @CAIROCKS_ITACLI: enables the italic Cairo font slant.
 * @CAIROCKS_X_LEFT: aligns the entire text body to the right of the X position.
 * @CAIROCKS_X_CENTER: aligns the entire text body in the middle of the X position.
 * @CAIROCKS_X_RIGHT: aligns the entire text body to the left of the X position.
 * @CAIROCKS_X_BASELINE: the default X text alignment.
 * @CAIROCKS_Y_TOP: aligns the entire text body below the Y position.
 * @CAIROCKS_Y_CENTER: aligns the entire text body in the middle of the Y position.
 * @CAIROCKS_Y_BOTTOM: aligns the entire text body above the Y position.
 * @CAIROCKS_Y_BASELINE: the default Y text alignment.
 * @CAIROCKS_ALIGN_LEFT: aligns each line of text in the text body to the left.
 * @CAIROCKS_ALIGN_RIGHT: aligns each line of text in the text body to the right.
 * @CAIROCKS_ALIGN_LEFT: aligns each line of text in the text body to the center.
 *
 * #cairo_text_flags_t is used a bitfield to combine one or more text flags into
 * a single parameter. Note that the ALIGN flags differ from the X/Y flags in that
 * they do not specify the absolute position of the text, but rather how each line
 * of text in the text body aligns to eachother.
 *
 * Other flags will be added in the future as this function evolves; particularly
 * the ability use Harfbuzz for layout and the ability to install callbacks to
 * override the rendering of each glyph.
 **/
typedef enum _cairocks_text_flags {
	CAIROCKS_BOLD = 1 << 0,
	CAIROCKS_ITALIC = 1 << 1,
	CAIROCKS_X_LEFT = 1 << 2,
	CAIROCKS_X_CENTER = 1 << 3,
	CAIROCKS_X_RIGHT = 1 << 4,
	CAIROCKS_X_BASELINE = 1 << 5,
	CAIROCKS_Y_TOP = 1 << 6,
	CAIROCKS_Y_CENTER = 1 << 7,
	CAIROCKS_Y_BOTTOM = 1 << 8,
	CAIROCKS_Y_BASELINE = 1 << 9,
	CAIROCKS_ALIGN_LEFT = 1 << 10,
	CAIROCKS_ALIGN_RIGHT = 1 << 11,
	CAIROCKS_ALIGN_JUSTIFY = 1 << 12
} cairocks_text_flags_t;

/**
 * cairocks_show_text:
 * @cr: a cairo context
 * @utf8: a UTF8-encoded string of text
 * @font: the font name (to be passed to cairo_select_font)
 * @size: the font size, in user coordinates
 * @x: the X position of the text
 * @y: the Y position of the text
 * @flags: a bitfield of cairocks_text_flags_t options
 *
 * This function brings together huge portions of the Cairo "toy" text API
 * and attempts to make it even easier to use by providing a number of common
 * flags and text positioning options. While serious text handling is certainly
 * outside the scope of what Cairo can (and should) do, Cairo's "toy" API is
 * often overlooked, and its usage is sometimes overly-discouraged, even though
 * it can perform quite nicely.
 *
 * The bulk of the magic facilitated by this routine is described in the various
 * flags documentation.
 **/
cairo_bool_t cairocks_show_text(
	cairo_t*    cr,
	const char* utf8,
	const char* font,
	double      size,
	double      x,
	double      y,
	int         flags
);

/**
 * cairocks_text_path:
 * @cr: a cairo context
 * @utf8: a UTF8-encoded string of text
 * @font: the font name (to be passed to cairo_select_font)
 * @size: the font size, in user coordinates
 * @x: the X position of the text
 * @y: the Y position of the text
 * @flags: a bitfield of cairocks_text_flags_t options
 *
 * This function behaves just like show_text, but calls text_path
 * as the underlying Cairo function.
 **/
cairo_bool_t cairocks_text_path(
	cairo_t*    cr,
	const char* utf8,
	const char* font,
	double      size,
	double      x,
	double      y,
	int         flags
);

/**
 * cairocks_text_extents:
 * @cr: a cairo context
 * @utf8: a UTF8-encoded string of text
 * @font: the font name (to be passed to cairo_select_font)
 * @size: the font size, in user coordinates
 * @x: the X position of the text
 * @y: the Y position of the text
 * @flags: a bitfield of cairocks_text_flags_t options
 * @rect_extents: an array of doubles representing the final drawn extents rectangle
 *
 * Similar to the the standard cairo_text_extents function, this routine will return
 * the extents applicable with the various flags also set. Optionally, if @rect_extents
 * is non-NULL, it will return 4 doubles specifying a rectangle (in Cairo parlance) that
 * defines the final, inked rectangle extents of the text body.
 **/
cairo_bool_t cairocks_text_extents(
	cairo_t*              cr,
	const char*           utf8,
	const char*           font,
	double                size,
	double                x,
	double                y,
	int                   flags,
	cairo_text_extents_t* extents,
	double*               rect_extents
);

#ifdef __cplusplus
}
#endif

