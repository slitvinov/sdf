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

enum {KERN_CONF_OK, KERN_CONF_FAIL};
int kern_conf_a(SDFKernel *kernel,
                double a, double b,
                double x0, double y0, double z0, /**/ double *pA) {
    return KERN_CONF_OK;
}

int main() {
    SDFKernel *kernel;
    double a, b, A;
    double x0, y0, z0;

    ex = 32.0; ey = 32.0; ez = 32.0;
    cx = ex/2; cy = ey/2; cz = ez/2;
    a = -1; b = 2;
    x0 = cx;   y0 = cy; z0 = cz + 0.2*ez;

    sdf_kernel_ini(fx, fy, fz, I, &kernel);

    kern_conf_a(kernel, a, b, x0, y0, z0, /**/ &A);
    printf("%g\n", A);
}
