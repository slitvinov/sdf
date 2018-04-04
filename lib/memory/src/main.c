#include <stdio.h>
#include <stdlib.h>
#include "sdf_memory.h"

void emalloc(size_t size, /**/ void **pq) {
    void *q;
    q = malloc(size);
    if (q == NULL) {
        fprintf(stderr, "emalloc failed (size=%ld\n", size);
        exit(2);
    }

    *pq = q;
}

void efree(void *ptr) {
    free(ptr);
    ptr = NULL;
}
