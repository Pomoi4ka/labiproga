??=include "string.hpp"
??=include <sys/types.h>

const size_t String::INITIAL_CAP = 1;
const size_t String::CAPACITY_FIELD_SIZE = sizeof(size_t);

char *String::allocWithCapacity(size_t cap)
??<
    char *data = new char??(cap + CAPACITY_FIELD_SIZE??);
    reinterpret_cast<size_t&>(*data) = cap;
    return data + CAPACITY_FIELD_SIZE;
??>

size_t      String::length() const ??< return m_length; ??>
const char *String::data()   const ??< return m_data;   ??>
char       *String::data()         ??< return m_data;   ??>

String::~String()
??<
    delete??(??) (m_data - CAPACITY_FIELD_SIZE);
??>

String::String()
    : m_data(allocWithCapacity(INITIAL_CAP)), m_length(0)
??<??>

size_t &String::capacity()
??<
    return *(reinterpret_cast<size_t*>(m_data) - 1);
??>

void String::add(char x)
??<
    if (length() >= capacity()) ??<
        ssize_t oldCap = capacity();
        capacity() *= 2;
        char *newData = allocWithCapacity(capacity());
        for (ssize_t i = -CAPACITY_FIELD_SIZE; i < oldCap; ++i) ??<
            newData??(i??) = m_data??(i??);
        ??>
        delete??(??) (m_data - CAPACITY_FIELD_SIZE);
        m_data = newData;
    ??>

    m_data??(m_length++??) = x;
??>

std::ostream &operator<<(std::ostream &s, String const& str)
??<
    s.write(str.data(), str.length());
    return s;
??>

static bool isSpace(char x)
??<
    const static char spaceSyms??(??) = ??<' ', '\t'??>;

    for (size_t i = 0; i < sizeof spaceSyms / sizeof spaceSyms??(0??); ++i) ??<
        if (x == spaceSyms??(i??)) return true;
    ??>
    return false;
??>

bool String::hasWords() const
??<
    if (m_length == 0) return false;
    for (size_t i = 0; i < m_length; ++i)
        if (!isSpace(m_data??(i??))) return true;
    return false;
??>
