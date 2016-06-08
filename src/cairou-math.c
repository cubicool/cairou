#include "cairou.h"

double slope(double x0, double y0, double x1, double y1) {
	return (y0 - y1) / (x0 - x1);
}

double y_intercept(double x0, double y0, double x1, double y1) {
	return -(slope(x0, y0, x1, y1) * x0 - y0);
}

double y_intercept_m(double x, double y, double m) {
	return -(m * x - y);
}

/* y = mx + b */
double slope_intercept(double x, double y, double m, double b) {
	return m * x + b;
}

/* double round5(double v, double base) {
	return round(base * round(v / base));
} */

