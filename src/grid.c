#include "cairocks.h"

#include <stdlib.h>
#include <string.h>

struct _cairocks_grid_t {
	unsigned int num_cols;
	unsigned int num_rows;
	double cell_width;
	double cell_height;
};

cairocks_grid_t* cairocks_grid_create(
	cairo_t*     cr,
	unsigned int cols,
	unsigned int rows,
	double       cell_width,
	double       cell_height
) {
	cairocks_grid_t* grid = (cairocks_grid_t*)(malloc(sizeof(cairocks_grid_t)));

	memset(grid, 0, sizeof(cairocks_grid_t));

	grid->num_cols = cols;
	grid->num_rows = rows;

	return grid;
}

