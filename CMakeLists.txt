PROJECT(cairou)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/etc/")
SET(CAIROU_BUILD_TESTS FALSE CACHE BOOL "Enable building of tests.")
SET(CAIROU_STATIC FALSE CACHE BOOL "Enable static build of Cairou.")

FIND_PACKAGE(Cairo REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(GIF REQUIRED)

SET(CAIROU_SRC_FILES
	# Standard C files.
	"src/distance-field.c"
	"src/emboss.c"
	"src/gaussian-blur.c"
	"src/gif-surface.c"
	"src/glyphicons.c"
	"src/icon.c"
	"src/invert.c"
	"src/map-path-onto.c"
	"src/rounded-rectangle.c"
	"src/spline.c"
	"src/surface-from-jpeg.c"
	"src/surface-from-png.c"

	# C++ files, use external C linkage.
	"src/named-path.cpp"
	"src/text.cpp"
)

SET(CAIROU_HEADER_FILES
	"src/cairou.h"
	"src/cairou-glyphicons.h"
)

INCLUDE_DIRECTORIES(
	src
	${CAIRO_INCLUDE_DIRS}
	${JPEG_INCLUDE_DIR}
	${GIF_INCLUDE_DIR}
)

LINK_DIRECTORIES(${CAIRO_LIBRARY_DIRS})

SOURCE_GROUP("Header Files" FILES ${CAIROU_HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${CAIROU_SRC_FILES})

# Setup the static/shared library variable.
SET(CAIROU_LIBRARY_TYPE SHARED)

IF(CAIROU_STATIC)
	SET(CAIROU_LIBRARY_TYPE STATIC)
ENDIF()

ADD_LIBRARY(cairou ${CAIROU_LIBRARY_TYPE} ${CAIROU_HEADER_FILES} ${CAIROU_SRC_FILES})

TARGET_LINK_LIBRARIES(cairou cairo ${JPEG_LIBRARIES} ${GIF_LIBRARIES})

# Add the C/C++ compiled examples (though Python is generally the preferred
# testing approach.
IF(CAIROU_BUILD_TESTS)
	ADD_SUBDIRECTORY("tests/c")
ENDIF()

INSTALL(TARGETS cairou
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

INSTALL(
	FILES ${CAIROU_HEADER_FILES}
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/etc/uninstall.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
	IMMEDIATE @ONLY
)

ADD_CUSTOM_TARGET(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
)

