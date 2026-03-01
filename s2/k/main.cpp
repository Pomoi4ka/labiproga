#include <iostream>
#include <fstream>
#include <cassert>

#include "string.hpp"

typedef unsigned hash_t;

hash_t dummyHash(const void *data, size_t len)
{
    hash_t h = 1931;
    const char *d = reinterpret_cast<const char *>(data);

    for (size_t i = 0; i < len; ++i) {
        h ^= h << 12 | h >> 9;
        h ^= d[i];
        h ^= 0xefc431fe;
    }
    return h;
}

static const float HASHMAP_DENSITY = 0.6f;

class HashMap {
    class Item;
    unsigned char *m_bitmap;
    void    *m_items;
    size_t   m_itemSize;
    size_t   m_count;
    size_t   m_cap;

    bool (*m_cmp)(const void *, const void *);
    hash_t (Item:: *m_hash)();

    inline bool bitmapAt(size_t index) const
    {
        assert(index < m_cap);
        return !!(m_bitmap[index>>3] & (1 << (index&7)));
    }

    inline const void *itemAt(size_t index) const
    {
        assert(index < m_cap);
        return reinterpret_cast<const char *>(m_items) + index * m_itemSize;
    }

    inline void *itemAt(size_t index)
    {
        assert(index < m_cap);
        return reinterpret_cast<char *>(m_items) + index * m_itemSize;
    }

    inline ssize_t findSlot(void *item) const
    {
        if (!m_cap) return -1;
        size_t i, limit = HASHMAP_DENSITY * m_cap;
        hash_t hash = dummyHash(item, m_itemSize);
        hash %= m_cap;
        for (i = 0; i < limit; hash = (hash + 1) % m_cap) {
            if (!bitmapAt(hash)) break;
        }
        return i == limit ? -1 : hash;
    }

    inline void copyInto(size_t index, void *item)
    {
        assert(index < m_cap);
        char *dst = reinterpret_cast<char *>(itemAt(index));
        char *src = reinterpret_cast<char *>(item);
        for (size_t i = 0; i < m_itemSize; ++i) {
            dst[i] = src[i];
        }
    }

    inline ssize_t findItem(void *item) const
    {
        hash_t hash = dummyHash(item, m_itemSize);
        if (!m_cap) return -1;
        hash %= m_cap;
        size_t i, limit = HASHMAP_DENSITY * m_cap;
        for (i = 0; i < limit; hash = (hash + 1) % m_cap) {
            if (!bitmapAt(hash)) continue;
            if (!m_cmp(itemAt(hash), item)) continue;
            break;
        }
        return i == limit ? -1 : hash;
    }

    inline void resize()
    {
        HashMap newMap(m_itemSize, m_cmp);
        newMap.m_cap    = m_cap * 2;
        newMap.m_items  = new char[m_cap * m_itemSize];
        newMap.m_bitmap = new unsigned char[m_cap];

        for (size_t k = 0, i = 0; k < m_count && i < m_cap; ++i) {
            if (!bitmapAt(i)) continue;
            k += 1;

            newMap.insert(itemAt(i));
        }

        delete[] (char*)m_items;
        delete[] m_bitmap;

        m_items         = newMap.m_items;
        m_bitmap        = newMap.m_bitmap;
        m_count         = newMap.m_count;
        m_cap           = newMap.m_cap;
        newMap.m_items  = NULL;
        newMap.m_bitmap = NULL;
        newMap.m_count  = 0;
        newMap.m_cap    = 0;
    }
public:
    HashMap(size_t itemSize, bool (*cmp)(const void *, const void *))
        : m_bitmap()
        , m_items()
        , m_itemSize(itemSize)
        , m_count()
        , m_cap()
        , m_cmp(cmp)
    {}

    inline bool insert(void *item)
    {
        ssize_t index = findItem(item);
        if (index >= 0) return false;

        index = findSlot(item);
        while (index < 0) {
            resize();
            index = findSlot(item);
        }

        copyInto(index, item);
        m_bitmap[index>>3] |= 1<<(index&7);
        m_count++;

        return true;
    }
};

int main()
{
    std::ifstream f("input.txt");

    while (!f.eof()) {
        String s = String::readWordFromStream(f);
        std::cout << s << std::endl;
    }

    return 0;
}
