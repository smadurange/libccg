#ifndef CCG_VORONOI2D_H
#define CCG_VORONOI2D_H

#include <stddef.h>

#include "point.h"

typedef struct voronoi_diagram voronoi_diagram;

voronoi_diagram *ccg_voronoi_solve(const point *pts, size_t n,
                                   const point *bbox);

void ccg_voronoi_destroy(voronoi_diagram *vd);

#endif
