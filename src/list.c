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

void *ccg_list_put_if_absent(void *item, const cmp cmp, list *ls) {
	if (!ls->item) {
		ls->item = item;
		return 0;
	}
	for (; ls; ls = ls->next) {
		if (cmp(ls->item, item) == 0)
			return ls->item;
		if (!ls->next)
			break;
	}
	ls->next = ccg_malloc(sizeof(node));
	ls->next->item = item;
	ls->next->next = 0;
	return 0;
}

void *ccg_list_find(const void *item, const cmp cmp, const list *ls) {
	for (; ls; ls = ls->next)
		if (cmp(ls->item, item) == 0)
			return ls->item;
	return 0;
}

void *ccg_list_remove(const void *item, const cmp cmp, list **ls) {
	void *rv;
	node *tmp;

	if (!(*ls)->item)
		return 0;
	if (cmp((*ls)->item, item) == 0) {
		rv = (*ls)->item;
		tmp = *ls;
		*ls = (*ls)->next;
		ccg_free(tmp);
		return rv;
	}
	for (; (*ls); *ls = (*ls)->next) {
		if ((*ls)->next && cmp((*ls)->next->item, item) == 0) {
			rv = (*ls)->next->item;
			tmp = (*ls)->next;
			(*ls)->next = (*ls)->next->next;
			ccg_free(tmp);
			return rv;
		}
	}
	return 0;
}

void ccg_list_destroy(const cls cls, list *ls) {
	node *prev, *curr;

	for (prev = 0, curr = ls; curr != 0; curr = curr->next) {
		if (prev) {
			if (cls)
				cls(prev->item);
			ccg_free(prev);
		}
		prev = curr;
	}
}
