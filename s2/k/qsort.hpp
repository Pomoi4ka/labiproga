#ifndef QSORT_HPP_
#define QSORT_HPP_

typedef int (*compar_f)(void *, const void *, const void *);

static void swap(void *x, void *y, size_t n)
{
    char *a = reinterpret_cast<char *>(x);
    char *b = reinterpret_cast<char *>(y);
    char  t, *end = a + n;
    while (a != end) t = *a, *a = *b, *b = t, ++b, ++a;
}

static void quicksort_impl(void *items, size_t size, long low,
                    long high, compar_f cmp, void *cmp_ctx)
{
    char *xs    = reinterpret_cast<char *>(items);
    char *pivot = xs + high - size;
    long  lo    = low - size;
    long  hi    = high - size;

    if (low >= high) return;

    for (;;) {
        do lo += size; while (cmp(cmp_ctx, xs + lo, pivot) < 0);
        if (hi) do hi -= size; while (hi && cmp(cmp_ctx, xs + hi, pivot) > 0);
        if (lo >= hi) break;
        swap(xs + lo, xs + hi, size);
    }

    swap(xs + lo, pivot, size);
    quicksort_impl(xs, size, low, lo, cmp, cmp_ctx);
    quicksort_impl(xs, size, lo + size, high, cmp, cmp_ctx);
}

static void quicksort(void *xs, size_t x_size, size_t x_count,
                      compar_f cmp, void *cmp_ctx)
{
    quicksort_impl(xs, x_size, 0, x_count * x_size, cmp, cmp_ctx);
}

#endif // QSORT_HPP_
