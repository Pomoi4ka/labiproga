#ifndef STRING_HPP_
#define STRING_HPP_

#include <cstddef>
#include <ostream>

class String {
    static const size_t INITIAL_CAP;
    static const size_t CAPACITY_FIELD_SIZE;

    char *m_data;
    size_t m_length;
    size_t &capacity();
    char *allocWithCapacity(size_t cap);

    // так как c++98 не имеет default, delete конструкторов, то пихнём их
    // в private без реализации. как это ещё больше обезопасить я не знаю
    void operator=(String);
    String(String const&);
public:
    explicit String();
    ~String();
    void add(char);
    size_t length() const;
    const char *data() const;
    char *data();
    bool hasWords() const;
};

std::ostream &operator<<(std::ostream &, String const&);

#endif // STRING_HPP_
