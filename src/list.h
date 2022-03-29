#ifndef CCG_LIST_H
#define CCG_LIST_H

#include "adt.h"

typedef struct node list;

list *ccg_list_create();
void *ccg_list_put_if_absent(void *item, const cmp cmp, list *ls);
void *ccg_list_find(const void *item, const cmp cmp, const list *ls);
void *ccg_list_remove(const void *item, const cmp cmp, list **ls);
void ccg_list_destroy(const cls cls, list *ls);

#endif
