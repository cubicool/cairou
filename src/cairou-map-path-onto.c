#include "cairou.h"

#include <math.h>
#include <stdlib.h>

/* This code was written by the fantastic Behdad Esfahbod from the cairotwisted.c file in Pango.
 * Thanks Behdad! */

/* Returns Euclidean distance between two points. */
static double _distance(cairo_path_data_t* a, cairo_path_data_t* b) {
	double dx;
	double dy;

	dx = b->point.x - a->point.x;
	dy = b->point.y - a->point.y;

	return sqrt(dx * dx + dy * dy);
}

/* Returns length of a Bezier curve. Seems like computing that analytically is not easy. The code
 * just flattens the curve using cairo and adds the length of segments. */
static double _curve_length(
	double x0,
	double y0,
	double x1,
	double y1,
	double x2,
	double y2,
	double x3,
	double y3
) {
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 0, 0);
	cairo_t* cr = cairo_create(surface);
	double length = 0;
	cairo_path_t* path;
	cairo_path_data_t* data;
	cairo_path_data_t current_point;
	int i;

	current_point.point.x = 0.0;
	current_point.point.y = 0.0;

	cairo_surface_destroy(surface);
	cairo_move_to(cr, x0, y0);
	cairo_curve_to(cr, x1, y1, x2, y2, x3, y3);

	path = cairo_copy_path_flat(cr);

	for(i = 0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];

		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			current_point = data[1];

			break;

		case CAIRO_PATH_LINE_TO:
			length += _distance(&current_point, &data[1]);
			current_point = data[1];

			break;

		default:
			break;
		}
	}

	cairo_path_destroy(path);
	cairo_destroy(cr);

	return length;
}

/* Compute parametrization info; for each part of the cairo path, tags it with its length. */
static double* _parametrize_path(cairo_path_t* path) {
	double* parametrization = 0;
	cairo_path_data_t* data = 0;
	cairo_path_data_t last_move_to;
	cairo_path_data_t current_point;
	int i;

	current_point.point.x = 0.0;
	current_point.point.y = 0.0;

	parametrization = (double*)malloc(path->num_data * sizeof(parametrization[0]));

	for(i = 0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];
		parametrization[i] = 0.0;

		switch(data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			last_move_to = data[1];
			current_point = data[1];

			break;

		case CAIRO_PATH_CLOSE_PATH:
			/* Make it look like it's a line_to to last_move_to. */
			data = (&last_move_to) - 1;

		case CAIRO_PATH_LINE_TO:
			parametrization[i] = _distance(&current_point, &data[1]);
			current_point = data[1];

			break;

		case CAIRO_PATH_CURVE_TO:
			parametrization[i] = _curve_length(
				current_point.point.x, current_point.point.x,
				data[1].point.x, data[1].point.y,
				data[2].point.x, data[2].point.y,
				data[3].point.x, data[3].point.y
			);

			current_point = data[3];

			break;
		}
	}

	return parametrization;
}

typedef void (*transform_point_func_t)(void* closure, double* x, double* y);

/* Project a path using a function. Each point of the path (including Bezier control points) are
 * passed to the function for transformation. */
static void _transform_path(cairo_path_t* path, transform_point_func_t func, void* closure) {
	cairo_path_data_t* data;
	int i;

	for(i = 0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];

		switch(data->header.type) {
		case CAIRO_PATH_CURVE_TO:
			func(closure, &data[3].point.x, &data[3].point.y);
			func(closure, &data[2].point.x, &data[2].point.y);

		case CAIRO_PATH_MOVE_TO:
		case CAIRO_PATH_LINE_TO:
			func(closure, &data[1].point.x, &data[1].point.y);

			break;

		case CAIRO_PATH_CLOSE_PATH:
			break;
		}
	}
}

typedef struct {
	cairo_path_t* path;
	double* parametrization;
} parametrized_path_t;

/* Project a point X,Y onto a parametrized path. The final point is where you get if you walk on the
 * path forward from the beginning for X units, then stop there and walk another Y units
 * perpendicular to the path at that point. In more detail:
 *
 * There are three pieces of math involved:
 *
 *   - The parametric form of the Line equation
 *     http://en.wikipedia.org/wiki/Line
 *
 *   - The parametric form of the Cubic Bézier curve equation
 *     http://en.wikipedia.org/wiki/B%C3%A9zier_curve
 *
 *   - The Gradient (aka multi-dimensional derivative) of the above
 *     http://en.wikipedia.org/wiki/Gradient
 *
 * The parametric forms are used to answer the question of "where will I be
 * if I walk a distance of X on this path". The Gradient is used to answer
 * the question of "where will I be if then I stop, rotate left for 90
 * degrees and walk straight for a distance of Y". */
