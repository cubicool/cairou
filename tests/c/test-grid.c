#include "common.h"

const char* test_name = "grid";

void print_grid(cairou_grid_t* grid, const char* message) {
	double padding;
	int origin[2];
	cairo_matrix_t matrix;

	if(!grid) {
		printf("grid (NULL) %s\n", message);

		return;
	}

	cairou_grid_get_options(
		grid,
		CAIROU_GRID_OPTION_PADDING, &padding,
		CAIROU_GRID_OPTION_ORIGIN, &origin[0], &origin[1],
		CAIROU_GRID_OPTION_MATRIX, &matrix
	);

	printf(
		"%20s: [%2dx%2d@%4.1f] "
		"padding=(%4.1f) "
		"origin=(%2d, %2d) "
		"matrix=(%4.1f, %4.1f, %4.1f, %4.1f, %4.1f, %4.1f)\n",
		message,
		cairou_grid_get_rows(grid),
		cairou_grid_get_cols(grid),
		cairou_grid_get_size(grid),
		padding,
		origin[0],
		origin[1],
		matrix.xx, matrix.yx,
		matrix.xy, matrix.yy,
		matrix.x0, matrix.y0
	);
}

#define TEST_GRID_BEGIN(func) { \
	const char* m = #func; \
	cairou_grid_t* grid = cairou_grid_##func

#define TEST_GRID_END \
	print_grid(grid, m); \
	cairou_grid_destroy(grid); } \

void test_function(int argc, char** argv, cairo_t* cr) {
	TEST_GRID_BEGIN(create) (10, 10, 10.0, 0); TEST_GRID_END
	TEST_GRID_BEGIN(create_for_rows_cols) (10, 10, 100.0, 100.0, 0); TEST_GRID_END
	TEST_GRID_BEGIN(create_for_size) (10.0, 100.0, 100.0, 0); TEST_GRID_END

	TEST_GRID_BEGIN(create) (
		10,
		10,
		10.0,
		CAIROU_GRID_OPTION_ORIGIN, 5, 5,
		NULL
	); TEST_GRID_END

	TEST_GRID_BEGIN(create) (
		10,
		10,
		10.0,
		CAIROU_GRID_OPTION_ORIGIN, 5, 5,
		CAIROU_GRID_OPTION_PADDING, 5.0,
		NULL
	); TEST_GRID_END
}

