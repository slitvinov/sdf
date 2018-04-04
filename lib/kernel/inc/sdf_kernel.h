typedef struct SDFKernel SDFKernel;
enum {KERNEL_OK, KERNEL_FAIL};
int sdf_kernel_ini(/**/ SDFKernel**);
int sdf_kernel_fin(SDFKernel*);
