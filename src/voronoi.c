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

static int evcmp(const event *ev1, const event *ev2) {
	double y1, y2;

	y1 = ev1->site ? ev1->data.s->loc->y
	               : ev1->data.c->center->y - ev1->data.c->radius;
	y2 = ev2->site ? ev2->data.s->loc->y
	               : ev2->data.c->center->y - ev2->data.c->radius;
	return y2 < y1 ? -1 : 1;
}

voronoi_diagram *ccg_voronoi_solve(const point **pts, const polyline *bbox) {
	pqueue *pq;

	pq = ccg_pqueue_create((cmp)evcmp, 0);
}
