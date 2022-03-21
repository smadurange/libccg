#ifndef CCG_PQUEUE_H
#define CCG_PQUEUE_H

#include <stddef.h>

typedef struct pqueue pqueue;

pqueue *ccg_pqueue_create(const int (*cmp)(void *, void *));
size_t ccg_pqueue_size(const pqueue *pq);
void ccg_pqueue_insert(void *item, pqueue *pq);
void *ccg_pqueue_remove();
void ccg_pqueue_destroy(pqueue *pq);

#endif