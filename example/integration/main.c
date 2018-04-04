#include <stdio.h>
#include <math.h>
#include <sdf_integration.h>

double fsin(double x, void *pv) {
    double *pd, w;
    pd = (double*)pv;
    w = *pd;
    return sin(w*x);
}

int main() {
    double a, b, res;
    double w;
    SDFIntegration *integration;
    SDFIntegrationF f;
    sdf_integration_ini(&integration);
    a = 0; b = 10; w = 2;
    f = fsin;
    sdf_integration_apply(integration, &f, &w, a, b, &res);
    printf("%g\n", res);
    
    sdf_integration_fin(integration);
}
