#include <stdio.h>
#include <sdf_integration.h>

int main() {
    SDFIntegration *integration;
    sdf_integration_ini(&integration);
    sdf_integration_fin(integration);
}
