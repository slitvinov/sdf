#include <stdio.h>
#include <math.h>
#include <sdf_root.h>

double fun(double x, void *param) { return x*x - 2.0; }

int main() {
    double lo, hi, res;
    SDFRoot *root;
    SDFRootF f;
    
    f = fun; lo = 0; hi = 2;
    sdf_root_ini(&root);

    sdf_root_apply(root, f, NULL, lo, hi, &res);
    printf("%g\n", res);
    
    sdf_root_fin(root);
}
