#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include "xmalloc.h"

void * Xmalloc(size_t mem)
{
    void *res = malloc(mem);
    if (res == NULL)
        errx(errno,"Error in allocating memory");
    return res;
}

void * Xcalloc(size_t num, size_t bytePerType)
{
    void *res = calloc(num, bytePerType);
    if (res == NULL)
        err(errno,"Error in allocating memory");
    return res;
}

void * Xrealloc(void * pointer, size_t mem)
{
    pointer = realloc(pointer, mem);
    if(pointer == NULL)
        err(errno, "Error in allocating memory");
    return pointer;
}
