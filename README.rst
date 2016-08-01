#######
NOTICE!
#######

**This project/library was formerly known as Cairocks!** If you are
looking for it (*for example, if your submodule just ... broke, somehow*) then
look no further; this is it!

######
Cairou
######

A small library of useful, common Cairo routines. It aims to be for Cairo what
libGLU was for traditional OpenGL.

========
Overview
========

.. _named-path: https://github.com/cubicool/cairou/blob/master/src/named-path.cpp
.. _text: https://github.com/cubicool/cairou/blob/master/src/text.cpp

Cairou (Cairo Utility) was born from a common codebase I continually ended up
copying from project to project. Instead of trying to keep these different
versions in sync, I've decided to divorce them out into their own project and
maintain them thusly. Cairou is written using a mix of C90-compatible C and
C++03, and strictly maintains a C-linkable external API for maximum
accessibility. C++ is usually only used (see `text`_ and `named-path`_) to take
advantage of the STL or interact with existing code that cannot be easily
converted.

While Cairou does include an example build system using CMake, you are welcome
to include the source wholesale into your own projects. Additionally, I have
provided an example Python extension module demonstrating Cairou's usage when
paired with the fantastic cairocffi project.

Enjoy! AND PLEASE, contribute your own little tidbits of useful stuff!

==============
Special Thanks
==============

* Behdad Esfahbod
* Mirco Mueller
* John Schlag, "Graphics Gems IV"
* Angus Johnson
* ranma42, pippin, joonas (Andrea Canciani, Oyvind Kolas, M. Joonas Pilhaja) in
  IRC; so much fantastic help from a great community!

=======
License
=======

.. _Glyphicons: http://glyphicons.com
.. _more information here: http://glyphicons.com/license
.. _Clipper: http://www.angusj.com/delphi/clipper.php
.. _Angus Johnson: http://www.angusj.com/

There is no single license accompanying Cairou. The code I have borrowed from
others (that is, that code I did not write myself) was also released with no
license of any kind. You are therefore free to do whatever you wish with this
code, with the following exceptions:

* Glyphicons

   The `Glyphicons`_ font comes in multiple varieties, free and otherwise. The
   license is generally very accomodating, and you can find
   `more information here`_ about the Pro version.

* Clipper

   The `Clipper`_ library was created by `Angus Johnson`_, and comes with a very
   permissable license.

========
Features
========

*************
Version 1.0.0
*************

.. _distance field: http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
.. _GIFLIB: https://sourceforge.net/projects/giflib/
.. _libjpeg-turbo: https://github.com/libjpeg-turbo/libjpeg-turbo
.. _toy: http://cairographics.org/manual/cairo-text.html

* Generate "`distance field`_" surfaces, as popularized by Valve.

* Emboss, gaussian blur, and surface inversion filters.

* Support for loading image surfaces from GIF files (using `GIFLIB`_ 5.0 or
  higher), with access to each individual GIF frame.

* Support for loading image surfaces from JPEG images using `libjpeg-turbo`_.

* A useful wrapper for loading PNG images from memory.

* An API wrapping and simplifying using the `Glyphicons`_ font, with support
  for interchangable icon backends in the future.

* Drastically simplified text functions wrapping the Cairo "`toy`_" text API
  with support for multiple lines and advanced XY positioning based on the
  extents of the entire body of text.

* Spline support, created with large groups of 2D points as control structures.

* Map paths onto other paths; for example, rendering text along arcs, etc.

* Introduces the concept of context-specific, persistent "named paths", which
  can be used to push and pop saved paths to and from the associated
  context.

====
TODO
====

*************
1.1.0 Release
*************

#. Move routines out of map-path-onto and into cairou-math.c.

#. Begin using the new cairo_rectangle_int_t, where appropriate.

#. Remove the instances of cairo_bool_t being returned (where it doesn't make
   any sense to do so).

#. Make documention punctuation consistent throughout the library.

#. Make private naming conventions in the implementation source files consistent
   throughout the library; e.g, with the "_private" components always appearing
   at the END of the function/structure/etc. name.

#. Consistently either check (or DO NOT check) for pointer validity throughout
   the API.

#. Use cairo_public for all exported cairou_* routines.

#. Determine the minimum official CAIRO_VERSION required for the Cairou API.

