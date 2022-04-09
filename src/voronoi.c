#include "voronoi.h"
#include "adt/dict.h"
#include "adt/list.h"
#include "adt/pqueue.h"
#include "mem.h"

#define PRECISION 0.00000001

typedef struct site {
	int id;
	const point *pt;
} site;

static inline int site_hash(const site *s, size_t n) { return s->id % n; }

static inline int site_cmp(const site *s1, const site *s2) {
	return !(s1 == s2 || s1->id == s2->id);
}

struct voronoi_diagram {
	const dict *edges;
};

typedef struct edge edge;

struct edge {
	const site *site;
	point *start;
	edge *twin, *prev, *next;
};

static inline int edge_hash(const edge *e, size_t n) {
	if (e->twin)
		return site_hash(e->site, n) ^ site_hash(e->twin->site, n) ^
		       (int)e->start->x ^ (int)e->start->y;
	// boundary edges
	return site_hash(e->site, n) ^ (int)e->start->x ^ (int)e->start->y;
}

static inline int edge_cmp(const edge *e1, edge *e2) {
	// Initially edges don't have origins. Hence, twin site must be checked.
	// Boundary edges don't have twins, but always have origins. This needs to be
	// carefully considered when storing them in a table.
	if (e1 == e2)
		return 0;
	if (site_cmp(e1->site, e2->site) != 0)
		return 1;
	if (e1->twin && e2->twin) {
		if (site_cmp(e1->twin->site, e2->twin->site) != 0)
			return 1;
		if ((!e1->start && e2->start) || (e1->start && !e2->start))
			return 1;
	}
	return ccg_point_eq(e1->start, e2->start, PRECISION);
}

static inline void edge_free(edge *e) {
	if (e->twin)
		e->twin->twin = 0;
	if (e->prev)
		e->prev->next = 0;
	if (e->next)
		e->next->prev = 0;
	ccg_free(e->start);
	ccg_free(e);
}

static voronoi_diagram *voronoi_diag_create() {
	voronoi_diagram *vd;

	vd = ccg_malloc(sizeof(voronoi_diagram));
	vd->edges = ccg_dict_create((hash)edge_hash, (cmp)edge_cmp, (cls)edge_free);
	return vd;
}

static void voronoi_insert_edge(edge *e, voronoi_diagram *vd) {}

typedef struct arc arc;

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

	y1 = ev1->site ? ev1->data.s->pt->y
	               : ev1->data.c->center->y - ev1->data.c->radius;
	y2 = ev2->site ? ev2->data.s->pt->y
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
	const site *focus;
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
static double sweep;

static void handle_site_event(site *site, voronoi_diagram *vd) {
	arc *a;
	beachrv rv;
	edge *e0, *e1;

	a = ccg_malloc(sizeof(arc));
	a->focus = site;
	a->dtrix = &sweep;
	a->circles = 0;
	rv = beachline_insert_arc(a);
	if (rv.left) {
		e0 = ccg_malloc(sizeof(edge));
		e0->site = rv.left->focus;
		e1 = ccg_malloc(sizeof(edge));
		e1->site = site;
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
		s->id = i;
		s->pt = pts[i];
		ev = ccg_malloc(sizeof(event));
		ev->site = 1;
		ev->data.s = s;
		ccg_pqueue_insert(ev, pq);
	}

	while ((ev = ccg_pqueue_remove(pq))) {
		sweep =
			ev->site ? ev->data.s->pt->y : ev->data.c->center->y - ev->data.c->radius;
		if (ev->site)
			handle_site_event(ev->data.s, vd);
		else
			handle_circle_event(ev->data.c);
	}

	ccg_pqueue_destroy(pq);
}
