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

static double exp_minus(double x) {
    const double big = 10;
    if (x > big) return 0.0; else return exp(-x);
}
static double w0(double x, double y, double z) {
    double r;
    r = x*x + y*y + z*z;
    return exp_minus(r);
}
static double w(SDFKernel *q, double t) {
    double x, y, z, x0, y0, z0, dx, dy, dz, cutoff, I;
    cutoff = q->cutoff;
    I = q->I(t);
    x0 = q->fx(t); y0 = q->fy(t); z0 = q->fz(t);

    x = q->x; y = q->y; z = q->z;

    dx = x - x0; dy = y - y0; dz = z - z0;
    dx /= cutoff; dy /= cutoff; dz /= cutoff;
    return I*w0(dx, dy, dz);
}

static double dw(SDFKernel *q, double t) {
    double x, y, z, x0, y0, z0, dx, dy, dz, cutoff, I, w;
    double al, grad;
    cutoff = q->cutoff;
    I = q->I(t);
    x0 = q->fx(t); y0 = q->fy(t); z0 = q->fz(t);

    x = q->x; y = q->y; z = q->z;

    dx = x - x0; dy = y - y0; dz = z - z0;
    dx /= cutoff; dy /= cutoff; dz /= cutoff;

    w = w0(dx, dy, dz);
    al = 2*I/(cutoff * cutoff);
    grad  = al*w*sqrt(dx*dx + dy*dy + dz*dz);
    return grad;
}

static double dw20(double x, double y, double z) {
    double gx, gy, gz, gxx, gyy, gzz, gxy, gxz, gyz, ans;
    gx = -2*x; gy = -2*y; gz = -2*z;
    
    gxx = 2*(2*x*x-1); gxx /= 2;
    gyy = 2*(2*y*y-1); gyy /= 2;
    gzz = 2*(2*z*z-1); gzz /= 2;

    gxy = 4*x*y;
    gxz = 4*x*z;
    gyz = 4*y*z;

    ans = gz*gz*gzz+gy*gyz*gz+gx*gxz*gz+gy*gy*gyy+gx*gxy*gy+gx*gx*gxx;
    return ans;
}
static double dw2(SDFKernel *q, double t) {
    double x, y, z, x0, y0, z0, dx, dy, dz, cutoff, I, w;
    double al, grad;
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

    w = dw20(dx, dy, dz);
    al = I/(cutoff*cutoff*cutoff) * w0(dx, dy, dz);
    return al * w;
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

double sdf_kernel_dw(double t, void *vp) {
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
    return dw(q, t);
}

double sdf_kernel_dw2(double t, void *vp) {
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
    return dw2(q, t);
}
