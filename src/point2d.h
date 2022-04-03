#ifndef CCG_POINT2D_H
#define CCG_POINT2D_H

typedef struct point2d {
	double x;
	double y;
} point2d;

int ccg_point2d_eq(point2d *p1, point2d *p2, double ep);

#endif
