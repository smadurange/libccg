#ifndef CCG_POINT2D_H
#define CCG_POINT2D_H

#include <float.h>
#include <math.h>

#define ccg_point2d_eq(a, b, eps)                                              \
	(a == b || (fabs(((point2d *)a)->x - ((point2d *)b)->x) < eps &&             \
							fabs(((point2d *)a)->y - ((point2d *)b)->y) < eps))

#define ccg_point2d_hash(a, n)                                                 \
	((17 * 31 * (((((point2d *)a)->x + DBL_MAX) / (DBL_MAX + DBL_MAX)) * n)) *   \
	 31 * (((((point2d *)a)->y + DBL_MAX) / (DBL_MAX + DBL_MAX)) * n))

typedef struct point2d {
	double x;
	double y;
} point2d;

#endif
