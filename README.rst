cairocks
========

A small library of useful, common Cairo routines.

Overview
========

.. _named-path: https://github.com/cubicool/cairocks/blob/master/src/named-path.cpp
.. _text: https://github.com/cubicool/cairocks/blob/master/src/text.cpp

Cairocks (Cairo Rocks!) was born from a common codebase I continually ended up
copying from project to project. Instead of trying to keep all these different
versions in sync, I've decided to divorce it out into its own project and
maintain it thusly. It is written using a mix of C90-compatible C and C++03,
although it strictly maintains a C-linkable API for maximum accessibility. C++
is usually only used (see `text`_ and `named-path`_) to take advantage of the
STL.

While Cairocks does include an example build system using CMake, you are
welcome to include the source wholesale into your own projects. Additionally,
I have provided an example Python extension module demonstrating cairocks' usage
when paired with the fantastic cairocffi project.

Enjoy! AND PLEASE, contribute your own little tidbits of useful stuff!

A special thanks to the following people

* Behdad Esfahbod
* Mirco Mueller
* John Schlag, "Graphics Gems IV"
* ranma42, pippin, joonas (Andrea Canciani, Oyvind Kolas, M. Joonas Pilhaja) in IRC;
  so much fantastic help from a great community

License
=======

There is no strict license accompanying Cairocks, and the code I have borrowed
from others (that is, that code I did not write myself) was all released with
no license of any kind.  However, I encourage you to do unto others...

Features
========

.. _Glyphicons: http://glyphicons.com

* Generate "signed distance field" surfaces (as popularized by Valve).
* Emboss, gaussian blur, and surface inversion filters.
* Support for loading image surfaces from GIF files (with access to each individual
  GIF frame).
* Support for loading image surfaces from JPEG images.
* An API wrapping and simplifying using the `Glyphicons`_ font, with support
  for interchangable icon backends in the future.
* Drastically simplified text functions wrapping the Cairo "toy" API with support
  for multiple lines and advanced XY positioning (based on the extents of the entire
  body of text).
* Routines for drawing common shapes like grids, rounded-rectangles, etc.
* Support for splines (by creating large groups of 2D points as control structures).
* Map paths onto other paths; for example, rendering text along arcs, etc.
* Introduces the concept of context-specific, persistent "named paths", which can be
  used to push and pop saved (named) paths to and from the associated context.

TODO
====

* Add harfbuzz layout support.
* Finish the grid API.
