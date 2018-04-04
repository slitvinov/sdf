#define EMALLOC(n, ppdata) emalloc((n)*sizeof(**(ppdata)), (void**)(ppdata))
#define EFREE(pdata) efree(pdata)

void emalloc(size_t, void**);
void efree(void *ptr);
