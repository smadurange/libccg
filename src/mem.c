#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

#undef malloc
#undef calloc
#undef realloc
#undef free

void *ccg_malloc(size_t size) {
	void *ptr;
	ptr = malloc(size);
	if (ptr == NULL) {
		perror("ccg_malloc: \n");
	}
	return ptr;
}

void *ccg_calloc(int nmemb, size_t size) { return calloc(nmemb, size); }

void *ccg_realloc(void *ptr, size_t size) { return realloc(ptr, size); }

void ccg_free(void *ptr) { free(ptr); }