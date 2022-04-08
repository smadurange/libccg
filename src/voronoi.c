#include "voronoi.h"
#include "adt/list.h"
#include "adt/pqueue.h"
#include "mem.h"

typedef struct site {
	const point *loc;
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

static void evfree(event *ev) {
	if (ev->site)
		ccg_free(ev->data.s);
	else
		ccg_free(ev->data.c);
	ccg_free(ev);
}

static void handle_site_event(site *site, double *sweep) {}

static void handle_circle_event(circle *circle) {}

voronoi_diagram *ccg_voronoi_solve(const point **pts, int n,
                                   const polyline *bbox) {
	int i;
	double sweep;
	site *s;
	event *ev;
	pqueue *pq;

	pq = ccg_pqueue_create((cmp)evcmp, (cls)evfree);
	for (i = 0; i < n; i++) {
		s = ccg_malloc(sizeof(site));
		s->loc = pts[i];
		ev = ccg_malloc(sizeof(event));
		ev->site = 1;
		ev->data.s = s;
		ccg_pqueue_insert(ev, pq);
	}

	while ((ev = ccg_pqueue_remove(pq))) {
		sweep = ev->site ? ev->data.s->loc->y
		                 : ev->data.c->center->y - ev->data.c->radius;
		if (ev->site)
			handle_site_event(ev->data.s, &sweep);
		else
			handle_circle_event(ev->data.c);
	}

	ccg_pqueue_destroy(pq);
}
