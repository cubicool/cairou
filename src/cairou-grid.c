#include "cairou.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* We put the members of the same "type" together, in case doing otherwise results in some weird
 * compiler padding or something. */
struct _cairou_grid_t {
	int origin[2];
	int rows;
	int cols;
	double size;
	double width;
	double height;
	double padding;
	cairo_matrix_t matrix;
};

static void cairou_grid_set_options_private(
	cairou_grid_t* grid,
	cairou_grid_option_t option,
	va_list* list
) {
	int active = 1;

	while(active) {
		switch(option) {
		case CAIROU_GRID_OPTION_PADDING:
			grid->padding = va_arg(*list, double);

			break;

		case CAIROU_GRID_OPTION_ORIGIN:
			grid->origin[0] = va_arg(*list, int);
			grid->origin[1] = va_arg(*list, int);

			break;

		case CAIROU_GRID_OPTION_MATRIX:
			memcpy(&grid->matrix, va_arg(*list, cairo_matrix_t*), sizeof(cairo_matrix_t));

			break;

		default:
			active = 0;

			break;
		}

		if(active) option = va_arg(*list, cairou_grid_option_t);
	}
}

/* This macro should be used as the very first line of each cairou_grid_t creation function. It
 * creates the dynamically-allocated "grid" variable, as well as properly handling the "options"
 * va_list used to handle the cairou_grid_options_t-based variadic parameters. */
#define CAIROU_GRID_CREATE_BEGIN \
	cairou_grid_t* grid = NULL; \
	va_list list; \
	va_start(list, options); \
	grid = cairou_grid_create_private(options, &list); \
	va_end(list); { \

#define CAIROU_GRID_CREATE_END } return grid; }

/* To avoid putting TOO MUCH logic in preprocessing, most of the common setup used for each creation
 * function is put here (and used by CAIROU_GRID_CREATE_BEGIN). */
static cairou_grid_t* cairou_grid_create_private(cairou_grid_option_t options, va_list* list) {
	cairou_grid_t* grid = (cairou_grid_t*)(malloc(sizeof(cairou_grid_t)));

	memset(grid, 0, sizeof(cairou_grid_t));

	cairo_matrix_init_identity(&grid->matrix);
	cairou_grid_set_options_private(grid, options, list);

	return grid;
}

cairou_grid_t* cairou_grid_create(
	int rows,
	int cols,
	double size,
	cairou_grid_option_t options,
	...
) { CAIROU_GRID_CREATE_BEGIN

	grid->rows = rows;
	grid->cols = cols;
	grid->size = size;

CAIROU_GRID_CREATE_END

cairou_grid_t* cairou_grid_create_for_rows_cols(
	int rows,
	int cols,
	double width,
	double height,
	cairou_grid_option_t options,
	...
) { CAIROU_GRID_CREATE_BEGIN

	grid->rows = rows;
	grid->cols = cols;

CAIROU_GRID_CREATE_END

cairou_grid_t* cairou_grid_create_for_size(
	double size,
	double width,
	double height,
	cairou_grid_option_t options,
	...
) { CAIROU_GRID_CREATE_BEGIN

	grid->size = size;

CAIROU_GRID_CREATE_END

void cairou_grid_destroy(cairou_grid_t* grid) {
	free(grid);
}

int cairou_grid_get_rows(cairou_grid_t* grid) {
	return grid->rows;
}

int cairou_grid_get_cols(cairou_grid_t* grid) {
	return grid->cols;
}

double cairou_grid_get_size(cairou_grid_t* grid) {
	return grid->size;
}

void cairou_grid_get_options(cairou_grid_t* grid, cairou_grid_option_t options, ...) {
	va_list list;
	int active = 1;

	va_start(list, options);

	while(active) {
		switch(options) {
		case CAIROU_GRID_OPTION_PADDING:
			*va_arg(list, double*) = grid->padding;

			break;

		case CAIROU_GRID_OPTION_ORIGIN:
			*va_arg(list, int*) = grid->origin[0];
			*va_arg(list, int*) = grid->origin[1];

			break;

		case CAIROU_GRID_OPTION_MATRIX:
			memcpy(va_arg(list, cairo_matrix_t*), &grid->matrix, sizeof(cairo_matrix_t));

			break;

		default:
			active = 0;

			break;
		}

		if(active) options = va_arg(list, cairou_grid_option_t);
	}

	va_end(list);
}

void cairou_grid_set_options(cairou_grid_t* grid, cairou_grid_option_t options, ...) {
	va_list list;

	va_start(list, options);

	cairou_grid_set_options_private(grid, options, &list);

	va_end(list);
}

