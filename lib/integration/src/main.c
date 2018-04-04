#include <gsl/gsl_integration.h>

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

int sdf_integration_apply(SDFIntegration *q, SDFIntegrationF *fun, double a, double b, double *presult) {
//    gsl_integration_qng (const gsl_function * F, double A,
//          double B, double EPSABS, double EPSREL, double * RESULT,
//          double * ABSERR, size_t * NEVAL)
    gsl_function F;
    double epsrel, epsabs, abserr, result;
    size_t neval;
    F.function = *fun;
    F.params = NULL;
    epsabs = epsrel = 1e-7;
    gsl_integration_qng(&F, a, b, epsabs, epsrel, &result, &abserr, &neval);
    
    *presult = result;
    
    
    return INTEGRATION_OK;

}
