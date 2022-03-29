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
	size_t keyc;
	size_t tablen;
	hasher hash;
	comparer cmp;
	finalizer fin;
	item **tab;
};

struct item {
	void *key;
	void *val;
	item *next;
};

dict *ccg_dict_create(const hasher hf, const comparer cmp,
											const finalizer fin) {
	dict *dt;

	dt = ccg_malloc(sizeof(dict));
	dt->keyc = 0;
	dt->tablen = primes[0];
	dt->hash = hf;
	dt->cmp = cmp;
	dt->fin = fin;
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
	void *val;
	item obj, *rv;

	obj.key = (void *)key;
	obj.val = 0;
	rv = ccg_list_remove(&obj, dt->cmp, &dt->tab[dt->hash(key, dt->tablen)]);
	if (rv != 0) {
		val = ((item *)rv)->val;
		ccg_free(rv);
		return val;
	}
	return 0;
}

void ccg_dict_destroy(dict *dt) {
	int i;

	for (i = 0; i < dt->tablen; i++)
		ccg_list_destroy(dt->fin, dt->tab[i]);
	ccg_free(dt);
}
