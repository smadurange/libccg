#ifndef CCG_VORONOI2D_H
#define CCG_VORONOI2D_H

#include "point.h"
#include "polyline.h"

typedef struct voronoi_diagram voronoi_diagram;

void ccg_voronoi_destroy(voronoi_diagram *vd);

voronoi_diagram *ccg_voronoi_solve(const point **pts, int n,
                                   const polyline *bbox);

#endif