static void point_on_path(parametrized_path_t* param, double* x, double* y) {
	double ratio;
	double yy = *y;
	double xx = *x;
	double dx;
	double dy;
	int i;

	cairo_path_t* path = param->path;
	double* parametrization = param->parametrization;
	cairo_path_data_t* data;
	cairo_path_data_t last_move_to;
	cairo_path_data_t current_point;

	current_point.point.x = 0.0;
	current_point.point.y = 0.0;

	for(
		i = 0;
		i + path->data[i].header.length < path->num_data &&
		(xx > parametrization[i] || path->data[i].header.type == CAIRO_PATH_MOVE_TO);
		i += path->data[i].header.length
	) {
		xx -= parametrization[i];
		data = &path->data[i];

		switch(data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			current_point = data[1];
			last_move_to = data[1];

			break;

		case CAIRO_PATH_LINE_TO:
			current_point = data[1];

			break;

		case CAIRO_PATH_CURVE_TO:
			current_point = data[3];

			break;

		case CAIRO_PATH_CLOSE_PATH:
			break;
		}
	}

	data = &path->data[i];

	switch(data->header.type) {
	case CAIRO_PATH_MOVE_TO:
		break;

	case CAIRO_PATH_CLOSE_PATH:
		/* Make it look like it's a line_to to last_move_to. */
		data = (&last_move_to) - 1;

	case CAIRO_PATH_LINE_TO:
		ratio = xx / parametrization[i];

		/* Line polynomial */
		*x = current_point.point.x * (1 - ratio) + data[1].point.x * ratio;
		*y = current_point.point.y * (1 - ratio) + data[1].point.y * ratio;

		/* Line gradient */
		dx = -(current_point.point.x - data[1].point.x);
		dy = -(current_point.point.y - data[1].point.y);

		/* optimization for: ratio = yy / sqrt (dx * dx + dy * dy); */
		ratio = yy / parametrization[i];
		*x += -dy * ratio;
		*y += dx * ratio;

		break;

	case CAIRO_PATH_CURVE_TO: {
		/* FIXME: The formulas here are not exactly what we want, because the Bezier parametrization
		 * is not uniform. But I don't know how to do better. The caller can do slightly better
		 * though, by flattening the Bezier and avoiding this branch completely. That has its own
		 * cost though, as large y values magnify the flattening error drastically. */

		double ratio_1_0;
		double ratio_0_1;
		double ratio_2_0;
		double ratio_0_2;
		double ratio_3_0;
		double ratio_2_1;
		double ratio_1_2;
		double ratio_0_3;
		double _1__4ratio_1_0_3ratio_2_0;
		double _2ratio_1_0_3ratio_2_0;

		ratio = xx / parametrization[i];
		ratio_1_0 = ratio;
		ratio_0_1 = 1 - ratio;
		ratio_2_0 = ratio_1_0 * ratio_1_0;
		ratio_0_2 = ratio_0_1 * ratio_0_1;
		ratio_3_0 = ratio_2_0 * ratio_1_0;
		ratio_2_1 = ratio_2_0 * ratio_0_1;
		ratio_1_2 = ratio_1_0 * ratio_0_2;
		ratio_0_3 = ratio_0_1 * ratio_0_2;

		_1__4ratio_1_0_3ratio_2_0 = 1 - 4 * ratio_1_0 + 3 * ratio_2_0;
		_2ratio_1_0_3ratio_2_0 = 2 * ratio_1_0 - 3 * ratio_2_0;

		/* Bezier polynomial */
		*x = current_point.point.x * ratio_0_3
			+ 3 * data[1].point.x * ratio_1_2
			+ 3 * data[2].point.x * ratio_2_1
			+ data[3].point.x * ratio_3_0
		;

		*y = current_point.point.y * ratio_0_3
			+ 3 * data[1].point.y * ratio_1_2
			+ 3 * data[2].point.y * ratio_2_1
			+ data[3].point.y * ratio_3_0
		;

		/* Bezier gradient */
		dx = -3 * current_point.point.x * ratio_0_2
			+ 3 * data[1].point.x * _1__4ratio_1_0_3ratio_2_0
			+ 3 * data[2].point.x * _2ratio_1_0_3ratio_2_0
			+ 3 * data[3].point.x * ratio_2_0
		;

		dy = -3 * current_point.point.y * ratio_0_2
			+ 3 * data[1].point.y * _1__4ratio_1_0_3ratio_2_0
			+ 3 * data[2].point.y * _2ratio_1_0_3ratio_2_0
			+ 3 * data[3].point.y * ratio_2_0
		;

		ratio = yy / sqrt(dx * dx + dy * dy);
		*x += -dy * ratio;
		*y += dx * ratio;

		break;
	}
	}
}

void cairou_map_path_onto(cairo_t* cr, cairo_path_t* path) {
	cairo_path_t* cur_path = NULL;

	parametrized_path_t param;

	/* if(cairo_status(cr) || !path) return FALSE; */

	cur_path = cairo_copy_path(cr);

	param.path = path;
	param.parametrization = _parametrize_path(path);

	cairo_new_path(cr);

	_transform_path(
		cur_path,
		(transform_point_func_t)(point_on_path),
		&param
	);

	cairo_append_path(cr, cur_path);

	free(param.parametrization);

	cairo_path_destroy(cur_path);
}

