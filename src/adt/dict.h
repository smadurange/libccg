#ifndef CCG_DICT_H
#define CCG_DICT_H

#include <stddef.h>

#include "adt.h"

typedef struct dict dict;
typedef int (*hash)(const void *, size_t n);

dict *ccg_dict_create(const hash hf, const cmp cmp, const cls cls);
void ccg_dict_put(void *key, void *val, const dict *dt);
void *ccg_dict_find(const void *key, const dict *dt);
void *ccg_dict_remove(const void *key, const dict *dt);
void ccg_dict_destroy(dict *dt);

#endif
