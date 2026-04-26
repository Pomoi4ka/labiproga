#include <cstddef>

#include "stringBlock.hpp"

StringBlock::StringBlock()
    : block()
{}

bool StringBlock::add(char x)
{
    size_t len = this->len();
    if (len == STRING_BLOCK_SIZE) return false;
    block[len] = x;
    return true;
}

size_t StringBlock::len() const
{
    size_t i;
    for (i = 0; i < STRING_BLOCK_SIZE; ++i)
        if (!block[i]) break;
    return i;
}

char const *StringBlock::data() const
{
    return block;
}

bool StringBlock::have(const char *c) const
{
    const char *b = block;
    const char *e = block + STRING_BLOCK_SIZE;
    while (*c && b != e)
        if (*b++ != *c++) return false;
    return true;
}

bool StringBlock::operator!=(StringBlock const &other) const
{
    for (size_t i = 0; i < STRING_BLOCK_SIZE; ++i)
        if (block[i] != other.block[i]) return true;
    return false;
}
