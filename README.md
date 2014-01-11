cairocks
========

A small library of useful, common Cairo routines.

Overview
========

UPDATE! As of May 25th, 2013, cairocks now supports creating Cairo surfaces
from JPEG and GIF (animated or otherwise) files, using libjpeg and giflib!
However, most distros are still shipping with giflib 4.1, and you'll need to
use 5.0 for cairocks.

Cairocks (Cairo Rocks!) was born from a common codebase I continually ended up
copying from project to project. Instead of trying to keep all these different
versions in sync, I've decided to divorce it out into its own project and
maintain it thusly. It is written in C90-compatible C and is designed NOT to be
built as a separate library but easily included wholesale into your own
projects (and I encourage you to do this via svn:externals if you're using
Subversion as your SCM). Additionally, I have included a robust Python
extension module demonstrating cairocks' usage.

You can get the best idea of the kind of things it can currently do by either
viewing the cairocks.h header or by checking out this screenshot.

Enjoy! AND PLEASE, contribute your own little tidbits of useful stuff--I know
it's out there!

A special thanks to the following people

* Behdad Esfahbod
* Mirco Mueller
* John Schlag, "Graphics Gems IV"
* ranma42, pippin, joonas (Andrea Canciani, Oyvind Kolas, M. Joonas Pilhaja) in IRC; so much fantastic help! Great community! 

Cairocks is simply a collection of useful C, C++, and Python code for use in
your own libraries involving Cairo. It provides a build system using CMake, but
you are welcome to simply copy the files you are interested in into your own
project and compile the files statically if it pleases you. :)

Some of the files (particularly for GIF and JPEG support) require the
defines CAIROCKS_JPEG_SUPPORT and CAIROCKS_GIF_SUPPORT. If not using
CMake to build, be sure and define these if you want to build those
components of cairocks.

