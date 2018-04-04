#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

#include "sdf_memory.h"
#include "sdf_root.h"

#define EPS (1e-5)
#define NITER (1000)

struct SDFRoot { gsl_root_fsolver *solver; };

int sdf_root_ini(SDFRoot **pq) {
    SDFRoot *q;
    EMALLOC(1, &q);
    q->solver = gsl_root_fsolver_alloc(gsl_root_fsolver_brent);
    *pq = q;
    return ROOT_OK;
}

int sdf_root_fin(SDFRoot *q) {
    gsl_root_fsolver_free(q->solver);
    EFREE(q);
    return ROOT_OK;
}

int sdf_root_apply(SDFRoot *q, SDFRootF fun, void *par, double lo, double hi, /**/ double *presult) {
    double result;
    int status, iter;
    gsl_function F;
    gsl_root_fsolver *solver;
    double epsrel, epsabs;
    solver = q->solver;
    F.function = fun;
    F.params = par;
    epsabs = epsrel = EPS; iter = 0;
    gsl_root_fsolver_set(solver, &F, lo, hi);
    for (;;) {
        iter++;
        status = gsl_root_fsolver_iterate(solver);
        result = gsl_root_fsolver_root(solver);
        lo = gsl_root_fsolver_x_lower(solver);
        hi = gsl_root_fsolver_x_upper(solver);
        status = gsl_root_test_interval(lo, hi, epsabs, epsrel);
        if (status == GSL_SUCCESS) break;
        if (iter   == NITER)       return ROOT_FAIL;
    }

    *presult = result;
    return ROOT_OK;
}
