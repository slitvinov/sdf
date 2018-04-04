typedef struct SDFRoot SDFRoot;

typedef double (*SDFRootF) (double X, void *PARAMS);
enum {ROOT_OK, ROOT_FAIL};

int sdf_root_ini(SDFRoot**);
int sdf_root_fin(SDFRoot*);

int sdf_root_apply(SDFRoot*, SDFRootF, void *par, double lo, double hi, double *result);
