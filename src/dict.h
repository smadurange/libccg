#ifndef CCG_DICT_H
#define CCG_DICT_H

#include <stddef.h>

typedef struct dict dict;
typedef int (*hash)(const void *, const size_t n);
typedef int (*comparer)(const void *, const void *);

dict *ccg_dict_create(const hash hf, const comparer eq);
void ccg_dict_put(void *key, void *val, const dict *dt);
void *ccg_dict_find(const void *key, const dict *dt);
void *ccg_dict_remove(const void *key, const dict *dt);
void ccg_dict_destroy(dict *dt);

#endif