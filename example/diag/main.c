#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_integration.h>
#include <sdf_kernel.h>
#include <sdf_kernel_conf.h>
#include <sdf_file.h>
#include <sdf_root.h>

#define PI (3.141592653589793)

static double ex, ey, ez, cx, cy, cz;

static double fx0(double t) { return ex*t;}
static double fy0(double t) { return cy;}
static double fz0(double t) { return cz;};

static double fx1(double t) { return ex*t;}
static double fy1(double t) { return ey*t;}
static double fz1(double t) { return cz;};

static double I0(double t) { return 1.0;};
static double I1(double t) { return sqrt(2);};

static double fx(double t) { return t < 2 ? fx0(t) : fx1(t - 3); }
static double fy(double t) { return t < 2 ? fy0(t) : fy1(t - 3); }
static double fz(double t) { return t < 2 ? fz0(t) : fz1(t - 3); }
static double  I(double t) { return t < 2 ?  I0(t) :  I1(t - 3); }

int main() {
    SDFIntegration *integration;
    SDFKernel *kernel;
    SDFKernelConf *kernel_conf;
    SDFFile *file;
    int nx, ny, nz, n, i, s;
    double val, lo, hi, x, y, z, x0, y0, z0, A, C, cutoff;
    double a, b, res;
    const char *o = "sdf.dat";

    ex = ey = ez = 32;
    nx = ny = nz = 64;

    cx = ex/2; cy = ey/2; cz = ez/2;
    a = -1; b = 2 + 3;
    x0 = 0;   y0 = cy; z0 = cz + 0.1*ez;

    sdf_file_ini(nx, ny, nz, ex, ey, ez, &file);
    sdf_file_n(file, &n);
    sdf_integration_ini(&integration);
    sdf_kernel_ini(fx, fy, fz, I, &kernel);
    sdf_kernel_xyz(kernel, x0, y0, z0);

    sdf_kernel_conf_ini(kernel, a, b, /**/ &kernel_conf);
    lo = 4; hi = 20;
    sdf_kernel_conf_apply(kernel_conf, lo, hi, /**/ &cutoff, &A, &C);
    sdf_kernel_cutoff(kernel, cutoff);

    for (i = 0; i < n; i++) {
        s = sdf_file_xyz(file, i, /**/ &x, &y, &z);
        if (s != FILE_OK) {
            fprintf(stderr, "error reading data\n");
            exit(2);
        }
        sdf_kernel_xyz(kernel, x, y, z);
        sdf_integration_apply(integration, sdf_kernel_w, kernel, a, b, &res);
        val = A*res  - C;
        sdf_file_set(file, i,  val);
    }
    sdf_file_write(file, o);

    sdf_kernel_fin(kernel);
    sdf_integration_fin(integration);
    sdf_file_fin(file);
    sdf_kernel_conf_fin(kernel_conf);
}
