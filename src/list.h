#ifndef CCG_LIST_H
#define CCG_LIST_H

typedef struct node list;
typedef int (*cmp)(const void *, const void *);

list *ccg_list_create();
void *ccg_list_find(const void *item, const cmp eq, const list *ls);
void *ccg_list_find_or_append(void *item, const cmp eq, list *ls);
void *ccg_list_remove(const void *item, const cmp eq, list **ls);
void ccg_list_destroy(list *ls);

#endif