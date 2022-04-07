#include <math.h>

#include "point.h"

int ccg_point_eq(const point *p1, const point *p2, double eps) {
	return (p1 == p2 || (fabs(p1->x - p2->x) < eps && fabs(p1->y - p2->y) < eps));
}
