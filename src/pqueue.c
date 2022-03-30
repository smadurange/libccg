#include <stddef.h>
#include <sys/types.h>

#include "adt.h"
#include "mem.h"
#include "pqueue.h"

#define BLKLEN 4

struct pqueue {
	size_t cap;
	size_t size;
	cmp cmp;
	cls cls;
	void **heap;
};

static void fix_up(register int i, const pqueue *pq);
static void fix_down(register int i, const pqueue *pq);

pqueue *ccg_pqueue_create(const cmp cmp, const cls cls) {
	pqueue *pq;

	pq = ccg_malloc(sizeof(pqueue));
	pq->cap = BLKLEN;
	pq->size = 0;
	pq->cmp = cmp;
	pq->cls = cls;
	pq->heap = ccg_calloc(BLKLEN, sizeof(void *));
	return pq;
}

size_t ccg_pqueue_size(const pqueue *pq) { return pq->size; }

void ccg_pqueue_insert(void *item, pqueue *pq) {
	if (pq->cap <= pq->size + 1)
		pq->heap = ccg_realloc(pq->heap, sizeof(void *) * (pq->cap += BLKLEN));
	pq->heap[++pq->size] = item;
	fix_up(pq->size, pq);
}

void *ccg_pqueue_remove(pqueue *pq) {
	void *tmp;

	if (pq->size == 0)
		return 0;
	tmp = pq->heap[1];
	pq->heap[1] = pq->heap[pq->size];
	pq->heap[pq->size] = tmp;
	fix_down(1, pq);
	if (pq->cap - pq->size > BLKLEN)
		pq->heap =
			ccg_realloc(pq->heap, sizeof(void *) * (pq->cap = pq->size + BLKLEN));
	return pq->heap[pq->size--];
}

void ccg_pqueue_destroy(pqueue *pq) {
	int i;

	for (i = 0; i < pq->size; i++) {
		if (pq->cls)
			pq->cls(pq->heap[i]);
	}
	ccg_free(pq);
}

static void fix_up(register int i, const pqueue *pq) {
	void *tmp;

	while (i > 1 && pq->cmp(pq->heap[i >> 1], pq->heap[i]) < 0) {
		tmp = pq->heap[i];
		pq->heap[i] = pq->heap[i >> 1];
		pq->heap[i >> 1] = tmp;
		i >>= 1;
	}
}

static void fix_down(register int i, const pqueue *pq) {
	int j;
	void *tmp;

	while (i << 1 <= pq->size - 1) {
		j = i << 1;
		if (j < pq->size - 1 && pq->cmp(pq->heap[j], pq->heap[j + 1]) < 0)
			j++;
		if (pq->cmp(pq->heap[i], pq->heap[j]) >= 0)
			break;
		tmp = pq->heap[i];
		pq->heap[i] = pq->heap[j];
		pq->heap[j] = tmp;
		i = j;
	}
}
