#ifndef CAIROU_H
#define CAIROU_H

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

/* When the project named changed from "Cairocks" to "Cairou", I debated on whether or not to also
 * introduce a full version reset. However, in order to keep the numeric tags consistent, I've opted
 * to simply do a MINOR bump, and proceed with business as usual. */
#define CAIROU_VERSION_MAJOR 1
#define CAIROU_VERSION_MINOR 1
#define CAIROU_VERSION_BUGFIX 0

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _cairou_status_t {
	CAIROU_STATUS_SUCCESS = 0,
	CAIROU_STATUS_FOO
} cairou_status_t;

cairo_public
cairou_status_t cairou_status(cairo_t* cr);

/**
 * cairou_map_path_onto:
 * @cr: a Cairo context
 * @path: a cairo path onto which the CURRENT cr path will be mapped
 *
 * This function (taken from Behdad Esfahbod's cairotwisted example) will attempt to map the current
 * path on @cr to the path provided by @path. In the common case you can create an arc or curve,
 * quickly issue a copy_path/copy_path_flat, create a NEW path on @cr and then pass the previously
 * saved path into @path.
 **/
cairo_public
void cairou_map_path_onto(cairo_t* cr, cairo_path_t* path);

/**
 * cairou_create_emboss_create:
 * @surface: a CAIRO_FORMAT_A8 surface
 * @azimuth: the azimuth value, in radians
 * @elevation: the light elevation value, in radians
 * @height: ???
 * @ambient: the ambient light component
 * @diffuse: the diffuse light componnet
 *
 * This function (taken mostly from "Graphics Gems IV") will return a newly-created Cairo image
 * surface created by applying the emboss algorightm on @surface. This new surface can be used to
 * create a new pattern, or used directly with set_source_surface.  This function was further
 * adapted by Andrea Canciani to use a Sobel operator to perform the matrix convolution.
 **/
cairo_public
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
 * This function is identical to the similarly named function @cairou_emboss_surface_create, except
 * that instead of returning a new surface it will do an IN-PLACE emboss of the passed in @surface.
 **/
cairo_public
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
 * Returns a newly created Cairo image surface with the gaussian_blur algorigthm applied.  See the
 * documentation for @cairou_gaussian_blur.
 **/
cairo_public
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
 * Performs an IN-PLACE gaussian blur on @surface. This function was taken from an example provided
 * by MacSlow (Mirco Mueller), though the original unmodified code has been lost in the ether.
 **/
cairo_public
cairo_bool_t cairou_gaussian_blur(
	cairo_surface_t* surface,
	double radius,
	double deviation
);

/**
 * cairou_a8_invert:
 * @surface: a CAIRO_FORMAT_A8 surface to be inverted
 *
 * Performs an IN-PLACE invert on @surface. This function can be handy when you need to invert an
 * alpha mask that is easy to stroke in one way, but hard to stroke another (for example, with font
 * glyphs).
 *
 * TODO: Why did I ever need this?
 **/
cairo_public
cairo_bool_t cairou_a8_invert(cairo_surface_t* surface);

/**
 * cairou_distance_field_create:
 * @surface: a CAIRO_FORMAT_A8 surface
 * @scan_size: size of the "effect" region
 * @block_size: the surface divisor
 *
 * This routine will create an unsigned distance field surface in a CAIRO_FORMAT_A8 surface. These
 * surfaces can be used in various calculations, particularly when combined with an appropriate GLSL
 * shader. For the best results, it is recommended that you rasterize on a large surface type (like
 * 4096x4096) and use a block_size large enough to reduce it to something sensible like 128x128. The
 * scan_size argument will determine how much variation you have in your aliased edges.
 **/
cairo_public
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
cairo_public
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
cairo_public
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
cairo_public
cairo_surface_t* cairou_surface_from_png_data(unsigned char* data);

/**
 * cairou_gif_surface_create:
 * @file: path to a GIF file on disk
 *
 * This function creates a new image surface with special "user data" attached that
 * represents the internal state of the GIF library. Each new frame is loaded "on
 * demand" with @cairo_gif_surface_next.
 **/
cairo_public
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
cairo_public
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
cairo_public
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
cairo_public
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
cairo_public
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
 **/
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
 **/
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
 **/
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
 **/
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
 **/
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
 **/
cairou_icon_t cairou_icon_from_string(const char* icon);

