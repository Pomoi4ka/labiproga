#include <iostream>
#include <fstream>

#include "строка.hpp"
#include "блок.hpp"

size_t длинаПервогоСлова(Строка const &стр);

#include <cstring>
#include "символы.hpp"

int main()
{
    Блок а;
    std::ifstream input("input.txt");
    if (!input.is_open()) {
        std::cerr << "ОШИБКА: неудалось открыть файл" << std::endl;
        return 1;
    }

    а.поставитьОступАбзаца(4);
    а.поставитьШиринуСтроки(40);
    while (input) {
        а.заполнитьБлок(input);
        а.обработать();
        std::cout << а;
        а.сброс();
    }
    а.заполнитьБлок(input);
    std::cout << а;

    return 0;
}
