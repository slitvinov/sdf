#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_file.h>

static double sq(double x) { return x*x; }

int main() {
    SDFFile *file;
    int nx, ny, nz, n, i, s;
    double ex, ey, ez, cx, cy, cz, r, v, g, x, y, z, volume;

    nx = 10; ny = 20; nz = 30;
    ex = 1; ey = 2; ez = 3;
    cx = ex/2; cy = ey/2; cz = cz/2;
    sdf_file_ini(nx, ny, nz, ex, ey, ez, &file);

    sdf_file_n(file, &n);
    sdf_file_volume(file, &volume);

    printf("n     : %d\n", n);
    printf("volume: %g\n", volume);


    for (i = 0; i < n; i++) {
        s = sdf_file_xyz(file, i, /**/ &x, &y, &z);
        if (s != FILE_OK) {
            fprintf(stderr, "error reading data\n");
            exit(2);
        }
        r = sq(x - cx) + sq(y - cy) + sq(z - cz);
        r = sqrt(r);
        v = r - 2.0;
        sdf_file_set(file, i,  v);
        sdf_file_get(file, i, &g);
        printf("%g\n", v - g);
    }
    sdf_file_fin(file);
}
