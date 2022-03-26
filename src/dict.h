#ifndef CCG_DICT_H
#define CCG_DICT_H

typedef struct dict dict;
typedef int (*hashcode)(const void *);
typedef int (*comparer)(const void *, const void *);

dict *ccg_dict_create(const hashcode hc, const comparer eq);
void ccg_dict_put(void *key, void *val, const dict *dt);
void *ccg_dict_find(const void *key, const dict *dt);
void *ccg_dict_remove(const void *key, const dict *dt);
void ccg_dict_destroy(dict *dt);

#endif