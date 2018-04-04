#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sdf_memory.h"
#include "sdf_kernel.h"

static const int MAGIC = 42;
struct SDFKernel {
    double x, y, z, cutoff;
    int rFlag, cutoffFlag;

    SDFKernelF fx, fy, fz, I;
    int stamp;
};

int sdf_kernel_ini(SDFKernelF fx, SDFKernelF fy, SDFKernelF fz, SDFKernelF I, /**/ SDFKernel **pq) {
    SDFKernel *q;
    EMALLOC(1, &q);
    q->rFlag = q->cutoffFlag = 0;
    q->fx = fx; q->fy = fy; q->fz = fz; q->I = I;
    q->stamp = MAGIC;
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
    return KERNEL_OK;
}

int sdf_kernel_cutoff(SDFKernel *q, double cutoff) {
    if (cutoff <= 0) {
        fprintf(stderr, "cutoff=%g <= 0\n", cutoff);
        return KERNEL_FAIL;
    }
    q->cutoff = cutoff;
    q->cutoffFlag = 1;
    return KERNEL_OK;
}

static double w0(double x, double y, double z) {
    double r;
    r = x*x + y*y + z*z;
    return exp(-r);
}
static double w(SDFKernel *q, double t) {
    double x, y, z, x0, y0, z0, dx, dy, dz, cutoff, I;
    cutoff = q->cutoff;
    I = q->I(t);
    x0 = q->fx(t);
    y0 = q->fy(t);
    z0 = q->fz(t);

    x = q->x;
    y = q->y;
    z = q->z;

    dx = x - x0; dy = y - y0; dz = z - z0;
    dx /= cutoff; dy /= cutoff; dz /= cutoff;
    
    return I*w0(dx, dy, dz);
}

double sdf_kernel_w(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    if (q->stamp != MAGIC) {
        fprintf(stderr, "q->stamp != MAGIC\n");
        exit(2);
    }

    if (!q->cutoffFlag) {
        fprintf(stderr, "cutoff is not set\n");
        exit(2);
    }

    if (!q->rFlag) {
        fprintf(stderr, "x, y, z are not set\n");
        exit(2);
    }

    return w(q, t);
}
