#include <gsl/gsl_integration.h>

#include "sdf_memory.h"
#include "sdf_integration.h"

struct SDFIntegration {
    int neval;
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

int sdf_integration_apply(SDFIntegration *q, SDFIntegrationF *fun, void *par, double a, double b, /**/ double *presult) {
    gsl_function F;
    double epsrel, epsabs, abserr, result;
    size_t neval;
    F.function = *fun;
    F.params = par;
    epsabs = epsrel = 1e-7;
    gsl_integration_qng(&F, a, b, epsabs, epsrel, /**/
                        &result, &abserr, &neval);

    q->neval = neval;
    *presult = result;
    return INTEGRATION_OK;
}

int sdf_integration_neval(SDFIntegration *q, int *pneval) {
    *pneval = q->neval;
    return INTEGRATION_OK;
}