/**
 * cairou_rectangle:
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
cairo_bool_t cairou_rectangle(
	cairo_t* cr,
	double x,
	double y,
	double width,
	double height,
	double radius,
	const cairo_bool_t* corners
);

#if 0
/**
 * cairou_object_t:
 * @CAIROU_OBJECT_TRIANGLE: draws a triangle, using a single size value for the radius. The object
 * is drawn in the traditional "pointing up" position.
 * @CAIROU_OBJECT_RECTANGLE: draws a rectangle using the specified width and height values.
 * @CAIROU_OBJECT_ROUNDED_RECTANGLE: draws a rounded rectangle using the specified width, height and
 * radius values. An optional array of 4 cairo_bool_t values can be specified indicating whether or
 * not a corner should (or should not) be rounded, corresponding to: top-left, top-right,
 * bottom-right, and bottom-left corners. By default all four corners are rounded.
 * @CAIROU_OBJECT_SQUARE: draws a square, using a single size value for both width and height.
 * @CAIROU_OBJECT_CIRCLE: draws a complete circular arc, using a single size value for the radius.
 *
 * The #cairou_object_t enum is used to amalgamate a group of drawing options
 * into a single operation. The functions that accept a #cairou_object_t also
 * accept a va_list of variable arguments, the number and type of which will depend
 * on the current object enumeration.
 *
 * All drawing operations occur using an origin of 0, 0; therefore, users must
 * manually translate before drawing (and this is almost always what you want anyways).
 **/
typedef enum _cairou_object_t {
	CAIROU_OBJECT_TRIANGLE,
	CAIROU_OBJECT_RECTANGLE,
	CAIROU_OBJECT_ROUNDED_RECTANGLE,
	CAIROU_OBJECT_SQUARE,
	CAIROU_OBJECT_PENTAGON,
	CAIROU_OBJECT_HEXAGON,
	CAIROU_OBJECT_OCTOGON,
	CAIROU_OBJECT_CIRCLE
} cairou_object_t;

cairo_bool_t cairou_show_object(cairou_object_t object, ...);
cairo_bool_t cairou_object_path(cairou_object_t object, ...);

/* cairo_bool_t cairou_object_extents(cairou_object_t object, double* extents, ...); */

/* TODO: Capture all "gettable" Cairo values and store them in this structure.
typedef struct _cairou_state_t {
	cairo_matrix_t font_matrix;
	cairo_font_options_t* font_options;
	cairo_font_face_t* font_face;
	cairo_scaled_font_t* scaled_font;
	cairo_operator_t op;
	cairo_pattern_t* source;
	double tolerance;
	cairo_antialias_t antialias;
	cairo_fill_rule_t fill_fule;
	double line_width;
	cairo_line_cap_t line_cap;
	cairo_line_join_t line_join;
	double miter_limit;
	int dash_count;
	double* dashes;
	double* offset;
	cairo_matrix_t matrix;
} cairou_state_t; */

/* cairo_save();
	cairo_translate();
	cairo_stuff();
cairo_restore();

cairou_save(CAIROU_STATE_TRANSLATE, 0.0, 0.0, 0.0);
cairou_restore();

typedef enum _cairou_state_t {
	CAIROU_STATE_TRANSLATE,
	CAIROU_STATE_ROTATE,
	CAIROU_STATE_SCALE,
	CAIROU_STATE_
} cairou_state_t; */

/* cairo_bool_t cairou_save(...);
 * cairo_tool_t cairou_restore(); */
#endif

/**
 * cairou_grid_t:
 * A #cairou_grid_t structure is an arrangement of squares (coordinates), each of uniform size, most
 * commonly created to either fill or fit within an existing, rectangular region. They can be
 * tightly packed (when padding is NOT requested) or sparse (when padding is requested), and can be
 * dynamically reinitialized at any point without concern.
 *
 * Since #cairou_grid_t structures are dynamically allocated, a thorough API is provided for
 * interacting with them for most of the common operations.
 **/
typedef struct _cairou_grid_t cairou_grid_t;

#if 0
 * Furthermore, the width and height values represet each dimension relative to the origin. Thus,
 * using the previous example and cell size of 5.0 (with no padding), the width of the grid becomes
 * 50.0 (10 cell_columns * 5.0 cell_size) and the height becomes 25.0 (5 cell_rows * 5.0 cell_size).
 * The ACTUAL extens of the grid, however, are twice this when accounting for the central origin.
#endif

/**
 * cairou_grid_option_t:
 * @CAIROU_GRID_OPTION_PADDING: the amount of uniform padding to use between coordinate cells,
 * defined as a single double. The default padding value is 0.
 * @CAIROU_GRID_OPTION_ORIGIN: the X/Y grid cell coordinate to be used as the origin, defined as two
 * integers. The default origin is (0, 0). The grid's cell coordinates are always referred to as
 * relative to the origin. For example, if a #cairou_grid_t is created using 10 rows and 20 columns,
 * the top-left coordinate becomes (-10, -5), the top-right (10, -5), etc.
 * @CAIROU_GRID_OPTION_MATRIX: the matrix instance to use when drawing, defined as a pointer to an
 * existing #cairou_matrix_t. The passed-in matrix data WILL be copied, making it safe to delete the
 * original matrix if necessary.
 *
 * The #cairou_grid_option_t enumeration defines a number of sentinel flags that are used during
 * #cairou_grid_t creation (implemented as the @options parameter of those functions). Each value
 * expects 1 or more subsequent values to follow, and an @options list is always terminated with
 * NULL.
 *
 * Note: while there are currently only a small number #cairou_grid_option_t enumerations, there
 * will likely be more in the future; thus, leading to this more future-proof--but slightly
 * wonky--API decision.
 **/
