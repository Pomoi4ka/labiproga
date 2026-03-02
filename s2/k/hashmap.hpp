#ifndef HASHMAP_HPP_
#define HASHMAP_HPP_

static const float HASHMAP_DENSITY = 0.6f;

class HashMap {
public:
    class Item;
    class Iterator {
        HashMap &m_map;
        size_t m_index;
    public:
        inline Iterator(HashMap &map);
        inline Item *operator*();
        inline void operator++();
        inline operator bool() const;
    };

    typedef hash_t (Item:: *HashMethod)() const;
    typedef bool (Item:: *EqMethod)(Item const &) const;
    typedef void (Item:: *MovedMethod)() const;
    typedef void (Item:: *DestroyMethod)() const;
private:
    unsigned char *m_bitmap;
    void    *m_items;
    size_t   m_itemSize;
    size_t   m_count;
    size_t   m_cap;

    EqMethod      m_eq;
    HashMethod    m_hash;
    MovedMethod   m_moved;
    DestroyMethod m_destroy;

    inline bool bitmapAt(size_t index) const;
    inline const Item *itemAt(size_t index) const;
    inline Item *itemAt(size_t index);
    inline ssize_t findSlot(Item *item) const;
    inline void copyInto(size_t index, Item *item);
    inline ssize_t findItem(Item *item) const;
    inline void resize();

    inline HashMap(HashMap const &);
    inline void operator=(HashMap);
public:
    inline HashMap(size_t itemSize, EqMethod eq, HashMethod hash);
    inline HashMap(size_t itemSize, EqMethod eq, HashMethod hash,
                   MovedMethod moved, DestroyMethod destroy);
    inline ~HashMap();
    inline Item *get(Item *key);
    inline const Item *get(Item *key) const;
    inline bool insert(Item *item);
    inline bool remove(Item *key);
    inline Iterator iter();
    inline size_t size() const;
};

bool HashMap::bitmapAt(size_t index) const
{
    assert(index < m_cap);
    return !!(m_bitmap[index>>3] & (1 << (index&7)));
}

const HashMap::Item *HashMap::itemAt(size_t index) const
{
    assert(index < m_cap);
    return (HashMap::Item *)(reinterpret_cast<const char *>(m_items) + index * m_itemSize);
}

HashMap::Item *HashMap::itemAt(size_t index)
{
    assert(index < m_cap);
    return (HashMap::Item *)(reinterpret_cast<char *>(m_items) + index * m_itemSize);
}

ssize_t HashMap::findSlot(HashMap::Item *item) const
{
    if (!m_cap) return -1;
    size_t i, limit = HASHMAP_DENSITY * m_cap;
    hash_t hash = (reinterpret_cast<HashMap::Item *>(item)->*m_hash)();
    hash %= m_cap;
    for (i = 0; i < limit; ++i, hash = (hash + 1) % m_cap) {
        if (!bitmapAt(hash)) break;
    }
    if (i == limit) return -1;
    return hash;
}

void HashMap::copyInto(size_t index, HashMap::Item *item)
{
    assert(index < m_cap);
    char *dst = reinterpret_cast<char *>(itemAt(index));
    char *src = reinterpret_cast<char *>(item);
    for (size_t i = 0; i < m_itemSize; ++i) {
        dst[i] = src[i];
    }
}

ssize_t HashMap::findItem(HashMap::Item *item) const
{
    hash_t hash = (item->*m_hash)();
    if (!m_cap) return -1;
    hash %= m_cap;
    size_t i, limit = HASHMAP_DENSITY * m_cap;
    for (i = 0; i < limit; ++i, hash = (hash + 1) % m_cap) {
        if (!bitmapAt(hash)) continue;
        if (!(item->*m_eq)(*itemAt(hash))) continue;
        break;
    }
    if (i == limit) return -1;
    return hash;
}

void HashMap::resize()
{
    HashMap newMap(m_itemSize, m_eq, m_hash);
    newMap.m_cap    = m_cap ? m_cap * 2 : 1;
    newMap.m_items  = new char[newMap.m_cap * m_itemSize];
    size_t bitmapSize = (newMap.m_cap + 7)>>3;
    newMap.m_bitmap = new unsigned char[bitmapSize];
    for (size_t i = 0; i < bitmapSize; ++i)
        newMap.m_bitmap[i] = 0;

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
}

HashMap::HashMap(size_t itemSize, EqMethod eq, HashMethod hash)
    : m_bitmap()
    , m_items()
    , m_itemSize(itemSize)
    , m_count()
    , m_cap()
    , m_eq(eq)
    , m_hash(hash)
    , m_moved()
    , m_destroy()
{}

HashMap::HashMap(size_t itemSize, EqMethod eq, HashMethod hash,
                 MovedMethod moved, DestroyMethod destroy)
    : m_bitmap()
    , m_items()
    , m_itemSize(itemSize)
    , m_count()
    , m_cap()
    , m_eq(eq)
    , m_hash(hash)
    , m_moved(moved)
    , m_destroy(destroy)
{}

HashMap::~HashMap()
{
    if (m_destroy)
        for (Iterator it = iter(); it; ++it)
            (*it->*m_destroy)();

    if (m_items)  delete[] (char *)m_items;
    if (m_bitmap) delete[] m_bitmap;
}

HashMap::Item *HashMap::get(HashMap::Item *key)
{
    ssize_t index = findItem(key);
    if (index >= 0) return itemAt(index);
    return NULL;
}

const HashMap::Item *HashMap::get(HashMap::Item *key) const
{
    ssize_t index = findItem(key);
    if (index >= 0) return itemAt(index);
    return NULL;
}

bool HashMap::remove(HashMap::Item *key)
{
    ssize_t index = findItem(key);
    if (index < 0) return false;

    if (m_destroy) (itemAt(index)->*m_destroy)();
    m_bitmap[index>>3] &= ~(1<<(index&7));
    m_count--;

    return true;
}

bool HashMap::insert(HashMap::Item *item)
{
    ssize_t index = findItem(item);
    if (index >= 0) return false;

    index = findSlot(item);
    while (index < 0) {
        resize();
        index = findSlot(item);
    }

    copyInto(index, item);
    if (m_moved) (item->*m_moved)();
    m_bitmap[index>>3] |= 1<<(index&7);
    m_count++;

    return true;
}

HashMap::Iterator::Iterator(HashMap &map)
    : m_map(map)
    , m_index()
{
    while (*this && !m_map.bitmapAt(m_index)) m_index++;
}


HashMap::Item *HashMap::Iterator::operator*()
{
    return m_map.itemAt(m_index);
}

void HashMap::Iterator::operator++()
{
    if (!*this) return;

    if (m_map.bitmapAt(m_index))
        ++m_index;

    while (*this) {
        if (m_map.bitmapAt(m_index)) break;
        m_index++;
    }
}

HashMap::Iterator::operator bool() const { return m_index < m_map.m_cap; }
HashMap::Iterator HashMap::iter() { return *this; }
size_t HashMap::size() const { return m_count; }

#endif // HASHMAP_HPP_
