typedef struct SDFKernel SDFKernel;
typedef struct SDFKernelConf SDFKernelConf;

typedef double (*SDFKernelConfF) (double X, void *par);
enum {KERNEL_CONF_OK, KERNEL_CONF_FAIL};

int sdf_kernel_conf_ini(SDFKernel*, double a, double b,
                        double x0, double y0, double z0,
                        SDFKernelConf**);
int sdf_kernel_conf_fin(SDFKernelConf*);

int sdf_kernel_conf_cutoff(SDFKernelConf*, /**/ double *pcutoff);
