#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_kernel.h>
#include <sdf_integration.h>

static double fx(double t) { return t;  };
static double fy(double t) { return t*t;  };
static double fz(double t) { return t*t*t;  };
static double  I(double t) { return 1.0;};

int main() {
    double x, y, z, cutoff, t;
    double a, b, res;
    SDFIntegration *integration;
    SDFIntegrationF f;
    SDFKernel *kernel;
    sdf_kernel_ini(fx, fy, fz, I, &kernel);
    sdf_integration_ini(&integration);

    cutoff = 2;
    x = 1; y = 2; z = 3;
    sdf_kernel_cutoff(kernel, cutoff);
    sdf_kernel_xyz(kernel, x, y, z);

    f = sdf_kernel_w;
    a = -1; b = 2;
    sdf_integration_apply(integration, f, kernel, a, b, &res);
    printf("g: %g\n", res);
    
    sdf_kernel_fin(kernel);
}
