#include "voronoi.h"
#include "adt/dict.h"
#include "adt/list.h"
#include "adt/pqueue.h"
#include "mem.h"

struct voronoi_diagram {
	const dict *edges;
};

typedef struct edge edge;

struct edge {
	const point *site;
	point *origin;
	edge *twin, *prev, *next;
};

static int edge_hash(const edge *e, const size_t n) {}

static int edge_cmp(const edge *e1, const edge *e2) {}

static void edge_cls(edge *e) {}

static voronoi_diagram *voronoi_diag_create() {
	voronoi_diagram *vd;

	vd = ccg_malloc(sizeof(voronoi_diagram));
	vd->edges = ccg_dict_create((hash)edge_hash, (cmp)edge_cmp, (cls)edge_cls);
	return vd;
}

static void voronoi_insert_edge(edge *e, voronoi_diagram *vd) {}

typedef struct arc arc;

typedef struct site {
	const point *loc;
} site;

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

struct arc {
	const point *focus;
	const double *dtrix;
	circle *circles;
};

typedef struct beachrv {
	arc *left, *right;
	circle **circ;
} beachrv;

typedef struct beachline {
} beachline;

static beachrv beachline_insert_arc(arc *a) {}

static pqueue *pq;
static beachline beach;
static double sweepline;

static void handle_site_event(site *site, voronoi_diagram *vd) {
	arc *a;
	beachrv rv;
	edge *e0, *e1;

	a = ccg_malloc(sizeof(arc));
	a->focus = site->loc;
	a->dtrix = &sweepline;
	a->circles = 0;
	rv = beachline_insert_arc(a);
	if (rv.left) {
		e0 = ccg_malloc(sizeof(edge));
		e0->site = rv.left->focus;
		e1 = ccg_malloc(sizeof(edge));
		e1->site = site->loc;
		e0->twin = e1;
		e1->twin = e0;
		voronoi_insert_edge(e0, vd);
		voronoi_insert_edge(e1, vd);
	}
	for (; *rv.circ; rv.circ++)
		ccg_pqueue_insert(*rv.circ, pq);
}

static void handle_circle_event(circle *circle) {}

voronoi_diagram *ccg_voronoi_solve(const point **pts, int n,
                                   const polyline *bbox) {
	int i;
	site *s;
	event *ev;
	voronoi_diagram *vd;

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
		sweepline = ev->site ? ev->data.s->loc->y
		                     : ev->data.c->center->y - ev->data.c->radius;
		if (ev->site)
			handle_site_event(ev->data.s, vd);
		else
			handle_circle_event(ev->data.c);
	}

	ccg_pqueue_destroy(pq);
}
