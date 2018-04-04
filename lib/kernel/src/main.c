#include <stdio.h>
#include <stdlib.h>
#include "sdf_memory.h"
#include "sdf_kernel.h"

struct SDFKernel {
    double ex, ey, ez; /* extent */
    int nx, ny, nz;
    float *D;
};

int sdf_kernel_ini(/**/ SDFKernel **pq) {
    SDFKernel *q;
    return KERNEL_OK;
}

int sdf_kernel_fin(SDFKernel *q) {
    EFREE(q);
    return KERNEL_OK;
}
