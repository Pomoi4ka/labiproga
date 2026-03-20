#ifndef БЛОК_З_
#define БЛОК_З_

#include "константы.hpp"

class Блок {
    Строка ч_строки[К_БЛОК_КОЛИЧЕСТВО_СТРОК];
    Строка ч_остаток;
    size_t ч_количество;
    size_t ч_ширина;
    size_t ч_отступ;
    bool ч_абзацВыведен;

    bool внестиСимвол(char);
public:
    Блок();

    void поставитьШиринуСтроки(size_t);
    void поставитьОступАбзаца(size_t);
    bool внестиСтроку(std::istream &);
    void обработать();
    void сброс();

    size_t количество() const;
    Строка const *строки() const;
};

std::ostream &operator<<(std::ostream &, Блок const&);

#endif // БЛОК_З_
