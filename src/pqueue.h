#ifndef CCG_PQUEUE_H
#define CCG_PQUEUE_H

#include <stddef.h>

#include "adt.h"

typedef struct pqueue pqueue;

pqueue *ccg_pqueue_create(const comparer cmp, const finalizer fin);
size_t ccg_pqueue_size(const pqueue *pq);
void ccg_pqueue_insert(void *item, pqueue *pq);
void *ccg_pqueue_remove(pqueue *pq);
void ccg_pqueue_destroy(pqueue *pq);

#endif