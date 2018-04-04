#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_kernel.h>
#include <sdf_integration.h>

static double fx(double t) { return t;  };
static double fy(double t) { return 2*t;  };
static double fz(double t) { return 3*t;  };
static double  I(double t) { return 4.0;};

int main() {
    double x, y, z, cutoff;
    SDFKernel *kernel;
    sdf_kernel_ini(fx, fy, fz, I, &kernel);

    cutoff = 2;
    sdf_kernel_cutoff(kernel, cutoff);
    sdf_kernel_xyz(kernel, x, y, z);
    
    sdf_kernel_fin(kernel);
}
