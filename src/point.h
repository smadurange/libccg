#ifndef CCG_POINT2D_H
#define CCG_POINT2D_H

typedef struct point {
	double x;
	double y;
} point;

int ccg_point_eq(const point *p1, const point *p2, double ep);

#endif
