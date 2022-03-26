#include <stddef.h>

#include "dict.h"
#include "list.h"
#include "mem.h"

#define TABLEN 100

struct dict {
  size_t size;
  size_t tablen;
  hasher hash;
  comparer eq;
  list **tab;
};

typedef struct item {
  void *key;
  void *val;
} item;

dict *ccg_dict_create(const hasher hf, const comparer eq) {
  dict *dt;

  dt = ccg_malloc(sizeof(dict));
  dt->size = 0;
  dt->tablen = TABLEN;
  dt->hash = hf;
  dt->eq = eq;
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
    ccg_list_destroy(dt->tab[i]);
  ccg_free(dt);
}