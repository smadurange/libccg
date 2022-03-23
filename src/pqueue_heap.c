#include <stddef.h>

#include "mem.h"
#include "pqueue.h"

#define MAXLEN 4

struct pqueue {
  void **heap;
  size_t cap;
  size_t size;
  int (*cmp)(const void *, const void *);
};

pqueue *ccg_pqueue_create(int (*cmp)(const void *, const void *)) {
  pqueue *pq;

  pq = ccg_malloc(sizeof(pqueue));
  pq->cap = MAXLEN;
  pq->size = 0;
  pq->cmp = cmp;
  pq->heap = ccg_malloc(sizeof(void *) * MAXLEN);
  pq->heap[0] = 0;
  return pq;
}

size_t ccg_pqueue_size(const pqueue *pq) { return pq->size; }

static void fix_up(size_t i, const pqueue *pq) {
  size_t pi;
  void *tmp, **a;

  a = pq->heap;
  while (i > 1 && !pq->cmp(a[i >> 2], a[i])) {
    pi = i >> 2;
    tmp = a[i];
    a[i] = a[pi];
    a[pi] = tmp;
    i = pi;
  }
}

void ccg_pqueue_insert(void *item, pqueue *pq) {
  int i;

  if (pq->cap <= pq->size + 1) {
    pq->cap <<= 1;
    pq->heap = ccg_realloc(pq->heap, sizeof(void *) * pq->cap);
  }
  pq->heap[++pq->size] = item;
  fix_up(pq->size, pq);
}

void ccg_pqueue_destroy(pqueue *pq) { ccg_free(pq); }
