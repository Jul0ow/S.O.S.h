#ifndef XMALLOC_H
#define XMALLOC_H
#include <stdlib.h>

void *xmalloc(size_t mem);

void *xcalloc(size_t num, size_t bytePerType);

#endif
