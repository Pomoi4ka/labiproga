#include <cstddef>

#include "consts.hpp"
#include "chars.hpp"

bool isDigit(char x)
{
    return '0' <= x && x <= '9';
}

bool isAlpha(char x)
{
    return x |= 'a'^'A', 'a' <= x && x <= 'z';
}

bool isSpace(char x)
{
    for (const char *s = SPACE_SYMBOLS; *s; ++s)
        if (x == *s) return true;
    return false;
}
