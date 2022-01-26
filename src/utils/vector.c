#include <err.h>
#include "vector.h"
#include <stdio.h>

struct vector *vector_new()
{
    struct vector * res = malloc(sizeof(struct vector));
    if(res == NULL)
        errx(1, "Not enough memory!\n");
    res->data = malloc(sizeof(char));
    if(res->data == NULL)
        errx(1, "Not enough memory!\n");
    res->capacity = 1;
    res->size = 0;
    return res;
}

void vector_free(struct vector *v)
{
    free(v->data);
    free(v);
}

void double_capacity(struct vector *v)
{
    v->data = realloc(v->data, v->capacity*2 * sizeof(char));
    if(v->data == NULL)
        errx(1, "Not enough memory!\n");
    v->capacity *=2;
}

void vector_push(struct vector *v, char x)
{
    if(v->capacity == v->size)
        double_capacity(v);
    v->data[v->size] = x;
    v->size++;
}

int vector_pop(struct vector *v, char *x)
{
    if(v->size == 0)
        return 0;
    *x = v->data[--(v->size)];
    return 1;
}

int vector_get(struct vector *v, size_t i, char *x)
{
    if(i < v->size)
    {
        *x = v->data[i];
        return 1;
    }
    return 0;
}

void vector_insert(struct vector *v, size_t i, char x)
{
    if(i <= v->size)
    {
        if(i == v->capacity)
            double_capacity(v);
        while(i < v->size)
        {
            char tmp = v->data[i];
            v->data[i++] = x;
            x = tmp;
        }
        v->data[i] = x;
        (v->size)++;
    }
}

int vector_remove(struct vector *v, size_t i, char *x)
{
    if(i < v->size)
    {
        *x = v->data[i];
        while(i < v->size -1)
        {
            v->data[i] = v->data[i+1];
            i++;
        }
        (v->size)--;
        return 1;
    }
    return 0;

}
