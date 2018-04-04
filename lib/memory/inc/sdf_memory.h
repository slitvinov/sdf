#define EMALLOC(n, ppdata) emalloc((n)*sizeof(**(ppdata)), (void**)(ppdata))
#define EFREE(pdata) efree(pdata)

void emalloc(int, void**);
void efree(void *ptr);
