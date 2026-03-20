#ifndef СТРОКА_З_
#define СТРОКА_З_

#include <cstddef>
#include <ostream>

class Строка {
    static const size_t НАЧАЛЬНАЯ_ЁМКОСТЬ;
    static const size_t РАЗМЕР_ЁМКОСТНОГО_ПОЛЯ;

    char *ч_данные;
    size_t ч_длина;
    size_t &ёмкость();
    void подрасти(size_t);

    static char *выделитьСЁмкостью(size_t); // @Drakoshaaa
    // так как c++98 не имеет default, delete конструкторов, то пихнём
    // их в private без реализации. как это ещё больше обезопасить я
    // не знаю
    inline void operator=(Строка);
    inline Строка(Строка const&);
public:
    enum Бритва {
        БР_ПЕРЕНОСЫ,
        БР_ПРОБЕЛЫ,
        БР_ПЕРЕНОСЫ_И_ПРОБЕЛЫ
    };

    explicit Строка();
    ~Строка();
    void добавить(char);
    void добавить(const char *, size_t);
    void сбросить();
    size_t длина() const;
    const char *данные() const;
    char *данные();
    void обрезать(size_t); // Справа
    void срезать(size_t); // Слева

    char operator[](size_t) const;

    void побрить(Бритва = БР_ПЕРЕНОСЫ_И_ПРОБЕЛЫ);
    void вставить(size_t куда, const char *кого, size_t сколько);
    void выровнятьПоШирине(size_t);
};

std::ostream &operator<<(std::ostream &, Строка const &);

#endif // СТРОКА_З_
