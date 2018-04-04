#include <gsl/gsl_integration.h>

#include "sdf_memory.h"
#include "sdf_integration.h"

#define EPS (1e-5)
#define NWORK (1000)
#define KEY (GSL_INTEG_GAUSS15)

struct SDFIntegration {
    gsl_integration_workspace *work;
    int neval;
    double abserr;
};

int sdf_integration_ini(SDFIntegration **pq) {
    SDFIntegration *q;
    EMALLOC(1, &q);
    q->work = gsl_integration_workspace_alloc(NWORK);
    *pq = q;
    return INTEGRATION_OK;
}

int sdf_integration_fin(SDFIntegration *q) {
    gsl_integration_workspace_free(q->work);
    EFREE(q);
    return INTEGRATION_OK;
}

int sdf_integration_apply(SDFIntegration *q, SDFIntegrationF fun, void *par, double a, double b, /**/ double *presult) {
    gsl_function F;
    gsl_integration_workspace *work;
    double epsrel, epsabs, abserr, result;
    size_t neval;
    work = q->work;
    F.function = fun;
    F.params = par;
    epsabs = epsrel = EPS;
    gsl_integration_qag(&F, a, b, epsabs, epsrel, NWORK, KEY, /**/
                        work, &result, &abserr);

    q->abserr = abserr; q->neval = neval;
    *presult = result;
    return INTEGRATION_OK;
}

int sdf_integration_neval(SDFIntegration *q, int *pneval) {
    *pneval = q->neval;
    return INTEGRATION_OK;
}

int sdf_integration_abserr(SDFIntegration *q, double *abserr) {
    *abserr = q->abserr;
    return INTEGRATION_OK;
}
