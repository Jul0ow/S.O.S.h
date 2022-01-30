#include <err.h>
#include "vector.h"

struct vector* vector_new()
{
    struct vector* v = malloc(sizeof(struct vector));
    if (v == NULL)
        errx(1, "Not enough memory!");
    v->data = malloc(sizeof(int));
    if (v->data == NULL)
        errx(1, "Not enough memory!");
    v->size = 0;
    v->capacity = 1;
    return v;
}

void vector_free(struct vector* v)
{
    free(v->data);
    free(v);
}

void double_capacity(struct vector* v)
{
    v->capacity = v->capacity * 2;
    int* data = realloc(v->data, v->capacity * sizeof(int));
    if (data == NULL)
        errx(1, "Not enough memory!");
    v->data = data;
}

void vector_push(struct vector* v, int x)
{
    if (v->size >= v->capacity)
        double_capacity(v);
    *(v->data + v->size) = x;
    v->size++;
}

int vector_pop(struct vector* v, int* x)
{
    if (v->size == 0)
        return 0;
    *x = *(v->data + v->size - 1);
    v->size--;
    return 1;
}

int vector_get(struct vector* v, size_t i, int* x)
{
    if (i >= v->size)
        return 0;
    *x = *(v->data + i);
    return 1;
}

void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void vector_insert(struct vector* v, size_t i, int x)
{
    if (i <= v->size)
    {
        if (v->size >= v->capacity)
            double_capacity(v);

        for (size_t j = v->size; j > i; j--)
            swap(v->data + j, v->data + j - 1);

        *(v->data + i) = x;
        v->size++;
    }
}

int vector_remove(struct vector* v, size_t i, int* x)
{
    if (i >= v->size)
        return 0;
    
    *x = *(v->data + i);
    for (size_t j = i; j < v->size - 1; j++)
        swap(v->data + j, v->data + j + 1);
    v->size--;
    return 1;
}