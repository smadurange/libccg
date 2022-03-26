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

void *ccg_list_find(const void *item, const cmp eq, const list *ls) {
  for (; ls != 0; ls = ls->next)
    if (eq(ls->item, item))
      return ls->item;
  return 0;
}

void *ccg_list_find_or_append(void *item, const cmp eq, list *ls) {
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

void *ccg_list_remove(const void *item, const cmp eq, list **ls) {
  void *rv;
  node register *ptr, *t;

  if (eq((*ls)->item, item)) {
    rv = (*ls)->item;
    ls = &(*ls)->next;
    ccg_free(*ls);
    return rv;
  }
  for (ptr = (*ls); ptr != 0; ptr = ptr->next) {
    if (ptr->next && eq(ptr->next->item, item)) {
      rv = ptr->next->item;
      t = ptr->next;
      ptr->next = ptr->next->next;
      ccg_free(t);
      return rv;
    }
  }
  return 0;
}

void ccg_list_destroy(list *ls) {
  register node *prev, *curr;

  for (prev = 0, curr = ls; curr != 0; curr = curr->next) {
    if (prev != 0)
      ccg_free(prev);
    prev = curr;
  }
}