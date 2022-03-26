#include "list.h"
#include "mem.h"

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

void *ccg_list_put_if_absent(void *item, const comparer eq, list *ls) {
  if (!ls->item) {
    ls->item = item;
    return 0;
  }
  for (; ls != 0; ls = ls->next) {
    if (eq(ls->item, item))
      return ls->item;
    if (ls->next == 0)
      break;
  }
  ls->next = ccg_malloc(sizeof(node));
  ls->next->item = item;
  ls->next->next = 0;
  return 0;
}

void *ccg_list_find(const void *item, const comparer eq, const list *ls) {
  for (; ls != 0; ls = ls->next)
    if (eq(ls->item, item))
      return ls->item;
  return 0;
}

void *ccg_list_remove(const void *item, const comparer eq, list **ls) {
  void *rv;
  node *tmp;

  if (!(*ls)->item)
    return 0;
  if (eq((*ls)->item, item)) {
    rv = (*ls)->item;
    tmp = *ls;
    *ls = (*ls)->next;
    ccg_free(tmp);
    return rv;
  }
  for (; (*ls) != 0; *ls = (*ls)->next) {
    if ((*ls)->next && eq((*ls)->next->item, item)) {
      rv = (*ls)->next->item;
      tmp = (*ls)->next;
      (*ls)->next = (*ls)->next->next;
      ccg_free(tmp);
      return rv;
    }
  }
  return 0;
}

void ccg_list_destroy(list *ls) {
  node *prev, *curr;

  for (prev = 0, curr = ls; curr != 0; curr = curr->next) {
    if (prev != 0)
      ccg_free(prev);
    prev = curr;
  }
}
