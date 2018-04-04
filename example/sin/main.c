#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_integration.h>
#include <sdf_kernel.h>
#include <sdf_file.h>

#define PI (3.141592653589793)

double ex, ey, ez, cx, cy, cz;

static double sq(double x) { return x*x; }

static double fx(double t) { return ex * t; };
static double fy(double t) { return cy + sin(2*PI*t);};
static double fz(double t) { return cz;};
static double  I(double t) { return 1.0;};

int main() {
    SDFIntegration *integration;
    SDFKernel *kernel;
    SDFFile *file;
    int nx, ny, nz, n, i, s;
    double r, v, g, x, y, z;
    double cutoff, a, b, res;
    const char *o = "sdf.dat";

    nx = 60;  ny = 20;  nz = 20;
    ex = 6.0; ey = 2.0; ez = 2.0;
    cx = ex/2; cy = ey/2; cz = ez/2;
    a = -2.0; b = 2.0;
    cutoff = 2.0;
    
    sdf_file_ini(nx, ny, nz, ex, ey, ez, &file);
    sdf_file_n(file, &n);
    sdf_integration_ini(&integration);
    sdf_kernel_ini(fx, fy, fz, I, &kernel);
    sdf_kernel_cutoff(kernel, cutoff);
    
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
