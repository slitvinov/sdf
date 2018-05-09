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
    al = 2*I/cutoff;
    grad  = al*w*sqrt(dx*dx + dy*dy + dz*dz);
    return grad;
}

static void assert_q(SDFKernel *q) {
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
}

double sdf_kernel_w(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return w(q, t);
}

double sdf_kernel_dw(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return dw(q, t);
}

static double dwdr(SDFKernel *q, double t) {
    double x, y, z, x0, y0, z0, dx, dy, dz, cutoff, I, w;
    cutoff = q->cutoff;
    I = q->I(t);
    x0 = q->fx(t); y0 = q->fy(t); z0 = q->fz(t);
    x = q->x; y = q->y; z = q->z;
    dx = x - x0; dy = y - y0; dz = z - z0;
    dx /= cutoff; dy /= cutoff; dz /= cutoff;
    w = w0(dx, dy, dz);
    return - 2*I*w/(cutoff*cutoff);
}

static double dwdr2(SDFKernel *q, double t) {
    double x, y, z, x0, y0, z0, dx, dy, dz, cutoff, I, w;
    cutoff = q->cutoff;
    I = q->I(t);
    x0 = q->fx(t); y0 = q->fy(t); z0 = q->fz(t);
    x = q->x; y = q->y; z = q->z;
    dx = x - x0; dy = y - y0; dz = z - z0;
    dx /= cutoff; dy /= cutoff; dz /= cutoff;
    w = w0(dx, dy, dz);
    return I*w/(cutoff*cutoff*cutoff*cutoff);
}

static double drdx(SDFKernel *q, double t) {
    double r, r0;
    r0 = q->fx(t); r = q->x;
    return r - r0;
}

static double drdy(SDFKernel *q, double t) {
    double r, r0;
    r0 = q->fy(t); r = q->y;
    return r - r0;
}

static double drdz(SDFKernel *q, double t) {
    double r, r0;
    r0 = q->fz(t); r = q->z;
    return r - r0;
}

static double drdxx(SDFKernel *q, double t) {
    double cutoff, r, r0, dr;
    cutoff = q->cutoff;
    r0 = q->fx(t); r = q->x; dr = r - r0;
    return 2*(2*dr*dr - cutoff*cutoff);
}

static double drdyy(SDFKernel *q, double t) {
    double cutoff, r, r0, dr;
    cutoff = q->cutoff;
    r0 = q->fy(t); r = q->y; dr = r - r0;
    return 2*(2*dr*dr - cutoff*cutoff);
}

static double drdzz(SDFKernel *q, double t) {
    double cutoff, r, r0, dr;
    cutoff = q->cutoff;
    r0 = q->fz(t); r = q->z; dr = r - r0;
    return 2*(2*dr*dr - cutoff*cutoff);
}

double sdf_kernel_dx(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr(q, t) * drdx(q, t);
}

double sdf_kernel_dy(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr(q, t) * drdy(q, t);
}

double sdf_kernel_dz(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr(q, t) * drdz(q, t);
}

double sdf_kernel_dxx(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr2(q, t) * drdxx(q, t);
}

double sdf_kernel_dyy(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr2(q, t) * drdyy(q, t);
}

double sdf_kernel_dzz(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr2(q, t) * drdzz(q, t);
}

static double drdxy(SDFKernel *q, double t) {
    double u, u0, du, v, v0, dv;
    u0 = q->fx(t); u = q->x; du = u - u0;
    v0 = q->fy(t); v = q->y; dv = v - v0;
    return 4*du*dv;
}

static double drdxz(SDFKernel *q, double t) {
    double u, u0, du, v, v0, dv;
    u0 = q->fx(t); u = q->x; du = u - u0;
    v0 = q->fz(t); v = q->z; dv = v - v0;
    return 4*du*dv;
}

static double drdyz(SDFKernel *q, double t) {
    double u, u0, du, v, v0, dv;
    u0 = q->fy(t); u = q->y; du = u - u0;
    v0 = q->fz(t); v = q->z; dv = v - v0;
    return 4*du*dv;
}

double sdf_kernel_dxy(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr2(q, t) * drdxy(q, t);
}

double sdf_kernel_dxz(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr2(q, t) * drdxz(q, t);
}

double sdf_kernel_dyz(double t, void *vp) {
    SDFKernel *q;
    q = (SDFKernel*)vp;
    assert_q(q);
    return  dwdr2(q, t) * drdyz(q, t);
}
