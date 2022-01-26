#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include "xmalloc.h"

void * xmalloc(size_t mem)
{
    void *res = malloc(mem);
    if (res == NULL)
        errx(errno,"Error in allocating memory");
    return res;
}

void * xcalloc(size_t num, size_t bytePerType)
{
    void *res = calloc(num, bytePerType);
    if (res == NULL)
        err(errno,"Error in allocating memory");
    return res;
}
