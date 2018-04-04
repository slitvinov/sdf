#include <stdio.h>
#include <stdlib.h>
#include "sdf_memory.h"
#include "sdf_file.h"

struct SDFFile {
    int nx, ny, nz;
    float *D;
};

int sdf_file_ini(int nx, int ny, int nz, /**/ SDFFile **pq) {
    int n;
    SDFFile *q;
    EMALLOC(1, &q);
    n = nx * ny * nz;
    EMALLOC(n, &q->D);
    *pq = q;
    return FILE_OK;    
}

int sdf_file_fin(SDFFile *q) {
    EFREE(q->D);
    EFREE(q);
    return FILE_OK;
}
