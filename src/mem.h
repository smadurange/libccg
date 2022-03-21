#ifndef CCG_MEM_H
#define CCG_MEM_H

#include <stdlib.h>

void *ccg_malloc(size_t size);
void *ccg_calloc(int nmemb, size_t size);
void *ccg_realloc(void *ptr, size_t size);
void ccg_free(void *ptr);

#undef malloc
#undef calloc
#undef realloc
#undef free

#define malloc(s) use_cg_malloc_instead
#define calloc(n, s) use_cg_calloc_instead
#define realloc(p, s) use_cg_realloc_instead
#define free(p) use_cg_free_instead

#endif