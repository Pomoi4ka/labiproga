#ifndef СТРОКА_З_
#define СТРОКА_З_

class Строка {
    static const size_t НАЧАЛЬНАЯ_ЁМКОСТЬ = 1;
    static const size_t РАЗМЕР_ЁМКОСТНОГО_ПОЛЯ = sizeof(size_t);

    char *ч_данные;
    size_t ч_длина;
    inline size_t &ёмкость();
    static char *выделитьСЁмкостью(size_t);

    // так как c++98 не имеет default, delete конструкторов, то пихнём их
    // в private без реализации. как это ещё больше обезопасить я не знаю
    inline void operator=(SizedString);
    inline Строка(Строка const&);
public:
    explicit Строка();
    ~Строка();
    void добавить(char);
    void сбросить();
    size_t длина() const;
    const char *данные() const;
    char *данные();
    void обрезать(size_t);
};

#endif // СТРОКА_З_
