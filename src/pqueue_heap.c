#include <stddef.h>

#include "mem.h"
#include "pqueue.h"

#define CAPACITY 5

struct pqueue {
  void **heap;
  size_t size;
  size_t capacity;
  int (*cmp)(void *, void *);
};

pqueue *ccg_pqueue_create(int (*cmp)(void *, void *)) {
  pqueue *pq;

  pq = ccg_malloc(sizeof(pqueue));
  pq->heap = NULL;
  pq->size = 0;
  pq->capacity = 0;
  pq->cmp = cmp;
  return pq;
}

size_t ccg_pqueue_size(const pqueue *pq) { return pq->size; }

void ccg_pqueue_insert(void *item, pqueue *pq) {
  int i;
  void **ptr;

  if (pq->capacity == 0) {
    pq->capacity = CAPACITY;
    pq->heap = ccg_malloc(sizeof(item) * CAPACITY);
  } else if (pq->capacity <= pq->size + 1) {
    pq->capacity = pq->size << 1;
    ptr = ccg_malloc(sizeof(item) * pq->capacity);
    for (i = 0; i < pq->size; i++)
      ptr[i] = pq->heap[i];
    ccg_free(pq->heap);
    pq->heap = ptr;
  }
  pq->heap[++pq->size] = item;
  // todo: fix heap
}

void ccg_pqueue_destroy(pqueue *pq) { ccg_free(pq); }