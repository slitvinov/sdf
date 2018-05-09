typedef struct SDFKernelConf SDFKernelConf;

typedef double (*SDFKernelConfF) (double X, void *par);
enum {KERNEL_CONF_OK, KERNEL_CONF_FAIL};

int sdf_kernel_conf_ini(SDFKernelConf**);
int sdf_kernel_conf_fin(SDFKernelConf*);

int sdf_kernel_conf_apply(SDFKernelConf*, SDFKernelConfF, void *par, double lo, double hi, double *result);