#. Remove the public cairou_spline_t, replacing it with an array of doubles. The
   structure may still be necessary internally.

#. (DONE) Rename each source file so that is begins with "cairou-".

#. Remove "named path" routines; they suck(ed).

#. Remove the ridiculous usage of printf() everywhere throughout the library;
   instead, set an error state on the Context/Surface and use cairou_status().

*************
1.2.0 Release
*************

#. Wrap as many features as possible using some manner of conditional
   inclusion/evaluation, and improve the example CMake build system to
   demonstrate this functionality. Most features (especially the JPEG, GIF,
   Clipper, and Glyphicons components) should be optional. This may simply
   require Cairou to adandon its "only a single header file" constraint, and
   move the toggleable features into their own includes.

#. Add support for the Clipper library, and break its functionality into
   different (but similar) APIs based on the methods provided by both the
   Clipper object and the ClipperOffset object.

#. Consider supporting C++11 by default and, possibly, a newer specification of
   C. At the minimum, Cairou should use the spec of the newest, non-optional
   include dependency.

*******************
Future Enhancements
*******************

.. _Harfbuzz: http://www.harfbuzz.org
.. _CairoGL/SDL2: https://github.com/cubicool/cairo-gl-sdl2
.. _Glyphy: https://www.glyphy.org
.. _Clutter: https://blogs.gnome.org/clutter
.. _Graphene: http://ebassi.github.io/graphene

#. **BETTER TEST COVERAGE!**

#. Drastically improved documentation and web presence. Cairou is actually
   pretty great, but no one knows about it!

#. Optional `Harfbuzz`_ layout support.

#. Optional wrappers for simplifying `CairoGL/SDL2`_ usage, potentially along
   with `Glyphy`_.

#. Spline constraints; i.e., point x0 must stay 5 units perpendicular to point
   x1 at all times.

#. Add a pixel-aligned drawing API.

#. Add cairou_object_path/cairou_show_object.

#. Implement a cairou_state_t structure, as well as cairou_{save/restore}
   functions that are capable of optionally pushing/popping these states.
   Additionally, define a special syntax for quick and easy structure
   creation/allocation. For example:

   .. code:: cpp

      /* Allocate a new cairou_state_t object on the heap. */
      cairou_state_t* state = cairou_state_create(
         CAIROU_TRANSLATE, 1.0, 1.0,
         CAIROU_SCALE, 1.0, 0.0,
         CAIROU_LINE_WIDTH, 2.0
      );

      cairou_state_destroy(state);

      /* Alternatively, manage your cairou_state_t object locally and simply
      initialize it using the meta-syntax. */
      cairou_state_t state;

      cairou_state_init(
         &state,
         CAIROU_TRANSLATE, 1.0, 1.0,
         CAIROU_SCALE, 1.0, 0.0,
         CAIROU_LINE_WIDTH, 2.0
      );

      /* METHOD 1: Push/pop the custom state using the most verbose method. */
      cairo_save(); {
         cairou_push_state(state);

         /* ...draw... */

         cairou_pop_state();
      } cairo_restore();

      /* METHOD 2: Pass an existing state object to save/restore. */
      cairou_save(state); {
          /* ...draw... */
      } cairou_restore();

      /* METHOD 3: Allow the save/restore functions to manage their state
      themselves, without having to allocate memory on the heap. */
      cairou_save(
         CAIROU_ROTATE, M_PI,
         CAIROU_FG_RGBA, 1.0f, 1.0f, 0.5f, 1.0f,
         CAIROU_BG_RGB, 0.0f, 0.0f, 0.0f
      ); {
          /* ...draw... */
      } cairou_restore();

   .. note::

      It might make sense to support different kinds of dynamic state creation
      markups. A va_args-based version would certainly be the default, but we
      could also support CSS, JSON, etc.

#. Develop a framework (and possibly adjust the named_path implmentation) that
   allows easy, consistent storing of Cairo/Cairou/etc. data as cairo_t
   "user_data."

