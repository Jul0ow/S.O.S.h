#include <err.h>
#include "vector.h"
#include <stdio.h>
#include "xmalloc.h"

vector *vector_new()
{
    vector * res = xmalloc(sizeof(vector));
    if(res == NULL)
        errx(1, "Not enough memory!\n");
    res->string = xmalloc(sizeof(char));
    if(res->string == NULL)
        errx(1, "Not enough memory!\n");
    res->capacity = 1;
    res->size = 0;
    return res;
}

void vector_free(vector *v)
{
    free(v->string);
    free(v);
}

void double_capacity(vector *v)
{
    v->string = xrealloc(v->string, v->capacity*2 * sizeof(char));
    if(v->string == NULL)
        errx(1, "Not enough memory!\n");
    v->capacity *=2;
}

void vector_push(vector *v, char x)
{
    if(v->capacity == v->size)
        double_capacity(v);
    v->string[v->size] = x;
    v->size++;
}

int vector_pop(vector *v, char *x)
{
    if(v->size == 0)
        return 0;
    *x = v->string[--(v->size)];
    return 1;
}

int vector_get(vector *v, size_t i, char *x)
{
    if(i < v->size)
    {
        *x = v->string[i];
        return 1;
    }
    return 0;
}

void vector_insert(vector *v, size_t i, char x)
{
    if(i <= v->size)
    {
        if(i == v->capacity)
            double_capacity(v);
        while(i < v->size)
        {
            char tmp = v->string[i];
            v->string[i++] = x;
            x = tmp;
        }
        v->string[i] = x;
        (v->size)++;
    }
}

int vector_remove(vector *v, size_t i, char *x)
{
    if(i < v->size)
    {
        *x = v->string[i];
        while(i < v->size -1)
        {
            v->string[i] = v->string[i+1];
            i++;
        }
        (v->size)--;
        return 1;
    }
    return 0;

}
