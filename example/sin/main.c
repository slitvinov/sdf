#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_integration.h>
#include <sdf_kernel.h>
#include <sdf_file.h>
#include <sdf_root.h>

#define PI (3.141592653589793)

static double ex, ey, ez, cx, cy, cz;
static double amp = 0.0;

static double sq(double x) { return x*x; }

static double fx(double t) { return ex * t; }
static double fy(double t) { return cy + amp*sin(2*PI*t);};
static double fz(double t) { return cz;};
static double  I(double t) { return 1.0;};

typedef struct Kernel {
    SDFKernel *kernel; /* inout */
    SDFIntegration *integration;
    double a, b;
} Kernel;

static double kern_fun(double rev_cutoff, void *vp) {
    Kernel *q;
    SDFKernel *kernel;
    double a, b, w, cutoff;
    SDFIntegration *integration;
    q = (Kernel*)vp;
    cutoff = 1/rev_cutoff;
    integration = q->integration; kernel = q->kernel;
    a = q->a; b = q->b;
    sdf_kernel_cutoff(kernel, cutoff);
    sdf_integration_apply(integration,
                          sdf_kernel_dw, kernel,
                          a, b,
                          &w);
    printf("cutoff w : %g %g\n", cutoff, w);
    return w*w - 1e-5;
}
static void kern_conf(Kernel *q, double x, double y, double z, /**/ double *palpha, double *pbeta) {
    double lo, hi, cutoff, alpha, beta, a, b, rev_cutoff;
    SDFIntegration *integration;
    SDFKernel *kernel;
    SDFRoot *root;
    integration = q->integration; kernel = q->kernel;
    a = q->a; b = q->b;
    sdf_root_ini(&root);
    lo = 1/10.0; hi = 1/0.1;
    sdf_kernel_xyz(kernel, x, y, z);
    sdf_root_apply(root, kern_fun, q, lo, hi, &rev_cutoff);
    cutoff = 1/rev_cutoff;
    sdf_kernel_cutoff(kernel, cutoff);
    printf("cutoff = %g\n", cutoff);
    sdf_root_fin(root);
    sdf_integration_apply(integration,
                          sdf_kernel_w, kernel,
                          a, b,
                          &alpha);
    sdf_integration_apply(integration,
                          sdf_kernel_dw, kernel,
                          a, b,
                          &beta);
    printf("alpha, beta : %g %g\n", alpha, beta);
    *palpha = alpha; *pbeta = beta;
}

int main() {
    SDFIntegration *integration;
    Kernel K;
    SDFKernel *kernel;
    SDFFile *file;
    int nx, ny, nz, n, i, s;
    double r, v, g, x, y, z, x0, y0, z0, alpha, beta, cutoff, val;
    double a, b, res;
    const char *o = "sdf.dat";

    ex = 32.0; ey = 32.0; ez = 32.0;
    nx = 64;   ny = 64;  nz = 64;

    cx = ex/2; cy = ey/2; cz = ez/2;
    a = -1; b = 2;
    x0 = 0;   y0 = 0; z0 = cz + 0.4*ez;

    sdf_file_ini(nx, ny, nz, ex, ey, ez, &file);
    sdf_file_n(file, &n);
    sdf_integration_ini(&integration);
    sdf_kernel_ini(fx, fy, fz, I, &kernel);

    K.kernel = kernel; K.a = a; K.b = b; K.integration = integration;
    kern_conf(&K, x0, y0, z0, /**/ &alpha, &beta);
//    cutoff = 4.0; sdf_kernel_cutoff(kernel, cutoff);

    for (i = 0; i < n; i++) {
        s = sdf_file_xyz(file, i, /**/ &x, &y, &z);
        if (s != FILE_OK) {
            fprintf(stderr, "error reading data\n");
            exit(2);
        }
        sdf_kernel_xyz(kernel, x, y, z);
        sdf_integration_apply(integration,
                              sdf_kernel_w, kernel,
                              a, b,
                              &res);
        sdf_file_set(file, i,  res);
        val = alpha/beta - res/beta;
        sdf_file_set(file, i,  val);
        printf("%g %g %g %g\n", x, y, z, val);
    }
    sdf_file_write(file, o);

    sdf_kernel_fin(kernel);
    sdf_integration_fin(integration);
    sdf_file_fin(file);
}
