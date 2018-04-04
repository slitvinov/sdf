#include <stdio.h>
#include <sdf_memory.h>

int main() {
    int *p;
    EMALLOC(1, &p);
    *p = 10;
    fprintf(stderr, "p = %d\n", *p);
    EFREE(p);
}
