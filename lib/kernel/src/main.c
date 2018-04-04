#include <stdio.h>
#include <stdlib.h>
#include "sdf_memory.h"
#include "sdf_kernel.h"

struct SDFKernel {
    double x, y, z, cutoff;
    int rFlag, cutoffFlag;
};

int sdf_kernel_ini(/**/ SDFKernel **pq) {
    SDFKernel *q;
    EMALLOC(1, &q);
    q->rFlag = q->cutoffFlag = 0;
    *pq = q;
    return KERNEL_OK;
}

int sdf_kernel_fin(SDFKernel *q) {
    EFREE(q);
    return KERNEL_OK;
}

int sdf_kernel_xyz(SDFKernel *q, double x, double y, double z) {
    q->x = x; q->y = y; q->z = z;
    q->rFlag = 1;
}

int sdf_kernel_cutoff(SDFKernel *q, double cutoff) {
    q->cutoff = cutoff;
    q->cutoffFlag = 1;
}
