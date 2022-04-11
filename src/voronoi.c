#include <math.h>

#include "adt/dict.h"
#include "adt/list.h"
#include "adt/pqueue.h"
#include "mem.h"
#include "voronoi.h"

#define EPSILON 0.00000001

typedef struct site {
	int id;
	const point *pt;
} site;

static inline int site_hash(const site *s, size_t n) { return s->id % n; }

static inline int site_cmp(const site *s1, const site *s2) {
	return !(s1 == s2 || s1->id == s2->id);
}

typedef struct edge edge;

struct edge {
	const site *site;
	point *start;
	edge *twin, *prev, *next;
};

static int edge_hash(const edge *e, size_t n) {
	if (e->twin)
		return site_hash(e->site, n) ^ site_hash(e->twin->site, n) ^
		       (int)e->start->x ^ (int)e->start->y;
	// boundary edges
	return site_hash(e->site, n) ^ (int)e->start->x ^ (int)e->start->y;
}

static int edge_cmp(const edge *e1, edge *e2) {
	// initially edges don't have origins. hence, twin site must be checked.
	// boundary edges don't have twins, but always have origins.
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
	return ccg_point_eq(e1->start, e2->start, EPSILON);
}

static void edge_destroy(edge *e) {
	if (e->twin)
		e->twin->twin = 0;
	if (e->prev)
		e->prev->next = 0;
	if (e->next)
		e->next->prev = 0;
	ccg_free(e->start);
	ccg_free(e);
}

struct voronoi_diagram {
	site **sites;
	dict *edges;
};

void ccg_voronoi_destroy(voronoi_diagram *vd) {
	for (; *(vd->sites); vd->sites++)
		ccg_free(*(vd->sites));
	ccg_free(vd->sites);
	ccg_dict_destroy(vd->edges);
}

static voronoi_diagram *voronoi_diag_create(site **s) {
	voronoi_diagram *vd;

	vd = ccg_malloc(sizeof(voronoi_diagram));
	vd->sites = s;
	vd->edges =
		ccg_dict_create((hash)edge_hash, (cmp)edge_cmp, (cls)edge_destroy);
	return vd;
}

static void voronoi_insert_edge(edge *e, voronoi_diagram *vd) {}

typedef struct arc arc;

typedef struct circle {
	arc *arc;
	point *center;
	double radius;
	char deleted;
} circle;

struct arc {
	const site *focus;
	const double *dtx;
	circle *circles;
};

typedef struct beachline {
	arc **arcs;
	int len;
	int cap;
} beachline;