#. Develop a complimentary framework for creating cairo_t Context objects
   already set with a pre-defined group of user data. This will require the use
   of clever macros that _LOOK_ like typical Cairou functions, possibly needing
   to use the "argument-counting-trick" to give the impression the macros can be
   "overloaded" depending on how many arguments you pass them. For example:

   .. code:: cpp

      foo_t* foo = foo_create();
      bar_t* bar = bar_create();
      static baz_t baz;

      cairou_user_data_create(KEY_FOO, foo, foo_destroy);
      cairou_user_data_create(KEY_BAR, bar, bar_destroy);
      cairou_user_data_create(KEY_BAZ, &baz);

      cairo_t* cr0 = cairou_create(surface, KEY_FOO);
      cairo_t* cr1 = cairou_create(surface, KEY_FOO, KEY_BAR, KEY_BAZ);

      /* ...draw... */

#. Introduce a system for creating an arbitrary number of "rendering objects"
   and having them called based on a timeout--and in some kind of dependent
   order--with the possibility of additional, pre-defined effects, etc.
   Something like:

   .. code:: cpp

      cairo_bool_t do_draw0(cairo_t* cr, cairou_state_t* state);
      cairo_bool_t do_draw1(cairo_t* cr, cairou_state_t* state);
      cairo_bool_t do_draw2(cairo_t* cr, cairou_state_t* state);

      typedef cairo_bool_t (*cairou_draw_cb_t)(cairo_t*, cairou_state_t*);

      typedef struct _cairou_draw_t {
          const char* name;
          cairou_draw_callback_t callback;
          double timeout;
          const char* draw_before;
          const char* draw_after;
      } cairou_draw_t;

      cairou_draw_t* draw0 = cairou_draw_create(do_draw0);
      cairou_draw_t* draw1 = cairou_draw_create(do_draw1);
      cairou_draw_t* draw2 = cairou_draw_create(do_draw2);

      cairou_state_t* state = cairo_state_create(...);

      cairou_draw(NULL, draw0);
      cairou_draw(state, draw1, draw2);

      cairo_state_destroy(state);

      cairou_draw_destroy(draw0);
      cairou_draw_destroy(draw1);
      cairou_draw_destroy(draw2);

   .. warning::

      A system like this might encroach too much on the user (as each developer
      will either have their own ideas about the best way to draw things or may
      be integrating with some existing rendering paradigm), and may be entirely
      worthless to implement. In fact, there are libraries like `Clutter`_ that
      do this quite well already.

#. Create an API for performing drawing operations that are automatically
   "mirrored" along additional axes. One implementation could redraw the current
   path after rotating the canvas N number of times.

#. Figure out some way to iplement variable stroke size. A possible
   implementation would be to create a stroke-worthy path of some sort (a box,
   for example) and calculate new points based on the "typical" stroke that
   would occur. While creating these new points, perturb them by some
   user-defined "weight", so that the old stroke becomes a new, complex fill.

#. Provide "faux 3D" transforms, possibly by using something like `Graphene`_ to
   convert Cairo matrices into 3D matrices, and back.

#. Create an optional *cairou.hpp* header file that implements C++11/14 API
   extensions to the core C-based Cairou routines.

#. Implement a CAIRO_SAVE_RESTORE wrapper; macro in C, lambda in C++11?

#. Wrap/shim the existing Cairo API with (conditional) prefixed calls to
   cairo_status(), invoking an error callback when the status is invalid.

#. Add additional path->point iteration helpers to the Context. For example,
   cairou_foreach_point(cairo_t*, ...), etc.

#. Introduce a group of aspect ratio routines that assist in calculating,
   placement, conversion, and maniuplation.

#. Potentially create a binary-only file format.

#. Add cairou_glyph_path_{rectangle,rect,constrained,...} that works similarly
   to the text API in that it allows you to path/show a single glyph in a
   user-defined rectangular region.

#. Consider adding a cairou_glyph_atlas_t object that creates tightly-packed
   surfaces for use in OpenGL texturing, among other things.

#. Add statically builtin "Lorem ipsum" text that easily be used for rendering.
   Since we only currently support L2R (and maybe R2L easily enough), we only
   need the standard text blob; not a full range of data covering Mandarain,
   Japanese, etc.

#. Keep fleshing out this idea of a "contextless" cairou_object_t, potentially
   deprecating the functionality provided by the current named_path routines. A
   cairou_object_t structure would encompass multiple "layers"
   (cairou_layer_t?), with each layer managing both a single path *AND* some
   manner of "style" (cairou_style_t?) with which it will be drawn. Styles would
   encompass things like line width, dash settings, fill color, stroke color,
   etc.

