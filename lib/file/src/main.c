#include <stdio.h>
#include <stdlib.h>
#include "sdf_memory.h"
#include "sdf_file.h"

struct SDFFile {
    double ex, ey, ez; /* extent */
    int nx, ny, nz;
    float *D;
};

int sdf_file_ini(int nx, int ny, int nz,
                 double ex, double ey, double ez,
                 /**/ SDFFile **pq) {
    int n;
    SDFFile *q;
    EMALLOC(1, &q);
    n = nx * ny * nz;
    EMALLOC(n, &q->D);
    q->nx = nx; q->ny = ny; q->nz = nz;
    q->ex = ex; q->ey = ey; q->ez = ez;
    *pq = q;
    return FILE_OK;
}

int sdf_file_fin(SDFFile *q) {
    EFREE(q->D);
    EFREE(q);
    return FILE_OK;
}

int sdf_file_n(SDFFile *q, int *pn) {
    int nx, ny, nz, n;
    nx = q->nx; ny = q->ny; nz = q->nz;
    n = nx * ny * nz;
    *pn = n;
    return FILE_OK;
}

int sdf_file_volume(SDFFile *q, double *pvolume) {
    double ex, ey, ez, volume;
    ex = q->ex; ey = q->ey; ez = q->ez;
    volume = ex * ey * ez;
    *pvolume = volume;
    return FILE_OK;
}

int sdf_file_xyz(SDFFile *q, int i, double *px, double *py, double *pz) {
    int nx, ny, nz, n;
    int ix, iy, iz;
    double ex, ey, ez, x, y, z;
    nx = q->nx; ny = q->ny; nz = q->nz;
    ex = q->ex; ey = q->ey; ez = q->ez;
    n = nx * ny * nz;
    if (i < 0) {
        fprintf(stderr, "i=%d < 0\n", i);
        return FILE_SIZE;
    }
    if (i >= n) {
        fprintf(stderr, "i=%d >= n=%d\n", i, n);
        return FILE_SIZE;
    }

    iz = i/(nx*ny); i %= (nx*ny);
    iy = i/(nx)   ; i %= nx;
    ix = i;

    x = (ix + 0.5)*ex/nx;
    y = (iy + 0.5)*ey/ny;
    z = (iz + 0.5)*ez/nz;

    *px = x; *py = y; *pz = z;
    return FILE_OK;
}

int sdf_file_set(SDFFile *q, int i, double val) {
    int nx, ny, nz, n;
    float *D;
    nx = q->nx; ny = q->ny; nz = q->nz; D = q->D;
    n = nx * ny * nz;
    if (i < 0) {
        fprintf(stderr, "i=%d < 0\n", i);
        return FILE_SIZE;
    }
    if (i >= n) {
        fprintf(stderr, "i=%d >= n=%d\n", i, n);
        return FILE_SIZE;
    }
    D[i] = val;
    return FILE_OK;
}

int sdf_file_get(SDFFile *q, int i, double *pval) {
    int nx, ny, nz, n;
    double val;
    float *D;
    nx = q->nx; ny = q->ny; nz = q->nz; D = q->D;
    n = nx * ny * nz;
    if (i < 0) {
        fprintf(stderr, "i=%d < 0\n", i);
        return FILE_SIZE;
    }
    if (i >= n) {
        fprintf(stderr, "i=%d >= n=%d\n", i, n);
        return FILE_SIZE;
    }
    val = D[i];
    *pval = val;
    return FILE_OK;
}

int sdf_file_write(SDFFile *q, const char *path) {
    FILE *f;
    int nx, ny, nz, n;
    float ex, ey, ez, *D;
    nx = q->nx; ny = q->ny; nz = q->nz; D = q->D;
    ex = q->ex; ey = q->ey; ez = q->ez;
    n = nx*ny*nz;

    f = fopen(path, "w");
    if (f == NULL) {
        fprintf(stderr, "fail to open '%s'\n", path);
        return FILE_WRITE;
    }
    fprintf(f, "%g %g %g\n", ex, ey, ez);
    fprintf(f, "%d %d %d\n", nx, ny, nz);
    if (fwrite(D, sizeof(D[0]), n, f) != n) {
        fprintf(stderr, "fail to write to '%s'\n", path);
        return FILE_WRITE;        
    };
    if (fclose(f) != 0) {
        fprintf(stderr, "fail to close '%s'\n", path);
        return FILE_WRITE;
    };
    
    return FILE_OK;
}
