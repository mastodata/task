#ifndef HLL
#define HLL

#include <stddef.h>

typedef struct hll {
    long long *arr;

    size_t size;

    // log2(size)
    int b;
} hll;

// size should be a power of 2 >= 2
hll hll_new(size_t size);
void hll_destroy(hll *sketch);
void hll_add(hll *sketch, long long id);
long long hll_count(hll *sketch);
void hll_merge(hll *left, hll *right);
size_t hll_hash(size_t size, long long id);

#endif
