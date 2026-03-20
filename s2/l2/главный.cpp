#include <iostream>
#include <fstream>

#include "строка.hpp"
#include "блок.hpp"

size_t длинаПервогоСлова(Строка const &стр);

#include <cstring>
#include "символы.hpp"

int main()
{
    Блок блок;
    std::ifstream input("input.txt");
    if (!input.is_open()) {
        std::cerr << "ОШИБКА: неудалось открыть файл" << std::endl;
        return 1;
    }

    size_t отступ, ширина;
    input >> отступ >> ширина;
    if (!input) {
        std::cerr << "ОШБИКА: неверный формат файла: ожидается отступ и ширина" << std::endl;
        return 1;
    }

    if (отступ > ширина) {
        std::cerr << "ОШИБКА: отступ не может быть больше ширины" << std::endl;
        return 1;
    }

    блок.поставитьОступАбзаца(отступ);
    блок.поставитьШиринуСтроки(ширина);
    while (input) {
        блок.заполнитьБлок(input);
        блок.обработать();
        std::cout << блок;
        блок.сброс();
    }
    блок.заполнитьБлок(input);
    std::cout << блок;

    return 0;
}
