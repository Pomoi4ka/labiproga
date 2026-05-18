#ifndef STRING_HPP_
#define STRING_HPP_

#include "stringBlock.hpp"
#include "list.hpp"

typedef List<StringBlock> StringBlocks;

class String : StringBlocks {
public:
    void transfer_from(String &);

    String();
    bool operator==(const char *) const;
    bool operator==(const String &) const;
    bool operator!=(const char *) const;
    size_t length() const;
    void append(char);
    StringBlocks::ConstNode view() const;
    bool isEmpty() const;
    void reset();
};

std::ostream &operator<<(std::ostream &, String const &);

#endif // STRING_HPP_