typedef enum _cairou_grid_option_t {
	CAIROU_GRID_OPTION_PADDING = 1,
	CAIROU_GRID_OPTION_ORIGIN,
	CAIROU_GRID_OPTION_MATRIX
} cairou_grid_option_t;

/**
 * cairou_grid_create:
 * @rows: the number of horizontal (left-to-right) cells
 * @cols: the number of vertical (top-to-bottom) cells
 * @size: the uniform size of each grid cell
 * @options: a variable, NULL-terminated list of #cairou_grid_option_t enums and their appropriate
 * parameters; see the #cairou_grid_option_t documention for me help
 *
 * This is the basic #cairou_grid_t creation function. It requires the user to know both the
 * dimensions and size of the coordinate cells beforehand.
 **/
cairou_grid_t* cairou_grid_create(
	int rows,
	int cols,
	double size,
	cairou_grid_option_t options,
	...
);

/**
 * cairou_grid_create_for_rows_cols:
 * @rows: the number of horizontal cells
 * @cols: the number of vertical cells
 * @width: the horizontal size constraint to be "filled" during creation
 * @height: the vertical size constraint to be "filled" during creation
 * @options: a variable, NULL-terminated list of #cairou_grid_option_t enums and their appropriate
 * parameters; see the #cairou_grid_option_t documention for me help
 **/
cairou_grid_t* cairou_grid_create_for_rows_cols(
	int rows,
	int cols,
	double width,
	double height,
	cairou_grid_option_t options,
	...
);

/**
 * cairou_grid_create_for_size:
 * @size: the uniform size of each grid cell
 * @width: the horizontal size constraint to be "filled" during creation
 * @height: the vertical size constraint to be "filled" during creation
 * @options: a variable, NULL-terminated list of #cairou_grid_option_t enums and their appropriate
 * parameters; see the #cairou_grid_option_t documention for me help
 **/
cairou_grid_t* cairou_grid_create_for_size(
	double size,
	double width,
	double height,
	cairou_grid_option_t options,
	...
);

/**
 * cairou_grid_destroy:
 * @grid: a valid #cairou_grid_t pointer
 *
 * Deletes the memory allocated for the passed-in #cairou_grid_t instance.
 **/
void cairou_grid_destroy(cairou_grid_t* grid);

/**
 * cairou_grid_get_rows:
 * @grid: a valid #cairou_grid_t pointer
 *
 * Returns the number of vertical rows for the passed-in #cairou_grid_t instance.
 **/
int cairou_grid_get_rows(cairou_grid_t* grid);

/**
 * cairou_grid_get_cols:
 * @grid: a valid #cairou_grid_t pointer
 *
 * Returns the number of horizontal columns for the passed-in #cairou_grid_t instance.
 **/
int cairou_grid_get_cols(cairou_grid_t* grid);

/**
 * cairou_grid_get_size:
 * @grid: a valid #cairou_grid_t pointer
 *
 * Returns the uniform size of each cell for the passed-in #cairou_grid_t instance.
 **/
double cairou_grid_get_size(cairou_grid_t* grid);

/**
 * cairou_grid_get_options:
 * @grid: a valid #cairou_grid_t pointer
 * @options:
 *
 * Returns the number of rows for the passed-in #cairou_grid_t instance.
 **/
void cairou_grid_get_options(cairou_grid_t* grid, cairou_grid_option_t options, ...);

/**
 * cairou_grid_set_options:
 * @grid: a valid #cairou_grid_t pointer
 *
 * Returns the number of rows for the passed-in #cairou_grid_t instance.
 **/
void cairou_grid_set_options(cairou_grid_t* grid, cairou_grid_option_t options, ...);

/*
 * cairou_grid_extents(cairou_grid_t* grid, double* extents);
 * cairou_grid_cell_extents(cairou_grid_t* grid, int row, int col, double* extents);
 * cairou_grid_intersect(cairou_grid_t* grid, double x, double y);
 * cairou_show_grid
 * cairou_grid_path
 **/

/* These math helper routines don't have the cairou_* prefix. */
double slope(double x0, double y0, double x1, double y1);
double y_intercept(double x0, double y0, double x1, double y1);
double y_intercept_m(double x, double y, double m);
double slope_intercept(double x, double y, double m, double b);

#ifdef __cplusplus
}
#endif

#endif
