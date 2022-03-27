#include <assert.h>
#include <stddef.h>

#include "adt.h"
#include "dict.h"
#include "list.h"
#include "mem.h"

static size_t primes[25] = {
		101,       251,       509,         1021,      2039,     4093,     8191,
		16381,     32749,     65521,       131071,    262139,   524287,   1048573,
		2097143,   4194301,   8388593,     16777213,  33554393, 67108859, 134217689,
		268435399, 536870909, 10737441789, 2147483647};

struct dict {
	size_t size;
	size_t tablen;
	hasher hash;
	comparer eq;
	finalizer fin;
	list **tab;
};

typedef struct item {
	void *key;
	void *val;
} item;

dict *ccg_dict_create(const hasher hf, const comparer eq, const finalizer fin) {
	dict *dt;

	dt = ccg_malloc(sizeof(dict));
	dt->size = 0;
	dt->tablen = primes[0];
	dt->hash = hf;
	dt->eq = eq;
	dt->fin = fin;
	dt->tab = 0;
	return dt;
}

void ccg_dict_put(void *key, void *val, const dict *dt) {
	item *obj;

	obj = ccg_malloc(sizeof(item));
	obj->key = key;
	obj->val = val;
	ccg_list_put_if_absent(obj, dt->eq, dt->tab[dt->hash(key, dt->tablen)]);
}

void *ccg_dict_find(const void *key, const dict *dt) {
	item obj, *rv;

	obj.key = (void *)key;
	obj.val = 0;
	rv = ccg_list_find(&obj, dt->eq, dt->tab[dt->hash(key, dt->tablen)]);
	return rv != 0 ? ((item *)rv)->val : 0;
}

void *ccg_dict_remove(const void *key, const dict *dt) {
	void *val;
	item obj, *rv;

	obj.key = (void *)key;
	obj.val = 0;
	rv = ccg_list_remove(&obj, dt->eq, &dt->tab[dt->hash(key, dt->tablen)]);
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
