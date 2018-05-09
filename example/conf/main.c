#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_integration.h>
#include <sdf_kernel.h>
#include <sdf_root.h>

#define PI (3.141592653589793)

static double ex, ey, ez, cx, cy, cz;
static double amp_l = 0.0;
static double amp_w = 0.4;

static double fx(double t) { return ex * t; }
static double fy(double t) {
    return cy + amp_l*sin(2*PI*t);
};
static double fz(double t) { return cz;};
static double  I(double t) {
    return 1.0 + amp_w*sin(2*PI*t);
};

static double dbl(int *pc, const char **pv[]) {
    double x;
    int c;
    const char **v;
    v = *pv; c = *pc;
    if (c < 2) {
        fprintf(stderr, "needs an argument\n");
        exit(2);
    }

    if (sscanf(v[1], "%lf", &x) != 1) {
        fprintf(stderr, "fail to read dobule\n");
        exit(2);
    }
    c--; v++;

    *pc = c; *pv = v;
    return x;
}

enum {KERN_CONF_OK, KERN_CONF_FAIL};
static int kern_conf_a(SDFKernel *kernel, double cutoff,
                double a, double b,
                double x0, double y0, double z0, /**/ double *px) {
    SDFIntegration *integration;
    double ux, uy, uz, x;

    sdf_integration_ini(&integration);
    sdf_kernel_cutoff(kernel, cutoff);

    sdf_kernel_xyz(kernel, x0, y0, z0);
    sdf_integration_apply(integration, sdf_kernel_dx, kernel, a, b, &ux);
    sdf_integration_apply(integration, sdf_kernel_dy, kernel, a, b, &uy);
    sdf_integration_apply(integration, sdf_kernel_dz, kernel, a, b, &uz);
    sdf_integration_fin(integration);
    x = 1/sqrt(ux*ux + uy*uy + uz*uz);
    *px = x;
    return KERN_CONF_OK;
}

static int kern_conf_b(SDFKernel *kernel, double cutoff,
                double a, double b,
                double x0, double y0, double z0, /**/ double *px) {
    SDFIntegration *integration;
    double ux, uy, uz;
    double uxx, uyy, uzz;
    double uxy, uxz, uyz;
    double num, den, x;

    sdf_integration_ini(&integration);
    sdf_kernel_cutoff(kernel, cutoff);

    sdf_kernel_xyz(kernel, x0, y0, z0);
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
    sdf_integration_fin(integration);

    *px = x;
    return KERN_CONF_OK;
}

int main(int argc, const char *argv[]) {
    SDFKernel *kernel;
    double a, b, A, B;
    double x0, y0, z0, cutoff;

    cutoff = dbl(&argc, &argv);

    ex = 32.0; ey = 32.0; ez = 32.0;
    cx = ex/2; cy = ey/2; cz = ez/2;
    a = -1; b = 2;
    x0 = cx;   y0 = cy; z0 = cz + 0.2*ez;

    sdf_kernel_ini(fx, fy, fz, I, &kernel);

    kern_conf_a(kernel, cutoff, a, b, x0, y0, z0, /**/ &A);
    kern_conf_b(kernel, cutoff, a, b, x0, y0, z0, /**/ &B);
    
    printf("%g %g\n", cutoff, B);
}
