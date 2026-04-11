#include <cstddef>

#include "consts.hpp"
#include "chunk.hpp"

bool Chunk::add(char x)
{
    if (len() == CHUNK_SYM_COUNT) return false;
    m_data[len()] = x;
    return true;
}

size_t Chunk::len() const
{
    size_t i;
    for (i = 0; i < CHUNK_SYM_COUNT; ++i)
        if (!m_data[i]) break;
    return i;
}

const char *Chunk::data() const { return m_data; }

Chunk::Chunk()
    : m_data()
{}

int Chunk::compare(Chunk const &c) const
{
    char a, b;
    for (size_t i = 0; i < CHUNK_SYM_COUNT; ++i) {
        a = m_data[i];
        b = c.m_data[i];
        if (a != b) return a - b;
        if (!a && !b) return 0;
    }
    return 0;
}

std::ostream &operator<<(std::ostream &s, Chunk const &c)
{
    s.write(c.data(), c.len());
    return s;
}
