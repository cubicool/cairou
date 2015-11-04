#ifndef CAIROU_H
#define CAIROU_H 1

#include "cairou-glyphicons.h"

#include <cairo.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define CAIROU_VERSION_MAJOR 1
#define CAIROU_VERSION_MINOR 0
#define CAIROU_VERSION_BUGFIX 0

#ifdef __cplusplus
extern "C" {
#endif

/**
 * cairou_append_named_path:
 * @cr: a Cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * This function appends the current path (in its entirety) to the
 * specified named path. After calling this routine, the current path
 * on the context is cleared.
 *
 * Note that this funtion uses "append" in its name (as opposed to create),
 * due to the fact that you can append multiple Cairo paths to a single named
 * path object; the first append implicitly "creates" the named path.
 **/
cairo_bool_t cairou_append_named_path(cairo_t* cr, const char* named_path);

/**
 * cairou_append_named_path_preserve:
 * @cr: a Cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * This function appends the current path (in its entirety) to the
 * specified named path. Unlike @cairou_append_named_path, the path
 * is NOT cleared after a call to this function.
 **/
cairo_bool_t cairou_append_named_path_preserve(cairo_t* cr, const char* named_path);

/**
 * cairou_set_named_path:
 * @cr: a Cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * This function clears the current path and makes the named path
 * specified by @named_path the current path on the passed-in context.
 * The named path is not deleted, and can be reused until either the
 * context to which it is bound is destroyed or the user explicitly calls
 * @cairou_remove_named_path.
 *
 * If a blank string (or NULL) is used as @named_path, the most recently
 * appended-to named path object is used.
 **/
cairo_bool_t cairou_set_named_path(cairo_t* cr, const char* named_path);

/**
 * cairou_set_named_path_preserve:
 * @cr: a Cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * This function behaves exactly as cairou_set_named_path(), except
 * that the current path on @cr is NOT cleared.
 **/
cairo_bool_t cairou_set_named_path_preserve(cairo_t* cr, const char* named_path);

/**
 * cairou_remove_named_path:
 * @cr: a Cairo context
 * @named_path: a string representing the name (or key) for the path
 *
 * Deletes the named path bound the specified context.
 **/
cairo_bool_t cairou_remove_named_path(cairo_t* cr, const char* named_path);

/**
 * cairou_map_path_onto:
 * @cr: a Cairo context
 * @corners: a cairo path onto which the CURRENT cr path will be mapped
 *
 * This function (taken from Behdad Esfahbod's cairotwisted example) will attempt to map
 * the current path on @cr to the path provided by @path. In the common case you can
 * create an arc or curve, quickly issue a copy_path/copy_path_flat, create a NEW path on @cr
 * and then pass the previously saved path into @path.
 **/
cairo_bool_t cairou_map_path_onto(
	cairo_t* cr,
	cairo_path_t* path
);

/**
 * cairou_create_emboss_create:
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
cairo_surface_t* cairou_emboss_create(
	cairo_surface_t* surface,
	double azimuth,
	double elevation,
	double height,
	double ambient,
	double diffuse
);

/**
 * cairou_emboss:
 * @surface: a CAIRO_FORMAT_A8 surface
 * @azimuth: the azimuth value, in radians
 * @elevation: the light elevation value, in radians
 * @height: ???
 * @ambient: the ambient light component
 * @diffuse: the diffuse light componnet
 *
 * This function is identical to the similarly named function @cairou_emboss_surface_create,
 * except that instead of returning a new surface it will do an IN-PLACE emboss of the passed
 * in @surface.
 **/
cairo_bool_t cairou_emboss(
	cairo_surface_t* surface,
	double azimuth,
	double elevation,
	double height,
	double ambient,
	double diffuse
);

/**
 * cairou_gaussian_blur_create:
 * @surface: a cairo surface (of any format)
 * @radius: the blur radius
 * @deviation: the deviation value used when creating the blur kernel
 *
 * Returns a newly created Cairo image surface with the gaussian_blur algorigthm applied.
 * See the documentation for @cairou_gaussian_blur.
 **/
cairo_surface_t* cairou_gaussian_blur_create(
	cairo_surface_t* surface,
	double radius,
	double deviation
);

/**
 * cairou_gaussian_blur:
 * @surface: a cairo surface (of any format)
 * @radius: the blur radius
 * @deviation: the deviation value used when creating the blur kernel
 *
 * Performs an IN-PLACE gaussian blur on @surface. This function was taken from an
 * example provided by MacSlow (Mirco Mueller), though the original unmodified code
 * has been lost in the ether.
 **/
cairo_bool_t cairou_gaussian_blur(
	cairo_surface_t* surface,
	double radius,
	double deviation
);

/**
 * cairou_a8_invert:
 * @surface: a CAIRO_FORMAT_A8 surface to be inverted
 *
 * Performs an IN-PLACE invert on @surface. This function can be handy when you
 * need to invert an alpha mask that is easy to stroke in one way, but hard to
 * stroke another (for example, with font glyphs).
 **/
cairo_bool_t cairou_a8_invert(cairo_surface_t* surface);

/**
 * cairou_distance_field_create:
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
cairo_surface_t* cairou_distance_field_create(
	cairo_surface_t* surface,
	int scan_size,
	int block_size
);

/**
 * cairou_surface_from_jpeg:
 * @file: path to a JPEG file on disk
 *
 * This routine will return a new cairo_surface_t* initialized from a JPEG file on disk.
 * Current, the only supported Cairo format is RGB24.
 **/
cairo_surface_t* cairou_surface_from_jpeg(const char* file);

/**
 * cairou_surface_from_jpeg_data:
 * @data: buffer of JPEG data in memory
 * @size: size of JPEG buffer data in bytes
 *
 * This routine will return a new cairo_surface_t* initialized from JPEG data previously
 * loaded into memory. Only the RGB24 Cairo format is suported.
 * TODO: Why is the size needed?
 **/
cairo_surface_t* cairou_surface_from_jpeg_data(unsigned char* data, unsigned int size);

/**
 * cairou_surface_from_png_data:
 * @data: buffer of PNG data in memory
 * @size: size of PNG buffer data in bytes
 *
 * This routine will return a new cairo_surface_t* initialized from PNG data previously
 * loaded into memory. It is nothing more (or less) than a simple wrapper over the standard
 * cairo_image_surface_create_from_png_stream.
 **/
cairo_surface_t* cairou_surface_from_png_data(unsigned char* data);

/**
 * cairou_gif_surface_create:
 * @file: path to a GIF file on disk
 *
 * This function creates a new image surface with special "user data" attached that
 * represents the internal state of the GIF library. Each new frame is loaded "on
 * demand" with @cairo_gif_surface_next.
 **/
cairo_surface_t* cairou_gif_surface_create(const char* file);

/**
 * cairou_gif_surface_create_for_data:
 * @data: buffer of GIF data in memory
 * @size: size of GIF buffer data in bytes
 *
 * This function behaves exactly as @cairou_gif_surface_create, and differs only in that it
 * allows you specify the memory buffer of GIF data rather than loading it from a file on disk.
 * A copy of the data is made and managed internally; the user may free their own buffer
 * after calling this function.
 **/
cairo_surface_t* cairou_gif_surface_create_for_data(unsigned char* data, unsigned int size);

/**
 * cairou_gif_next:
 * @surface: a surface created from @cairou_gif_surface_create
 *
 * For GIF surfaces which contain multiple frames, advance the frame by one, rewinding to
 * frame 0 in a "loop-like manner" when necessary. This routine returns the amount of time
 * this frame should be displayed in 1/100ths of a second, or 0 if unspecified. If a value
 * less than 0 is returned, some error occurred internally.
 **/
int cairou_gif_surface_next(cairo_surface_t* surface);

/**
 * cairou_gif_surface_get_num_frames:
 * @surface: a surface created from @cairou_gif_surface_create
 *
 * Returns the number of frames inside the GIF image. This is possible not because the
 * GIF format makes it easy but because cairou performs a very fast "quick pass" over
 * the image during initialization that doesn't actually load any data, but determines
 * how many frames (or records) are in the file.
 **/
unsigned int cairou_gif_surface_get_num_frames(cairo_surface_t* surface);

/**
 * cairou_text_flags_t:
 * @CAIROU_TEXT_BOLD: enables the bold Cairo font weight.
 * @CAIROU_TEXT_ITALIC: enables the italic Cairo font slant.
 * @CAIROU_TEXT_X_LEFT: aligns the entire text body to the right of the X position.
 * @CAIROU_TEXT_X_CENTER: aligns the entire text body in the middle of the X position.
 * @CAIROU_TEXT_X_RIGHT: aligns the entire text body to the left of the X position.
 * @CAIROU_TEXT_X_BASELINE: the default, natural X text alignment.
 * @CAIROU_TEXT_Y_TOP: aligns the entire text body below the Y position.
 * @CAIROU_TEXT_Y_CENTER: aligns the entire text body in the middle of the Y position.
 * @CAIROU_TEXT_Y_BOTTOM: aligns the entire text body above the Y position.
 * @CAIROU_TEXT_Y_BASELINE: the default, natural Y text alignment.
 * @CAIROU_TEXT_ALIGN_LEFT: aligns each line of text in the text body to the left; default.
 * @CAIROU_TEXT_ALIGN_RIGHT: aligns each line of text in the text body to the right.
 * @CAIROU_TEXT_ALIGN_CENTER: aligns each line of text in the text body to the center.
 * @CAIROU_TEXT_NO_SAVE_RESTORE: do NOT protect text drawing with cairo_save()/cairo_restore(),
 * which is normally the default behavior. This can be useful when using text with named
 * paths.
 * @CAIROU_TEXT_PIXEL_ALIGN: rounds all calculated X and Y values so that they occur on
 * pixel boundaries; useful for very small text.
 *
 * #cairo_text_flags_t is used as a bitfield to combine one or more text flags into
 * a single parameter. Note that the ALIGN flags differ from the X/Y flags in that
 * they do not specify the absolute position of the text, but rather how each line
 * of text in the text body aligns to eachother. Note that when positioning a body
 * of text, the default alignment is BASELINE on both the X and Y axes. This is the
 * "natural" alignment of text, wherein you can conceivably have parts of the glyph
 * to-the-left-of and below the specified origin.
 *
 * Other flags will be added in the future as this function evolves; particularly
 * the ability use Harfbuzz for layout and the ability to install callbacks to
 * override the rendering of each glyph.
 **/
typedef enum _cairou_text_flags_t {
	CAIROU_TEXT_BOLD = 1 << 0,
	CAIROU_TEXT_ITALIC = 1 << 1,
	CAIROU_TEXT_X_LEFT = 1 << 2,
	CAIROU_TEXT_X_CENTER = 1 << 3,
	CAIROU_TEXT_X_RIGHT = 1 << 4,
	CAIROU_TEXT_X_BASELINE = 1 << 5,
	CAIROU_TEXT_Y_TOP = 1 << 6,
	CAIROU_TEXT_Y_CENTER = 1 << 7,
	CAIROU_TEXT_Y_BOTTOM = 1 << 8,
	CAIROU_TEXT_Y_BASELINE = 1 << 9,
	CAIROU_TEXT_ALIGN_LEFT = 1 << 10,
	CAIROU_TEXT_ALIGN_RIGHT = 1 << 11,
	CAIROU_TEXT_ALIGN_CENTER = 1 << 12,
	CAIROU_TEXT_NO_SAVE_RESTORE = 1 << 13,
	CAIROU_TEXT_PIXEL_ALIGN = 1 << 14
} cairou_text_flags_t;

/**
 * cairou_show_text:
 * @cr: a Cairo context
 * @utf8: a UTF8-encoded string of text
 * @font: the font name (to be passed to cairo_select_font)
 * @size: the font size, in user coordinates
 * @x: the X position of the text
 * @y: the Y position of the text
 * @flags: a bitfield of cairou_text_flags_t options
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
cairo_bool_t cairou_show_text(
	cairo_t* cr,
	const char* utf8,
	const char* font,
	double size,
	double x,
	double y,
	unsigned int flags
);

/**
 * cairou_text_path:
 * @cr: a Cairo context
 * @utf8: a UTF8-encoded string of text
 * @font: the font name (to be passed to cairo_select_font)
 * @size: the font size, in user coordinates
 * @x: the X position of the text
 * @y: the Y position of the text
 * @flags: a bitfield of cairou_text_flags_t options
 *
 * This function behaves just like show_text, but calls text_path
 * as the underlying Cairo function.
 **/
cairo_bool_t cairou_text_path(
	cairo_t* cr,
	const char* utf8,
	const char* font,
	double size,
	double x,
	double y,
	int flags
);

/**
 * cairou_text_extents:
 * @cr: a Cairo context
 * @utf8: a UTF8-encoded string of text
 * @font: the font name (to be passed to cairo_select_font)
 * @size: the font size, in user coordinates
 * @x: the X position of the text
 * @y: the Y position of the text
 * @flags: a bitfield of cairou_text_flags_t options
 * @extents: a pointer to cairo_text_extents_t to be filled
 * @rect_extents: an array of doubles representing the final drawn extents rectangle
 *
 * Similar to the the standard cairo_text_extents function, this routine will return
 * the extents applicable with the various flags also set. Optionally, if @rect_extents
 * is non-NULL, it will return 4 doubles specifying a rectangle (in Cairo parlance) that
 * defines the final, inked rectangle extents of the text body.
 **/
cairo_bool_t cairou_text_extents(
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

/**
 * cairou_point_t:
 * This structure serves as the primitive unit for each point in a spline.
 */
typedef struct _cairou_point_t {
	double x;
	double y;
} cairou_point_t;

/**
 * cairou_append_spline:
 * @cr: a Cairo context
 * @points: an array of cairou_point_t structures
 * @num_points: number of points in @points
 * @closed: whether or not to close the spline path
 *
 * This routine appends an array of #cairou_point_t instances to the current path.
 * The points define a spline, which is optionally @closed or otherwise.
 */
cairo_bool_t cairou_append_spline(
	cairo_t* cr,
	cairou_point_t* points,
	int num_points,
	cairo_bool_t closed
);

/**
 * cairou_icon_flags_t:
 * @CAIROU_ICON_X_LEFT: aligns the icon to the right of the X position.
 * @CAIROU_ICON_X_CENTER: aligns the icon in the middle of the X position.
 * @CAIROU_ICON_X_RIGHT: aligns the icon to the left of the X position.
 * @CAIROU_ICON_Y_TOP: aligns the icon below the Y position.
 * @CAIROU_ICON_Y_CENTER: aligns the icon in the middle of the Y position.
 * @CAIROU_ICON_Y_BOTTOM: aligns the icon above the Y position.
 *
 * #cairou_icon_flags_t behaves identically to #cairou_text_flags_t, although
 * there is only a subset of available enumerations. Though the icon implementation
 * mary vary in the future (and currently only uses Glyphicons), there is no reason
 * to include BOLD, ITALIC, or BASELINE flags (among others).
 **/
typedef enum _cairou_icon_flags_t {
	CAIROU_ICON_X_LEFT = 1 << 2,
	CAIROU_ICON_X_CENTER = 1 << 3,
	CAIROU_ICON_X_RIGHT = 1 << 4,
	CAIROU_ICON_Y_TOP = 1 << 6,
	CAIROU_ICON_Y_CENTER = 1 << 7,
	CAIROU_ICON_Y_BOTTOM = 1 << 8
} cairou_icon_flags_t;

/**
 * cairou_show_icon:
 * @cr: a Cairo context
 * @icon: the icon enumeration
 *
 * This function is part of an API wrapping the incredibly useful Glyphicons
 * font, which can be found here:
 *
 * http://glyphicons.com
 *
 * It provides an easy way to dray a glyph in this font using an enumeration
 * corresponding to the particular desired icon. It returns FALSE if the
 * Glyphicons font is not found by Cairo, as this is NOT a totally free font.
 */
cairo_bool_t cairou_show_icon(
	cairo_t* cr,
	cairou_icon_t icon,
	double size,
	double x,
	double y,
	unsigned int flags
);

/**
 * cairou_icon_path:
 * @cr: a Cairo context
 * @icon: the icon enumeration
 *
 * This function behaves like cairou_show_icon(), except that the path is
 * not automatically filled.
 */
cairo_bool_t cairou_icon_path(
	cairo_t* cr,
	cairou_icon_t icon,
	double size,
	double x,
	double y,
	unsigned int flags
);

/**
 * cairou_icon_extents:
 * @cr: a Cairo context
 * @icon: the icon enumeration
 * @size: the icon size, in user coordinates
 * @x: the X position of the icon
 * @y: the Y position of the icon
 * @flags: a bitfield of cairou_icon_flags_t options
 * @extents: an array of doubles representing the final drawn extents rectangle
 *
 * This function returns the extents (similar to rect_extents in the text API)
 * of the rectangular region affected by this icon and the associated flags.
 */
cairo_bool_t cairou_icon_extents(
	cairo_t* cr,
	cairou_icon_t icon,
	double size,
	double x,
	double y,
	unsigned int flags,
	double* extents
);

/**
 * cairou_icon_from_string:
 * @icon: a string matching a known Glyphicons glyph name
 *
 * Finds the nearest match to the passed-in string @icon. The matching
 * heuristic used is trivially simple: all known enumerations are
 * converted to lowercase, and the first substring match that occurs
 * is used. On failure, CAIROU_ICON_ERROR is returned.
 */
cairou_icon_t cairou_icon_from_string(const char* icon);

/**
 * cairou_rounded_rectangle:
 * @cr: a Cairo context
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
cairo_bool_t cairou_rounded_rectangle(
	cairo_t* cr,
	double x,
	double y,
	double width,
	double height,
	double radius,
	const cairo_bool_t* corners
);

/**
 * cairou_rounded_rectangle_apply:
 * @cr: a Cairo context
 * @x: the x coordinate
 * @y: the y coordinate
 * @width: the width value
 * @height: the height value
 * @radius: the radius of each arc
 * @corners: a 4-value cairo_bool_t array, or NULL
 *
 * This function, whose signature is identical to cairou_rounded_rectangle,
 * applies the resultant rounded rectangle and "punches out" the edges of the
 * context passed in as @cr. This can be thougt of as applying a "border" to an
 * existing context/surface, almost like a traditional picture.
 **/
cairo_bool_t cairou_rounded_rectangle_apply(
	cairo_t* cr,
	double x,
	double y,
	double width,
	double height,
	double radius,
	const cairo_bool_t* corners
);

/**
 * cairou_rounded_rectangle_center:
 * @cr: a Cairo context
 * @x: the center x coordinate
 * @y: the center y coordinate
 * @width: the width value, equal to the left and right of the @x position
 * @height: the height value, equal above and below the @y position
 * @radius: the radius of each arc
 * @corners: a 4-value cairo_bool_t array, or NULL
 *
 * This function creates a common rounded-rectangle shape similar to the normal
 * @cairou_rounded_rectangle function above, except that the @x and @y coordinates
 * specify the center of the shape and the @width and @height represent the equidistant
 * lenghts from the center. The remaining parameters behave as in @cairo_rounded_rectangle.
 **/
cairo_bool_t cairou_rounded_rectangle_center(
	cairo_t* cr,
	double x,
	double y,
	double width,
	double height,
	double radius,
	const cairo_bool_t* corners
);

/**
 * cairou_rounded_rectangle_center_apply:
 * @cr: a Cairo context
 * @x: the center x coordinate
 * @y: the center y coordinate
 * @width: the width value, equal to the left and right of the @x position
 * @height: the height value, equal above and below the @y position
 * @radius: the radius of each arc
 * @corners: a 4-value cairo_bool_t array, or NULL
 *
 * Behaves exactly as @cairou_rounded_rectangle_apply, with sizing and positioning
 * parameters behaving as they do in @cairou_rounded_rectangle_center.
 **/
cairo_bool_t cairou_rounded_rectangle_center_apply(
	cairo_t* cr,
	double x,
	double y,
	double width,
	double height,
	double radius,
	const cairo_bool_t* corners
);

#ifdef __cplusplus
}
#endif

#endif
