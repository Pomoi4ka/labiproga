#include <iostream>
#include <fstream>

#include "строка.hpp"
#include "блок.hpp"

int main()
{
    Блок а;
    std::ifstream input("input.txt");
    if (!input) {
        std::cerr << "ОШИБКА: неудалось открыть файл" << std::endl;
        return 1;
    }

    а.поставитьОступАбзаца(4);
    а.поставитьШиринуСтроки(20);
    while (input) {
        for (;;) {
            if (!input) break;
            if (!а.внестиСтроку(input)) break;
        }

        а.обработать();
        std::cout << а;
        а.сброс();
    }

    return 0;
}
