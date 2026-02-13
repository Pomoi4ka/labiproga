#include <iostream>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

#ifndef VERSION
#define VERSION 1
#endif

#if VERSION == 1

float items[15];

int main()
{
    int i, count;
    int initial_count;

    const int cap = sizeof items / sizeof 0[items];

    const char input_file_name[] = "input.txt";
    const char output_file_name[] = "output.txt";

    std::ifstream input(input_file_name);
    std::ofstream output;

    cout << "Лаба №4. Автор: Соболевский Максим" << endl;

    if (!input) {
        cerr << "Проблема с файлом " << input_file_name << endl;
        return 1;
    }

    input >> count;
    if (!input || count < 0) {
        cerr << "Некорректное количество чисел" << endl;
        return 1;
    }

    if (count > cap) {
        cerr << "Слишком много чисел (макс. "
             << cap << ")" << endl;
        return 1;
    }

    for (i = 0; !input.eof(); ) {
        float garbage;
        if (i < count) input >> i[items];
        else input >> garbage;
        if (input) {
            i++;
            continue;
        }

        char next;
        input.clear(); // очень важная строчка, неё может начинаться майнинг
        do input.get(), next = input.peek();
        while (!input.eof() &&
               !(('0' <= next && next <= '9')
                 || next == '-'
                 || next == '.'));
    }

    if (i < count) {
        cerr << "Файл некорректен: чисел меньше заявленного" << endl;
        return 1;
    }

    if (i > count) {
        cerr << "Файл некорректен: чисел больше заявленного" << endl;
        return 1;
    }

    output.open(output_file_name);

    if (!output) {
        cerr << "Проблема с файлом" << output_file_name << endl;
        return 1;
    }

    initial_count = count;
    for (int i = 0; i < count; ++i) {
        size_t shift = 0;
        for (int j = i + 1; j < count; ++j) {
            (j - shift)[items] = j[items];
            if (j[items] == i[items]) ++shift;
        }
        count -= shift;
    }

    for (int i = 0; i < count; ++i) {
        if (i > 0) output << " ";
        output << i[items];
    }
    output << std::endl;

    std::cout << "Обработано " << initial_count
              << " элементов" << std::endl;
    return 0;
}

#elif VERSION == 2

int main()
{
    int count = 0;
    int cap = 0;
    float *items = 0;
    int initial_count;

    const char *input_file_name = "input.txt";
    const char *output_file_name = "output.txt";

    std::ifstream input(input_file_name);
    std::ofstream output;

    cout << "Лаба №4. Автор: Соболевский Максим" << endl;

    if (!input) {
        cerr << "Проблема с файлом " << input_file_name << endl;
        return 1;
    }

    while (!input.eof()) {
        char next = input.peek();
        if (!(('0' <= next && next <= '9')
              || next == '.' || next == '-')) {
            input.get();
            continue;
        }
        if (count >= cap) {
            cap = cap ? cap * 2 : 1;
            float *new_items = new float[cap];
            if (new_items == NULL) {
                cerr << "Не удалось выделить память" << endl;
                return 1;
            }
            for (int i = 0; i < count; ++i) {
                *(new_items + i) = *(items + i);
            }
            if (items) delete[] items;
            items = new_items;
        }
        input >> *(items + count);
        if (input.eof()) break;
        count++;
    }

    output.open(output_file_name);
    if (!output) {
        cerr << "Проблема с файлом" << output_file_name << endl;
        return 1;
    }

    initial_count = count;

    for (int i = 0; i < count; ++i) {
        int shift = 0;
        for (int j = i + 1; j < count; ++j) {
            *(items + j - shift) = *(items + j);
            if (*(items + j) == *(items + i)) ++shift;
        }
        count -= shift;
    }

    for (int i = 0; i < count; ++i) {
        if (i > 0) output << " ";
        output << *(items + i);
    }
    output << std::endl;

    std::cout << "Обработано " << initial_count
              << " элементов" << std::endl;

    delete[] items;
    return 0;
}
#endif
