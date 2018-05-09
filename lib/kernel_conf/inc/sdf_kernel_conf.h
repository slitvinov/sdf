typedef struct SDFKernel SDFKernel;
typedef struct SDFKernelConf SDFKernelConf;

typedef double (*SDFKernelConfF) (double X, void *par);
enum {KERNEL_CONF_OK, KERNEL_CONF_FAIL};

int sdf_kernel_conf_ini(SDFKernel*, double a, double b,
                        SDFKernelConf**);
int sdf_kernel_conf_fin(SDFKernelConf*);

int sdf_kernel_conf_cutoff(SDFKernelConf*, double lo, double hi, /**/ double *pcutoff);
int sdf_kernel_conf_a(SDFKernelConf*, /**/ double *pa);

int sdf_kernel_conf_apply(SDFKernelConf*, double lo, double hi, /**/ double *pcutoff, double *pA, double *pC);
