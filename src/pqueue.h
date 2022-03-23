#ifndef CCG_PQUEUE_H
#define CCG_PQUEUE_H

#include <stddef.h>

typedef struct pqueue pqueue;

pqueue *ccg_pqueue_create(int (*cmp)(const void *, const void *));
size_t ccg_pqueue_size(const pqueue *pq);
void ccg_pqueue_insert(void *item, pqueue *pq);
void *ccg_pqueue_remove(pqueue *pq);
void ccg_pqueue_destroy(pqueue *pq);

#endif