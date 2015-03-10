PROJECT(cairocks)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/etc/")
SET(CAIROCKS_BUILD_TESTS FALSE CACHE BOOL "Enable building of tests.")

IF(NOT WIN32)
	SET(CMAKE_C_FLAGS "-W -Wall -Wno-unused-parameter -Werror -ansi -pedantic")
	SET(CMAKE_CXX_FLAGS "-W -Wall -Wno-unused-parameter -Werror")
ENDIF()

FIND_PACKAGE(Cairo REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(GIF REQUIRED)

SET(CAIROCKS_SRC_FILES
	# Standard C files.
	"src/cairocks-glyphicons.c"
	"src/distance-field.c"
	"src/emboss.c"
	"src/gaussian-blur.c"
	"src/gif-surface.c"
	"src/icon.c"
	"src/invert.c"
	"src/map-path-onto.c"
	"src/rounded.c"
	"src/spline.c"
	"src/surface-from-jpeg.c"
	"src/surface-from-png.c"

	# C++ files, use external C linkage.
	"src/named-path.cpp"
	"src/text.cpp"
)

SET(CAIROCKS_HEADER_FILES
	"src/cairocks.h"
	"src/cairocks-glyphicons.h"
)

INCLUDE_DIRECTORIES(
	src
	${CAIRO_INCLUDE_DIRS}
	${JPEG_INCLUDE_DIR}
	${GIF_INCLUDE_DIR}
)

LINK_DIRECTORIES(${CAIRO_LIBRARY_DIRS})

SOURCE_GROUP("Header Files" FILES ${CAIROCKS_HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${CAIROCKS_SRC_FILES})

ADD_LIBRARY(cairocks ${CAIROCKS_HEADER_FILES} ${CAIROCKS_SRC_FILES})

TARGET_LINK_LIBRARIES(cairocks cairo ${JPEG_LIBRARIES} ${GIF_LIBRARIES})

# Add the C/C++ compiled examples (though Python is generally the preferred
# testing approach.
IF(CAIROCKS_BUILD_TESTS)
	ADD_SUBDIRECTORY("tests/c")
ENDIF()

INSTALL(TARGETS cairocks
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

INSTALL(
	FILES ${CAIROCKS_HEADER_FILES}
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

