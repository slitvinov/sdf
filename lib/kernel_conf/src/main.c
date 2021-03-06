#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <sdf_memory.h>
#include <sdf_root.h>
#include <sdf_integration.h>
#include <sdf_kernel.h>

#include "sdf_kernel_conf.h"

struct SDFKernelConf {
    SDFKernel *kernel;
    double a, b;
    SDFIntegration *integration;
};

int sdf_kernel_conf_ini(SDFKernel *kernel,
                        double a, double b,
                        SDFKernelConf **pq) {
    SDFKernelConf *q;
    EMALLOC(1, &q);
    q->kernel = kernel; q->a = a; q->b = b;

    sdf_integration_ini(&q->integration);

    *pq = q;
    return KERNEL_CONF_OK;
}

int sdf_kernel_conf_fin(SDFKernelConf *q) {
    sdf_integration_fin(q->integration);
    EFREE(q);
    return KERNEL_CONF_OK;
}

static double fun(double cutoff, void *param) {
    SDFKernelConf *q;
    SDFIntegration *integration;
    SDFKernel *kernel;
    double a, b;
    double ux, uy, uz;
    double uxx, uyy, uzz;
    double uxy, uxz, uyz;
    double num, den, x;

    q = (SDFKernelConf*)param;

    a = q->a; b = q->b;

    kernel = q->kernel;
    integration = q->integration;
    sdf_kernel_cutoff(kernel, cutoff);

    sdf_integration_apply(integration, sdf_kernel_dx, kernel, a, b, &ux);
    sdf_integration_apply(integration, sdf_kernel_dy, kernel, a, b, &uy);
    sdf_integration_apply(integration, sdf_kernel_dz, kernel, a, b, &uz);

    sdf_integration_apply(integration, sdf_kernel_dxx, kernel, a, b, &uxx);
    sdf_integration_apply(integration, sdf_kernel_dyy, kernel, a, b, &uyy);
    sdf_integration_apply(integration, sdf_kernel_dzz, kernel, a, b, &uzz);

    sdf_integration_apply(integration, sdf_kernel_dxy, kernel, a, b, &uxy);
    sdf_integration_apply(integration, sdf_kernel_dxz, kernel, a, b, &uxz);
    sdf_integration_apply(integration, sdf_kernel_dyz, kernel, a, b, &uyz);

    num = pow(uz,2)*uzz+2*uy*uyz*uz+2*ux*uxz*uz+pow(uy,2)*uyy+2*ux*uxy*uy+pow(ux,2)*uxx;
    den = 2*sqrt(pow(uz,2)+pow(uy,2)+pow(ux,2))
        *(pow(uy,2)*pow(uz,2)+pow(uz,2)+pow(uy,4)+pow(ux,2)*pow(uy,2)+pow(ux,2));
    x = num/den;
    return x;
}

int sdf_kernel_conf_cutoff(SDFKernelConf *q, double lo, double hi, /**/ double *pcutoff) {
    int r;
    double cutoff;
    SDFRoot *root;
    sdf_root_ini(&root);
    r = sdf_root_apply(root, fun, q, lo, hi, /**/ &cutoff);
    if (r != ROOT_OK) {
        fprintf(stderr, "fail to find root\n");
        exit(2);
    }
    sdf_root_fin(root);
    *pcutoff = cutoff;
    return KERNEL_CONF_OK;
}

int sdf_kernel_conf_a(SDFKernelConf *q, /**/ double *px) {
    double x, a, b, ux, uy, uz;
    SDFIntegration *integration;
    SDFKernel *kernel;

    integration = q->integration;
    kernel = q->kernel;
    a = q->a; b = q->b;

    sdf_integration_apply(integration, sdf_kernel_dx, kernel, a, b, &ux);
    sdf_integration_apply(integration, sdf_kernel_dy, kernel, a, b, &uy);
    sdf_integration_apply(integration, sdf_kernel_dz, kernel, a, b, &uz);

    x = 1/sqrt(ux*ux + uy*uy + uz*uz);

    *px = x;
    return KERNEL_CONF_OK;
}

static int w(SDFKernelConf *q, double cutoff, double *px) {
    double x, a, b;
    SDFIntegration *integration;
    SDFKernel *kernel;

    integration = q->integration;
    kernel = q->kernel;
    a = q->a; b = q->b;
    sdf_kernel_cutoff(kernel, cutoff);
    sdf_integration_apply(integration, sdf_kernel_w, kernel, a, b, /**/ &x);
    *px = x;

    return KERNEL_CONF_OK;
}

int sdf_kernel_conf_apply(SDFKernelConf *q, double lo, double hi, /**/ double *pcutoff, double *pA, double *pC) {
    double cutoff, A, C, val;

    sdf_kernel_conf_cutoff(q, lo, hi, /**/ &cutoff);
    sdf_kernel_conf_a(q, /**/ &A);
    w(q, cutoff, /**/ &val);
    C = A * val;
    *pcutoff = cutoff; *pA = A; *pC = C;

    return KERNEL_CONF_OK;
}