typedef struct event {
	char site;
	const double *swp;
	pqueue *pq;
	beachline *beach;
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

static void ev_destroy(event *ev) {
	if (ev->site)
		ccg_free(ev->data.s);
	else
		ccg_free(ev->data.c);
	ccg_free(ev);
}

static double arc_eval(double x, const arc *a) {
	// vertical parabola: (x-h)^2 = 4|fy-swp|(y-k)
	double xh;
	const point *f;

	f = a->focus->pt;
	xh = x - f->x;
	return ((xh * xh) / (4 * (fabs(f->y - *a->dtx) / 2))) + (f->y + *a->dtx) / 2;
}

static inline void arc_destroy(arc *a) {
	ccg_free(a->circles);
	ccg_free(a);
}

typedef struct beachrv {
	arc *left, *right;
	circle **circles;
} beachrv;

static void breakpoint(const arc *a, const arc *b, point *bp) {
	// https://github.com/Zalgo2462/VoronoiLib
	const point *fa, *fb;
	double dtx, fax, fay, fbx, fby;

	fa = a->focus->pt;
	fb = b->focus->pt;
	dtx = *a->dtx;
	fax = fa->x, fay = fa->y;
	fbx = fb->x, fby = fb->y;
	bp->x = fabs(fay - fby) < EPSILON
	          ? (fax + fbx) / 2
	          : (fbx * (dtx - fay) + fax * (fby - dtx) +
	             sqrt((dtx - fby) * (dtx - fay) *
	                  ((fbx - fax) * (fbx - fax) + (fby - fay) * (fby - fay)))) /
	              (fby - fay);
	bp->y = fabs(fay - dtx) < EPSILON ? arc_eval(bp->x, b) : arc_eval(bp->x, a);
}

static int beachline_find_arc(const arc *a, beachline *beach) {
	int idx, i;
	point bp;
	double x, y, ymin, x0, x1;

	idx = -1;
	x0 = -INFINITY, x1 = ymin = INFINITY;
	x = a->focus->pt->x;
	for (i = 0; i < beach->len; i++) {
		y = arc_eval(x, beach->arcs[i]);
		if (y != INFINITY && (fabs(y - ymin) < EPSILON || y < ymin)) {
			if (i - 1 >= 0) {
				breakpoint(beach->arcs[i - 1], beach->arcs[i], &bp);
				x0 = bp.x;
			} else
				x0 = -INFINITY;
			if (i + 1 < beach->len) {
				breakpoint(beach->arcs[i], beach->arcs[i + 1], &bp);
				x1 = bp.x;
			} else
				x1 = INFINITY;
		}
		if ((fabs(x - x0) < EPSILON || x > x0) &&
		    (fabs(x - x1) < EPSILON || x < x1)) {
			ymin = y;
			idx = i;
			if ((x1 != -INFINITY || x1 != INFINITY) && fabs(x - x1) < EPSILON)
				break;
		}
	}
	return idx;
}

static void beachline_insert_arc(arc *a, beachrv *rv, beachline *beach) {
	int i;
	arc *sa, *lha, *rha;

	if (!beach->arcs) {
		beach->cap = 4;
		beach->arcs = ccg_malloc(sizeof(arc *) * beach->cap);
		beach->arcs[0] = a;
		return;
	}
	i = beachline_find_arc(a, beach);
	if (i >= 0) {
		sa = beach->arcs[i];
		for (; sa->circles; (sa->circles)++)
			sa->circles->deleted = 1;
	}
	// todo:
}

static void beachline_destroy(beachline *beach) { ccg_free(beach); }

static void handle_site_event(event *ev, voronoi_diagram *vd) {
	arc *a;
	beachrv rv;
	edge *e0, *e1;

	rv = (beachrv){};
	a = ccg_malloc(sizeof(arc));
	a->focus = ev->data.s;
	a->circles = 0;
	beachline_insert_arc(a, &rv, ev->beach);
	if (rv.left) {
		e0 = ccg_malloc(sizeof(edge));
		e0->site = rv.left->focus;
		e1 = ccg_malloc(sizeof(edge));
		e1->site = ev->data.s;
		e0->twin = e1;
		e1->twin = e0;
		voronoi_insert_edge(e0, vd);
		voronoi_insert_edge(e1, vd);
	}
	for (; rv.circles; rv.circles++)
		ccg_pqueue_insert(*rv.circles, ev->pq);
}

static void handle_circle_event(event *ev) {}

voronoi_diagram *ccg_voronoi_solve(const point *pts, size_t n,
                                   const point *bbox) {
	int i;
	double swp;
	site *s, **sites;
	event *ev;
	pqueue *pq;
	beachline *beach;
	voronoi_diagram *vd;

	sites = ccg_malloc(sizeof(site *) * (n + 1));
	beach = ccg_malloc(sizeof(beachline));
	vd = voronoi_diag_create(sites);
	pq = ccg_pqueue_create((cmp)evcmp, (cls)ev_destroy);
	for (i = 0; i < n; i++) {
		s = ccg_malloc(sizeof(site));
		s->id = i;
		s->pt = &pts[i];
		sites[i] = s;
		ev = ccg_malloc(sizeof(event));
		ev->swp = &swp;
		ev->site = 1;
		ev->data.s = s;
		ccg_pqueue_insert(ev, pq);
	}
	sites[i] = 0;
	while ((ev = ccg_pqueue_remove(pq))) {
		if (ev->site) {
			swp = ev->data.s->pt->y;
			handle_site_event(ev, vd);
		} else {
			swp = ev->data.c->center->y - ev->data.c->radius;
			handle_circle_event(ev);
		}
	}
	beachline_destroy(beach);
	ccg_pqueue_destroy(pq);
	return vd;
}
