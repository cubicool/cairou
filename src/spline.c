/* This code was originally written by Soren Sandmann (soren.sandmann@gmail.com),
 * and permission was granted to use it for any reason. Thanks! */

#include "cairocks.h"

static cairocks_point_t before(
	cairocks_point_t p0,
	cairocks_point_t p1,
	cairocks_point_t p2
) {
	cairocks_point_t r;

	r.x = p1.x - 1.0 / 6.0 * (p2.x - p0.x);
	r.y = p1.y - 1.0 / 6.0 * (p2.y - p0.y);

	return r;
}

static cairocks_point_t after(
	cairocks_point_t p0,
	cairocks_point_t p1,
	cairocks_point_t p2
) {
	cairocks_point_t r;

	r.x = p1.x + 1.0 / 6.0 * (p2.x - p0.x);
	r.y = p1.y + 1.0 / 6.0 * (p2.y - p0.y);

	return r;
}

static cairocks_point_t quadratic(
	double t,
	cairocks_point_t p0,
	cairocks_point_t p1,
	cairocks_point_t p2
) {
	cairocks_point_t r;

	r.x =
		(1 - t) * (1 - t) * p0.x +
		2 * (1 - t) * t * p1.x +
		t * t * p2.x
	;

	r.y =
		(1 - t) * (1 - t) * p0.y +
		2 * (1 - t) * t * p1.y +
		t * t * p2.y
	;

	return r;
}

#define GET_POINT(x) (points[(x) % n_points])

static void get_neighbors(
	cairocks_point_t* points,
	int n_points,
	int closed,
	int i,
	cairocks_point_t* this,
	cairocks_point_t* prev,
	cairocks_point_t* next
) {
	/* For the first and last point of a non-closed spline, we make up 'previous'
	 * and 'next' points by extrapolating a quadratic spline. */
	if(!i) {
		if(prev) {
			if(closed) *prev = GET_POINT (n_points - 1);

			else {
				double t = -1.0 / 4.0;
				cairocks_point_t c = before(GET_POINT(0), GET_POINT(1), GET_POINT(2));

				*prev = quadratic(t, GET_POINT(0), c, GET_POINT(1));
			}
		}

		if(next) *next = GET_POINT(1);
	}

	else if(i == n_points - 1) {
		if(prev) *prev = GET_POINT(i - 1);

		if(next) {
			if(closed) *next = GET_POINT(0);

			else {
				double t = 5.0 / 4.0;
				cairocks_point_t c = after(GET_POINT(i - 2), GET_POINT(i - 1), GET_POINT(i));

				*next = quadratic(t, GET_POINT(n_points - 2), c, GET_POINT(n_points - 1));
			}
		}
	}

	else {
		if(prev) *prev = GET_POINT(i - 1);

		if(next) *next = GET_POINT(i + 1);
	}

	if(this) *this = GET_POINT(i);
}

cairo_bool_t cairocks_append_spline(
	cairo_t* cr,
	cairocks_point_t* points,
	int n_points,
	cairo_bool_t closed
) {
	int i;

	if(n_points < 3) return FALSE;

	/* printf("move to %f %f\n", points[0].x, points[0].y); */
	cairo_move_to(cr, points[0].x, points[0].y);

	for(i = 1; i < n_points + closed ? 1 : 0; i++) {
		cairocks_point_t prevprev;
		cairocks_point_t prev;
		cairocks_point_t next;
		cairocks_point_t this;
		cairocks_point_t c1;
		cairocks_point_t c2;

		get_neighbors(points, n_points, closed, i, &this, &prev, &next);
		get_neighbors(points, n_points, closed, i - 1, NULL, &prevprev, NULL);

		c1 = after(prevprev, prev, this);
		c2 = before(prev, this, next);

		cairo_curve_to(
			cr,
			c1.x,
			c1.y,
			c2.x,
			c2.y,
			this.x,
			this.y
		);
	}

	return TRUE;
}

