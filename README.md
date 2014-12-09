cairocks
========

A small library of useful, common Cairo routines.

Overview
========

Cairocks (Cairo Rocks!) was born from a common codebase I continually ended up
copying from project to project. Instead of trying to keep all these different
versions in sync, I've decided to divorce it out into its own project and
maintain it thusly. It is written using a mix of C90-compatible C and C++,
although it strictly maintains a C-linkable API for maximum accessibility.

While Cairocks does include an example build system using CMake, you are
welcome to include the source wholesale into your own projects. Additionally,
I have provided an example Python extension module demonstrating cairocks' usage
when paired with the fantastic cairocffi project.

Enjoy! AND PLEASE, contribute your own little tidbits of useful stuff!

A special thanks to the following people

* Behdad Esfahbod
* Mirco Mueller
* John Schlag, "Graphics Gems IV"
* ranma42, pippin, joonas (Andrea Canciani, Oyvind Kolas, M. Joonas Pilhaja) in IRC; so much fantastic help! Great community! 

License
=======

There is no strict license accompanying Cairocks, and the code I have borrowed
from others (that is, that code I did not write myself) was all released with
no license of any kind.  However, I encourage you to do unto others...

TODO
====

- Add spline code.
- Finish the text/alignment API.
- Add harfbuzz layout support.
