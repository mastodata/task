#include "hll.h"
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

void binprint(size_t x)
{
    int c = 0;
    printf("repr: '");
    while (x) {
        if (x & 1)
            printf("1");
        else
            printf("0");
        x >>= 1;
        c++;
    }
    printf("' %d \n", c);
}

int count_run_of_zeros(size_t x)
{
    int c = 0;
    if (x == 0)
    {
        return 0;
    }

    while ((x & 1) == 0)
    {
        x >>= 1;
        c++;
    }

    return c;
}

long double alpha_m(size_t m)
{
    switch (m) {
        case 2:
            return 0.35119395;
        case 4:
            return 0.53243461;
        case 8:
            return 0.62560871;
        case 16:
            return 0.67310202;
        case 32:
            return 0.69712263;
        case 64:
            return 0.70920845;
        default:
            return 0.7213 / (1 + (1.079 / m));
    }
}

hll hll_new(size_t size)
{
    long long *arr = calloc(size, sizeof(long long));
    hll sketch = {
        arr,
        size,
        ceil(log2(size)),
    };
    return sketch;
}

void hll_destroy(hll *sketch)
{
    free(sketch->arr);
}

void hll_add(hll *sketch, long long id)
{
    size_t x = hll_hash(sketch->size, id);

    // get 1 + first b bits of x. this will be the index of the counter to
    // modify
    int mask = ((1 << sketch->b) - 1);
    size_t i = (mask & x);

    // get the position of the leftmost 1, after removing the first b bits
    size_t rest = x >> sketch->b;
    int reg_val = count_run_of_zeros(rest) + 1;

    // update sketch
    sketch->arr[i] = max(sketch->arr[i], reg_val);
}

long long hll_count(hll *sketch)
{
    long double z = 0;
    for (unsigned int i = 0; i < sketch->size; i++)
    {
        z += powl(2.0, -sketch->arr[i]);
    }
    z = 1.0 / z;

    long double e = roundl(alpha_m(sketch->size) * sketch->size * sketch->size * z);
    if (e < 2.5 * sketch->size) {
        int v = 0;
        for (unsigned int i = 0; i < sketch->size; i++)
        {
            if (sketch->arr[i] == 0)
            {
                v++;
            }
        }
        if (v == 0)
        {
            return e;
        }
        else
        {
            long double t = logl((long double)sketch->size / v);
            return roundl(sketch->size * t);
        }
    }
    else
    {
        return e;
    }
}

size_t hll_hash(size_t size, long long id)
{
    return id;
}
