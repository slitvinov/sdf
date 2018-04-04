#include "sdf_memory.h"
#include "sdf_integration.h"

struct SDFIntegration {

};

int sdf_integration_ini(SDFIntegration **pq) {
    SDFIntegration *q;
    EMALLOC(1, &q);
    *pq = q;
    return INTEGRATION_OK;
}

int sdf_integration_fin(SDFIntegration *q) {
    EFREE(q);
    return INTEGRATION_OK;
}

int sdf_integration_apply(SDFIntegration *q, const SDFIntegrationF *fun, double *presult) {
    return INTEGRATION_OK;
}
