#ifndef БЛОК_З_
#define БЛОК_З_

#include "константы.hpp"

class Блок {
    Строка ч_строки[К_БЛОК_КОЛИЧЕСТВО_СТРОК];
    Строка ч_временная;
    size_t ч_количество;
    size_t ч_ширина;
    size_t ч_отступ;
    bool ч_абзацВыполнен;
    bool ч_переносСлова;

    bool внестиСимвол(char);
    void перенести();
public:
    Блок();

    void поставитьШиринуСтроки(size_t);
    void поставитьОступАбзаца(size_t);
    void заполнитьБлок(std::istream &);
    void обработать();
    void сброс();

    size_t количество() const;
    Строка const *строки() const;
};

std::ostream &operator<<(std::ostream &, Блок const&);

#endif // БЛОК_З_
