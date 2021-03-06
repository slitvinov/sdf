typedef struct SDFKernel SDFKernel;
typedef double (*SDFKernelF) (double t);
enum {KERNEL_OK, KERNEL_FAIL};
int sdf_kernel_ini(SDFKernelF fx, SDFKernelF fy, SDFKernelF fz, SDFKernelF I, /**/ SDFKernel**);
int sdf_kernel_fin(SDFKernel*);

int sdf_kernel_xyz(SDFKernel*, double x, double y, double z);
int sdf_kernel_cutoff(SDFKernel*, double cutoff);

/* for gsl */
double sdf_kernel_w (double t, void *kernel);
double sdf_kernel_dw(double t, void *kernel);

double sdf_kernel_dx(double t, void *kernel);
double sdf_kernel_dy(double t, void *kernel);
double sdf_kernel_dz(double t, void *kernel);

double sdf_kernel_dxx(double t, void *kernel);
double sdf_kernel_dyy(double t, void *kernel);
double sdf_kernel_dzz(double t, void *kernel);

double sdf_kernel_dxy(double t, void *kernel);
double sdf_kernel_dxz(double t, void *kernel);
double sdf_kernel_dyz(double t, void *kernel);
