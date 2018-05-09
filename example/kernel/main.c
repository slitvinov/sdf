#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_kernel.h>
#include <sdf_integration.h>

static double fx(double t) { return t;  };
static double fy(double t) { return t*t;  };
static double fz(double t) { return t*t*t;  };
static double  I(double t) { return 1000.0;};

int main() {
    double x, y, z, cutoff, t, dx, dy, dz;
    double dxx, dyy, dzz, dxy, dxz, dyz;
    double a, b;
    SDFIntegration *integration;
    SDFKernel *kernel;
    sdf_kernel_ini(fx, fy, fz, I, &kernel);

    cutoff = 2;
    x = 1; y = 2; z = 3;
    sdf_kernel_cutoff(kernel, cutoff);
    sdf_kernel_xyz(kernel, x, y, z);

    t = 2;
    dx = sdf_kernel_dx(t, kernel);
    dy = sdf_kernel_dy(t, kernel);
    dz = sdf_kernel_dz(t, kernel);
    printf("%g %g %g\n", dx, dy, dz);

    dxx = sdf_kernel_dxx(t, kernel);
    dyy = sdf_kernel_dyy(t, kernel);
    dzz = sdf_kernel_dzz(t, kernel);
    printf("%g %g %g\n", dxx, dyy, dzz);

    dxy = sdf_kernel_dxy(t, kernel);
    dxz = sdf_kernel_dxz(t, kernel);
    dyz = sdf_kernel_dyz(t, kernel);
    printf("%g %g %g\n", dxy, dxz, dyz);

    sdf_kernel_fin(kernel);
}
