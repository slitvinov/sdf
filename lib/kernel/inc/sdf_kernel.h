typedef struct SDFKernel SDFKernel;
typedef double (*SDFKernelF) (double t);
enum {KERNEL_OK, KERNEL_FAIL};
int sdf_kernel_ini(/**/ SDFKernel**, SDF);
int sdf_kernel_fin(SDFKernel*);

int sdf_kernel_xyz(SDFKernel*, double x, double y, double z);
int sdf_kernel_cutoff(SDFKernel*, double cutoff);
