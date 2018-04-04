#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sdf_kernel.h>
#include <sdf_integration.h>

int main() {
    SDFKernel *kernel;

    sdf_kernel_ini(&kernel);
    sdf_kernel_fin(kernel);
}
