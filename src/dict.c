#include <assert.h>
#include <stddef.h>

#include "adt.h"
#include "dict.h"
#include "mem.h"

static size_t primes[25] = {
	101,       251,       509,         1021,      2039,     4093,     8191,
	16381,     32749,     65521,       131071,    262139,   524287,   1048573,
	2097143,   4194301,   8388593,     16777213,  33554393, 67108859, 134217689,
	268435399, 536870909, 10737441789, 2147483647};

typedef struct item item;

struct dict {
	size_t size;
	size_t tablen;
	hash hash;
	cmp cmp;
	cls cls;
	item **tab;
};

struct item {
	void *key;
	void *val;
	item *next;
};

dict *ccg_dict_create(const hash hf, const cmp cmp, const cls cls) {
	dict *dt;

	dt = ccg_malloc(sizeof(dict));
	dt->size = 0;
	dt->tablen = primes[0];
	dt->hash = hf;
	dt->cmp = cmp;
	dt->cls = cls;
	dt->tab = ccg_calloc(primes[0], sizeof(item *));
	return dt;
}

void *ccg_dict_find(const void *key, const dict *dt) {
	item *kv;

	for (kv = dt->tab[(dt->hash(key, dt->tablen))]; kv != 0; kv = kv->next)
		if (dt->cmp(key, kv->key) == 0)
			return kv->val;
	return 0;
}

void ccg_dict_put(void *key, void *val, const dict *dt) {
	int i;
	item *kv;

	// todo: resize
	if (!ccg_dict_find(key, dt)) {
		kv = ccg_malloc(sizeof(item));
		kv->key = key;
		kv->val = val;
		kv->next = dt->tab[(i = dt->hash(key, dt->tablen))];
		dt->tab[i] = kv;
	}
}

void *ccg_dict_remove(const void *key, const dict *dt) {
	int i;
	void *rv;
	item *p, *c;

	for (p = 0, c = dt->tab[(i = dt->hash(key, dt->tablen))]; c; c = c->next) {
		if (dt->cmp(key, c->key) == 0) {
			rv = c->val;
			if (!p)
				dt->tab[i] = c->next;
			else
				p->next = c->next;
			ccg_free(c);
			return rv;
		}
		p = c;
	}
	return 0;
}

void ccg_dict_destroy(dict *dt) {
	int i;
	item *p, *c;

	for (i = 0; i < dt->size; i++) {
		for (p = 0, c = dt->tab[i]; c; c = c->next) {
			if (p) {
				if (dt->cls)
					dt->cls(p->val);
				ccg_free(p);
			}
			p = c;
		}
	}
}
