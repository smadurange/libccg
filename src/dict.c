#include <stddef.h>

#include "dict.h"
#include "list.h"
#include "mem.h"

#define TABLEN 100

struct dict {
  size_t size;
  size_t tablen;
  hash hash;
  comparer eq;
  list **tab;
};

dict *ccg_dict_create(const hash hf, const comparer eq) {
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
  ccg_list_put_if_absent(val, dt->eq, dt->tab[dt->hash(key, dt->tablen)]);
}