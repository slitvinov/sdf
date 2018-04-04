#include <gsl/gsl_roots.h>

#include "sdf_memory.h"
#include "sdf_root.h"

struct SDFRoot { };

int sdf_root_ini(SDFRoot **pq) {
    SDFRoot *q;
    EMALLOC(1, &q);
    *pq = q;
    return ROOT_OK;
}

int sdf_root_fin(SDFRoot *q) {
    EFREE(q);
    return ROOT_OK;
}

int sdf_root_apply(SDFRoot *q, SDFRootF fun, void *par, double lo, double hi, /**/ double *presult) {
    double result;
    *presult = result;
    return ROOT_OK;
}

