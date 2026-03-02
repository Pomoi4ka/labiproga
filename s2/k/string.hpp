#ifndef STRING_HPP_
#define STRING_HPP_

#include "hash.hpp"

static inline bool isSpace(char x)
{
    switch (x) {
    case ' ': case '\n': case '\t':
    case '\r': return true;
    }
    return false;
}

class String {
    char *m_data;
    size_t m_count;
    size_t m_cap;
public:
    inline String();
    inline ~String();
    inline String(String const &other);
    inline String &operator=(String const &);
    inline bool operator==(const char *) const;
    inline bool operator!=(const char *) const;
    inline static String readWordFromStream(std::istream &s);
    inline const char *data() const;
    inline size_t length() const;
    inline void moved();
    inline void add(char x);

    inline bool eq(String const &other) const;
    inline hash_t hash() const;
    inline void destroy();
};

inline std::ostream &operator<<(std::ostream &strm, String const &s)
{
    strm.write(s.data(), s.length());
    return strm;
}

String::String()
    : m_data()
    , m_count()
    , m_cap()
{}

String::~String()
{
    if (m_data) delete[] m_data;
}

String::String(String const &other)
    : m_data(new char[other.m_cap])
    , m_count(other.m_count)
    , m_cap(other.m_cap)
{
    for (size_t i = 0; i < other.m_count; ++i)
        m_data[i] = other.m_data[i];
}

String String::readWordFromStream(std::istream &s)
{
    String acc;
    while (isSpace(s.peek())) s.get();
    while (!isSpace(s.peek()) && s) acc.add(s.get());
    return acc;
}

const char *String::data() const { return m_data; }
size_t String::length() const { return m_count; }

void String::add(char x)
{
    if (m_count >= m_cap) {
        if (m_cap) m_cap *= 2;
        else m_cap = 1;
        char *new_data = new char[m_cap];
        for (size_t i = 0; i < m_count; ++i)
            new_data[i] = m_data[i];
        delete[] m_data;
        m_data = new_data;
    }
    m_data[m_count++] = x;
}

bool String::eq(String const &other) const
{
    if (m_count != other.m_count) return false;
    for (size_t i = 0; i < m_count && i < other.m_count; i++)
        if (m_data[i] != other.m_data[i])
            return false;
    return true;
}

hash_t String::hash() const
{
    return dummyHash(m_data, m_count);
}

String &String::operator=(String const &other)
{
    this->~String();
    return *new (this) String(other);
}

void String::moved()
{
    m_data = NULL;
    m_count = 0;
    m_cap = 0;
}

bool String::operator!=(const char *cstr) const
{
    return !(*this == cstr);
}

bool String::operator==(const char *cstr) const
{
    const char *data = m_data;
    const char *end = m_data + m_count;
    while (*cstr && data < end) {
        if (*data++ != *cstr++) return false;
    }
    return *cstr == 0 && data == end;
}

void String::destroy() { this->~String(); }

#endif // STRING_HPP_
