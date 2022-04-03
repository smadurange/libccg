#include <math.h>

#include "point2d.h"

int ccg_point2d_eq(point2d *p1, point2d *p2, double eps) {
	return (p1 == p2 || (fabs(p1->x - p2->x) < eps && fabs(p1->y - p2->y) < eps));
}
