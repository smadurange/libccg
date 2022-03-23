#ifndef CCG_LIST_H
#define CCG_LIST_H

typedef struct list list;

list *ccg_list_create(int (*eq)(const void *, const void *));
void *ccg_list_append(const void *node, list *list);
void *ccg_list_find(const void *node, const list *list);
void *ccg_list_remove(const void *node, list *list);
void ccg_list_destroy(list *list);

#endif