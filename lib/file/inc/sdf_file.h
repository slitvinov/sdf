typedef struct SDFFile SDFFile;

enum {FILE_OK, FILE_SIZE, FILE_WRITE};
int sdf_file_ini(int nx, int ny, int nz,
                 double ex, double ey, double ez,
                 /**/ SDFFile**);
int sdf_file_fin(SDFFile*);

int sdf_file_n       (SDFFile*, /**/ int *n);
int sdf_file_volume  (SDFFile*, /**/ double *volume);

int sdf_file_xyz(SDFFile*, int i, /**/ double *x, double *y, double *z);
int sdf_file_set(SDFFile*, int i,      double  val);
int sdf_file_get(SDFFile*, int i, /**/ double *val);

int sdf_file_write(SDFFile*, const char *path);
