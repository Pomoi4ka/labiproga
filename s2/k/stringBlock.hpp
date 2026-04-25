#ifndef STRING_BLOCK_HPP_
#define STRING_BLOCK_HPP_

#include "consts.hpp"

class StringBlock {
    char block[STRING_BLOCK_SIZE];
public:
    StringBlock();
    bool operator!=(StringBlock const &) const;
    bool add(char);
    size_t len() const;
    char const *data() const;
    bool have(const char *) const;
};

#endif // STRING_BLOCK_HPP_
