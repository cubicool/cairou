# This module defines:
# CAIRO_LIBRARY_DIRS
# CAIRO_INCLUDE_DIRS

FIND_PATH(CAIRO_INCLUDE_DIRS cairo.h
	$ENV{CAIRODIR}/include/cairo
	$ENV{CAIRODIR}/include
	$ENV{CAIRODIR}
	/usr/include/cairo
	/usr/local/include/cairo
	/opt/include
)

IF(WIN32)
	SET(LIBCAIRO "cairo.lib")
ELSE()
	SET(LIBCAIRO "libcairo.so")
ENDIF()

FIND_PATH(CAIRO_LIBRARY_DIRS ${LIBCAIRO}
	$ENV{CAIRODIR}/lib
	$ENV{CAIRODIR}/lib64
	$ENV{CAIRODIR}
	/usr/local/lib
	/usr/local/lib64
	/usr/lib
	/usr/lib64
	/opt/lib
)

SET(CAIRO_FOUND "NO")

IF(CAIRO_LIBRARY_DIRS AND CAIRO_INCLUDE_DIRS)
	SET(CAIRO_FOUND "YES")
ENDIF()

MARK_AS_ADVANCED(CAIRO_LIBRARY_DIRS CAIRO_INCLUDE_DIRS)

