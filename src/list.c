#include "list.h"
#include "mem.h"

#define BLK_LEN 4

typedef struct node node;

struct node {
  void *item;
  node *next;
};

list *ccg_list_create() {
  list *head;

  head = ccg_malloc(sizeof(list));
  head->item = 0;
  head->next = 0;
  return head;
}

void *ccg_list_find(const void *item, const cmp eq, const list *ls) {
  for (; ls != 0; ls = ls->next)
    if (eq(ls->item, item))
      return ls->item;
  return 0;
}

void *ccg_list_find_or_append(void *item, const cmp eq, list *ls) {
  node *tail;

  for (; ls != 0; ls = ls->next)
    if (eq(ls->item, item))
      return ls->item;
    else
      tail = ls;
  tail->next = ccg_malloc(sizeof(node));
  tail->next->item = item;
  tail->next->next = 0;
  return 0;
}

void *ccg_list_remove(const void *item, const cmp eq, list *ls) {
  // todo:
  return 0;
}

void ccg_list_destroy(list *ls) {
  // todo:
}