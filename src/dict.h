#ifndef CCG_DICT_H
#define CCG_DICT_H

#include <stddef.h>

#include "adt.h"

typedef struct dict dict;
typedef int (*hasher)(const void *, const size_t n);

dict *ccg_dict_create(const hasher hf, const comparer eq, const finalizer fin);
void ccg_dict_put(void *key, void *val, const dict *dt);
void *ccg_dict_find(const void *key, const dict *dt);
void *ccg_dict_remove(const void *key, const dict *dt);
void ccg_dict_destroy(dict *dt);

#endif