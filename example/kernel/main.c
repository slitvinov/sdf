#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_kernel.h>
#include <sdf_integration.h>

static double fx(double t) { return t;  };
static double fy(double t) { return t*t;  };
static double fz(double t) { return t*t*t;  };
static double  I(double t) { return 10.0 * 2;};

int main() {
    double x, y, z, cutoff, t, w, dw;
    double a, b, res;
    SDFIntegration *integration;
    SDFKernel *kernel;
    sdf_kernel_ini(fx, fy, fz, I, &kernel);
    sdf_integration_ini(&integration);

    cutoff = 2;
    x = 1; y = 2; z = 3;
    sdf_kernel_cutoff(kernel, cutoff);
    sdf_kernel_xyz(kernel, x, y, z);

    a = -1; b = 2;
//    sdf_integration_apply(integration, sdf_kernel_w,  kernel, a, b, &res);
    sdf_integration_apply(integration, sdf_kernel_dw, kernel, a, b, &res);
    printf("g: %g\n", res);

    t = 2;
    w  = sdf_kernel_w (t, kernel);
    dw = sdf_kernel_dw(t, kernel);
    printf("g: %g %g\n", w, dw);

    sdf_kernel_fin(kernel);
}
