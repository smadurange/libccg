#ifndef CCG_POINT2D_H
#define CCG_POINT2D_H

#include <math.h>

#define ccg_point2d_eq(a, b, eps)                                              \
	(fabs(a->x - b->x) < eps && fabs(a->y - b->y) < eps)

typedef struct point2d {
	double x;
	double y;
} point2d;

#endif
