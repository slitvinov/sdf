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
static double amp_l, amp_w;

static double fx(double t) { return ex * t; }
static double fy(double t) { return cy + amp_l*sin(2*PI*t); }
static double fz(double t) { return cz;};
static double  I(double t) { return 1.0 + amp_w*sin(2*PI*t); }
static void ini_length() { amp_l = 4.0; amp_w = 0.0; }
static void ini_width()  { amp_l = 0.0; amp_w = 0.6; }

enum {LINE_OK, LINE_IO};
typedef double (*LineF) (double t);
static int line(LineF fx, LineF fy, LineF fz, double a, double b, long n, const char *path) {
    long i;
    double t, x, y, z;
    FILE *f;

    if ((f = fopen(path, "w")) == NULL) {
        fprintf(stderr, "fail to open file '%s'", path);
        return LINE_IO;
    }

    fprintf(f, "line\n");
    for (i = 0; i < n; i++) {
        t = a + (double)i/(n - 1) * (b - a);
        x = fx(t); y = fy(t); z = fz(t);
        fprintf(f, "%g,%g,%g\n", x, y, z);
    }
    if (fclose(f) != 0) {
        fprintf(stderr, "fail to close file '%s'", path);
        return LINE_IO;
    }
    return LINE_OK;
}

int main() {
    SDFIntegration *integration;
    SDFKernel *kernel;
    SDFKernelConf *kernel_conf;
    SDFFile *file;
    int nx, ny, nz, n, i, s;
    int status;
    double val, lo, hi, x, y, z, x0, y0, z0, A, C, cutoff;
    double a, b, res;
    const char *osdf  = "sdf.dat";
    const char *oline = "sdf.lines";

    ini_length();

    ex = 32.0; ey = 32.0; ez = 32.0;
    nx = ny = nz = 64;

    cx = ex/2; cy = ey/2; cz = ez/2;
    a = -1; b = 2;
    x0 = cx;   y0 = cy; z0 = cz + 0.2*ez;

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
    sdf_file_write(file, osdf);

    sdf_kernel_fin(kernel);
    sdf_integration_fin(integration);
    sdf_file_fin(file);
    sdf_kernel_conf_fin(kernel_conf);

    n = 100;
    status = line(fx, fy, fz, a, b, n, oline);
    if (status != LINE_OK) {
        fprintf(stderr, "fail to dump lines");
        exit(2);
    }
    
}
