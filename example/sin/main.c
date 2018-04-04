#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_integration.h>
#include <sdf_kernel.h>
#include <sdf_file.h>
#include <sdf_root.h>

#define PI (3.141592653589793)

double ex, ey, ez, cx, cy, cz;

static double sq(double x) { return x*x; }

static double fx(double t) { return ex * t; };
static double fy(double t) { return cy + sin(2*PI*t);};
static double fz(double t) { return cz;};
static double  I(double t) { return 1.0;};

typedef struct Kernel {
    SDFKernel *kernel; /* inout */
    SDFIntegration *integration;
    double a, b;
} Kernel;

static double kern_fun(double cutoff, void *vp) {
    Kernel *q;
    SDFKernel *kernel;
    double a, b, grad;
    SDFIntegration *integration;
    q = (Kernel*)vp;
    integration = q->integration; kernel = q->kernel;
    a = q->a; b = q->b;
    sdf_kernel_cutoff(kernel, cutoff);
    sdf_integration_apply(integration,
                          sdf_kernel_dw, kernel,
                          a, b,
                          &grad);
    printf("cutoff grad : %g %g\n", cutoff, grad);
    return grad - 1.0;
}
static void kern_conf(Kernel *k, double x, double y, double z, /**/ double *pC) {
    double lo, hi, cutoff, C;
    SDFKernel *kernel;
    SDFRoot *root;

    kernel = k->kernel;
    sdf_root_ini(&root);
    lo = 0.1; hi = 1000.0;
    sdf_kernel_xyz(kernel, x, y, z);
    sdf_root_apply(root, kern_fun, k, lo, hi, &cutoff);
    
    printf("cutoff = %g\n", cutoff);
    
    sdf_root_fin(root);

    *pC = C;
}

int main() {
    SDFIntegration *integration;
    Kernel K;
    SDFKernel *kernel;
    SDFFile *file;
    int nx, ny, nz, n, i, s;
    double r, v, g, x, y, z, x0, y0, z0, C;
    double cutoff, a, b, res;
    const char *o = "sdf.dat";


    nx = 60;  ny = 20;  nz = 20;
    ex = 6.0; ey = 2.0; ez = 2.0;
    cx = ex/2; cy = ey/2; cz = ez/2;
    a = -2.0; b = 2.0;
    x0 = y0 = 0; z0 = cz + 1.0;
    cutoff = 2.0;

    sdf_file_ini(nx, ny, nz, ex, ey, ez, &file);
    sdf_file_n(file, &n);
    sdf_integration_ini(&integration);
    sdf_kernel_ini(fx, fy, fz, I, &kernel);

    K.kernel = kernel; K.a = a; K.b = b; K.integration = integration;
    kern_conf(&K, x0, y0, z0, /**/ &C);

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
    }
    sdf_file_write(file, o);

    sdf_kernel_fin(kernel);
    sdf_integration_fin(integration);
    sdf_file_fin(file);
}
