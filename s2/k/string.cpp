#include <cstddef>
#include <cassert>
#include <inttypes.h>
#include <ostream>

#include "string.hpp"

String::String()
    : StringBlocks()
{}

StringBlocks::ConstNode String::view() const
{
    return head();
}

std::ostream &operator<<(std::ostream &sink, String const &str)
{
    StringBlocks::ConstNode node = str.view();
    for (; *node; node = node.next()) {
        StringBlock const *block = *node;
        sink.write(block->data(), block->len());
        if (*node.next()) sink << " -> ";
    }
    return sink;
}

size_t String::length() const
{
    size_t len;
    StringBlocks::ConstNode node = head();
    if (!*node) return 0;
    for (len = 0; *node.next(); node = node.next())
        len += STRING_BLOCK_SIZE;
    return len + node->len();
}

void String::append(char x)
{
    StringBlock *tail = List::tail();
    if (!tail || !tail->add(x))
        List::append()->add(x);
}

bool String::isEmpty() const
{
    return !hasNext();
}

void String::reset()
{
    List::~List();
    new (this) List;
}

bool String::operator!=(const char *cstr) const
{
    return !(*this == cstr);
}

bool String::operator==(const char *cstr) const
{
    StringBlocks::ConstNode node = head();
    for (; *node; cstr += node->len(), node = node.next())
        if (!node->have(cstr)) return false;
    return !*cstr;
}

bool String::operator==(const String &other) const
{
    StringBlocks::ConstNode a = head();
    StringBlocks::ConstNode b = other.head();
    for (; *a && *b; a = a.next(), b = b.next())
        if (**a != **b) return false;

    return !*a && !*b;
}
