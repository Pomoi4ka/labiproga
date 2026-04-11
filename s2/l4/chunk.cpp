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

std::ostream &operator<<(std::ostream &s, Chunk const &c)
{
    s.write(c.data(), c.len());
    return s;
}
