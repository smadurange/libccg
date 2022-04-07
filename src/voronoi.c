#include "voronoi.h"
#include "adt/list.h"
#include "adt/pqueue.h"

typedef struct site {
	point *loc;
} site;

typedef struct arc {
	point *focus;
	double *directrix;
	list *circles;
} arc;

typedef struct circle {
	arc *arc;
	point *center;
	double radius;
} circle;

typedef struct event {
	char site;
	union {
		site *s;
		circle *c;
	} data;
} event;

static int eventcmp(const event *ev1, const event *ev2) {
	double y1, y2;

	y1 = ev1->site ? ((site *)(ev1->data.s))->loc->y
								 : ((circle *)(ev1->data.c))->center->y -
										 ((circle *)(ev1->data.c))->radius;
	y2 = ev2->site ? ((site *)(ev2->data.s))->loc->y
								 : ((circle *)(ev2->data.c))->center->y -
										 ((circle *)(ev2->data.c))->radius;
}

voronoi_diagram *ccg_voronoi_solve(const point **pts, const polyline *bbox) {}
