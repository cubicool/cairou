/* This code was originally written by Soren Sandmann (soren.sandmann@gmail.com),
 * and permission was granted to use it for any reason. Thanks! */

#include "cairou.h"

static cairou_point_t cairou_spline_private_before(
	cairou_point_t p0,
	cairou_point_t p1,
	cairou_point_t p2
) {
	cairou_point_t r;

	r.x = p1.x - 1.0 / 6.0 * (p2.x - p0.x);
	r.y = p1.y - 1.0 / 6.0 * (p2.y - p0.y);

	return r;
}

static cairou_point_t cairou_spline_private_after(
	cairou_point_t p0,
	cairou_point_t p1,
	cairou_point_t p2
) {
	cairou_point_t r;

	r.x = p1.x + 1.0 / 6.0 * (p2.x - p0.x);
	r.y = p1.y + 1.0 / 6.0 * (p2.y - p0.y);

	return r;
}

static cairou_point_t cairou_spline_private_quadratic(
	double t,
	cairou_point_t p0,
	cairou_point_t p1,
	cairou_point_t p2
) {
	cairou_point_t r;

	r.x = (1.0 - t) * (1.0 - t) * p0.x + 2.0 * (1.0 - t) * t * p1.x + t * t * p2.x;
	r.y = (1.0 - t) * (1.0 - t) * p0.y + 2.0 * (1.0 - t) * t * p1.y + t * t * p2.y;

	return r;
}

#define GET_POINT(x) (points[(x) % n_points])

static void cairou_spline_private_get_neighbors(
	cairou_point_t* points,
	int n_points,
	int closed,
	int i,
	cairou_point_t* this,
	cairou_point_t* prev,
	cairou_point_t* next
) {
	/* For the first and last point of a non-closed spline, we make up 'previous'
	 * and 'next' points by extrapolating a cairou_spline_private_quadratic spline. */
	if(!i) {
		if(prev) {
			if(closed) *prev = GET_POINT(n_points - 1);

			else {
				double t = -1.0 / 4.0;

				cairou_point_t c = cairou_spline_private_before(
					GET_POINT(0),
					GET_POINT(1),
					GET_POINT(2)
				);

				*prev = cairou_spline_private_quadratic(t, GET_POINT(0), c, GET_POINT(1));
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

				cairou_point_t c = cairou_spline_private_after(
					GET_POINT(i - 2),
					GET_POINT(i - 1),
					GET_POINT(i)
				);

				*next = cairou_spline_private_quadratic(
					t,
					GET_POINT(n_points - 2),
					c,
					GET_POINT(n_points - 1)
				);
			}
		}
	}

	else {
		if(prev) *prev = GET_POINT(i - 1);

		if(next) *next = GET_POINT(i + 1);
	}

	if(this) *this = GET_POINT(i);
}

cairo_bool_t cairou_append_spline(
	cairo_t* cr,
	cairou_point_t* points,
	int n_points,
	cairo_bool_t closed
) {
	int i;

	if(n_points < 3) return FALSE;

	cairo_move_to(cr, points[0].x, points[0].y);

	for(i = 1; i < n_points + closed ? 1 : 0; i++) {
		cairou_point_t prevprev;
		cairou_point_t prev;
		cairou_point_t next;
		cairou_point_t this;
		cairou_point_t c1;
		cairou_point_t c2;

		cairou_spline_private_get_neighbors(
			points,
			n_points,
			closed,
			i,
			&this,
			&prev,
			&next
		);

		cairou_spline_private_get_neighbors(
			points,
			n_points,
			closed,
			i - 1,
			NULL,
			&prevprev,
			NULL
		);

		c1 = cairou_spline_private_after(prevprev, prev, this);
		c2 = cairou_spline_private_before(prev, this, next);

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

