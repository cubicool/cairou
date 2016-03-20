#######
NOTICE!
#######

**This project/library was formerly known as Cairocks!** If you are
looking for it (*for example, if your Git remote just ... broke, somehow*) then
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

A special thanks to the following people:

* Behdad Esfahbod
* Mirco Mueller
* John Schlag, "Graphics Gems IV"
* Angus Johnson
* ranma42, pippin, joonas (Andrea Canciani, Oyvind Kolas, M. Joonas Pilhaja) in
  IRC; so much fantastic help from a great community!

=======
License
=======

.. _Clipper: http://www.angusj.com/delphi/clipper.php
.. _Angus Johnson: http://www.angusj.com/

There is no strict license accompanying Cairou, and the code I have borrowed
from others (that is, that code I did not write myself) was all released with no
license of any kind.

The exceptions to this are the following:

* *clipper.cpp*: The `Clipper`_ library was created by `Angus Johnson`_, and
  comes with a very permissable license.

========
Features
========

.. _Glyphicons: http://glyphicons.com

*************
Version 1.0.0
*************

* Generate "unsigned distance field" surfaces, as popularized by Valve.
* Emboss, gaussian blur, and surface inversion filters.
* Support for loading image surfaces from GIF files, with access to each
  individual GIF frame.
* Support for loading image surfaces from JPEG images.
* A useful wrapper for loading PNG images from memory.
* An API wrapping and simplifying using the `Glyphicons`_ font, with support
  for interchangable icon backends in the future.
* Drastically simplified text functions wrapping the Cairo "toy" API with
  support for multiple lines and advanced XY positioning based on the extents
  of the entire body of text.
* Support for splines, created with large groups of 2D points as control
  structures.
* Map paths onto other paths; for example, rendering text along arcs, etc.
* Introduces the concept of context-specific, persistent "named paths", which
  can be used to push and pop saved (named) paths to and from the associated
  context.

*************
Version 1.1.0
*************

* [**IN PROGRESS**] Perform polygon clipping and offsetting of complex, closed
  paths. Derived and improved from the `Clipper`_ library.

====
TODO
====

.. _Harfbuzz: http://www.harbuzz.org
.. _CairoGL/SDL2: https://github.com/cubicool/cairo-gl-sdl2
.. _Glyphy: https://www.glyphy.org
.. _Clutter: https://blogs.gnome.org/clutter
.. _Graphene: http://ebassi.github.io/graphene

* [ ] Add support for the Clipper library, and break its functionality into
  different (but similar) APIs based on the methods provided by both the Clipper
  object and the ClipperOffset object.
* [ ] Wrap as many features as possible using some manner of conditional
  inclusion/evaluation, and improve the example CMake build system to
  demonstrate this functionality. Most features (especially the JPEG, GIF,
  Clipper, and Glyphicons components) should be optional. This may simply
  require Cairou to adandon its "only a single header file" constraint, and move
  the toggleable features into their own includes.
* [ ] Optional `Harfbuzz`_ layout support.
* [ ] Optional wrappers for simplifying `CairoGL/SDL2`_ usage, potentially along
  with `Glyphy`_.
* [ ] Spline constraints; i.e., point x0 must stay 5 units perpendicular to point x1
  at all times.
* Add a pixel-aligned drawing API.
* Add cairou_object_path/cairou_show_object/cairo_object_apply.
* Implement a cairou_state_t structure, as well as cairou_{save/restore}
  functions that are capable of optionally pushing/popping these states.
  Additionally, define a special syntax for quick and easy structure
  creation/allocation. For example:

.. code:: c

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

   cairou_state_init(&state,
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
   markups. A va_args-based version would certainly be the default, but we could
   also support CSS, JSON, etc.

* Develop a framework (and possibly adjust the named_path implmentation) that
  allows easy, consistent storing of Cairo/Cairou/etc. data as cairo_t
  "user_data."
* Develop a complimentary framework for creating cairo_t Context objects already
  set with a pre-defined group of user data. This will require the use of clever
  macros that _LOOK_ like typical Cairou functions, possibly needing to use the
  "argument-counting-trick" to give the impression the macros can be
  "overloaded" depending on how many arguments you pass them. For example:

.. code:: c

   foo_t* foo = foo_create();
   bar_t* bar = bar_create();
   static baz_t baz;

   cairou_user_data_create(KEY_FOO, foo, foo_destroy);
   cairou_user_data_create(KEY_BAR, bar, bar_destroy);
   cairou_user_data_create(KEY_BAZ, &baz);

   cairo_t* cr0 = cairou_create(surface, KEY_FOO);
   cairo_t* cr1 = cairou_create(surface, KEY_FOO, KEY_BAR, KEY_BAZ);

   /* ...draw... */

* Introduce a system for creating an arbitrary number of "rendering objects" and
  having them called based on a timeout--and in some kind of dependent
  order--with the possibility of additional, pre-defined effects, etc. Something
  like:

.. code:: c

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

.. note::

   A system like this might encroach too much on the user (as each developer
   will either have their own ideas about the best way to draw things or may be
   integrating with some existing rendering paradigm), and may be entirely
   worthless to implement. In fact, there are libraries like `Clutter`_ that do
   this quite well already.

* Investigate "_apply" API; see what happens when I set the source automatically
  with different alpha values.
* Create an API for performing drawing operations that are automatically
  "mirrored" along additional axes. One implementation could redraw the current
  path after rotating the canvas N number of times.
* Figure out some way to iplement variable stroke size. A possible
  implementation would be to create a stroke-worthy path of some sort (a box, for
  example) and calculate new points based on the "typical" stroke that would
  occur. While creating these new points, perturb them by some user-defined
  "weight", so that the old stroke becomes a new, complex fill.
* Provide "faux 3D" transforms, possibly by using something like `Graphene`_ to
  convert Cairo matrices into 3D matrices, and back.
* Create an optional *cairou.hpp* header file that implements C++11/14 API
  extensions to the core C-based Cairou routines.

[ ] Test foo!
[ ] Test bar?

* [ ] Test baz!?
* [ ] Test qux...
