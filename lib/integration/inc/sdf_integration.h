typedef struct SDFIntegration SDFIntegration;

typedef double (*SDFIntegrationF) (double X, void *PARAMS);
enum {INTEGRATION_OK, INTEGRATION_FAIL};

int sdf_integration_ini(SDFIntegration**);
int sdf_integration_apply(SDFIntegration*, const SDFIntegrationF*, double *result);
int sdf_integration_fin(SDFIntegration*);
