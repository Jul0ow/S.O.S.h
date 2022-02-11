#ifndef XMALLOC_H
#define XMALLOC_H
#include <stdlib.h>

void *Xmalloc(size_t mem);

void *Xcalloc(size_t num, size_t bytePerType);

void *Xrealloc(void * pointer, size_t mem);

#endif
