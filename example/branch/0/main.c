#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_integration.h>
#include <sdf_kernel.h>
#include <sdf_kernel_conf.h>
#include <sdf_file.h>

#define PI (3.141592653589793)

#define r_small (1.0)
#define r_big   (r_small/9*12.5)

static double r_hi = r_small;
static double r_lo = r_big;

static double ex, ey, ez, cx, cy, cz;

static double fx0(double t) { return cx*t;}
static double fy0(double t) { return cy;}
static double fz0(double t) { return cz;};

static double fx1(double t) { return cx + cx*t;}
static double fy1(double t) { return cy + cy*t;}
static double fz1(double t) { return cz;};

static double fx2(double t) { return cx + cx*t;}
static double fy2(double t) { return cy - cy*t;}
static double fz2(double t) { return cz;};

static double fx(double t) { return t < 1 ? fx0(t) : t < 3 ? fx1(t - 1) : fx2(t - 3); }
static double fy(double t) { return t < 1 ? fy0(t) : t < 3 ? fy1(t - 1) : fy2(t - 3); }
static double fz(double t) { return t < 1 ? fz0(t) : t < 3 ? fz1(t - 1) : fz2(t - 3); }

static double I(double t) { return t < 1 ? r_small : t < 3 ? sqrt(2)*r_hi : sqrt(2)*r_lo; }

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
