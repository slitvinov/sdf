#include <stdio.h>
#include <stdlib.h>
#include "sdf_memory.h"

void emalloc(int size, /**/ void **pq) {
    void *q;
    q = malloc(size);
    if (q == NULL) {
        fprintf(stderr, "emalloc failed (size=%d\n", size);
        exit(2);
    }
    *pq = q;
}

void efree(void *q) {
    free(q);
    q = NULL;
}
