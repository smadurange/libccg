#ifndef CCG_VORONOI2D_H
#define CCG_VORONOI2D_H

#include "point.h"
#include "polyline.h"

typedef struct voronoi_diagram voronoi_diagram;

voronoi_diagram *ccg_voronoi_solve(const point **pts, const polyline *bbox);

#endif
