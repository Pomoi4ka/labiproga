#ifndef HASH_HPP_
#define HASH_HPP_

typedef unsigned hash_t;

static inline hash_t dummyHash(const void *data, size_t len)
{
    hash_t h = 0x63fcba32;
    const char *d = reinterpret_cast<const char *>(data);

    for (size_t i = 0; i < len; ++i) {
        h ^= h << 12 | h >> 9;
        h ^= d[i];
        h ^= 0xefc431fe;
    }
    return h;
}

#endif // HASH_HPP_
