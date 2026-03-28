#include <iostream>
#include <fstream>
#include <iomanip>

#include "string.hpp"
#include "form.hpp"

void readLines(Form &form, std::ifstream &f)
{
    String *str = &(form.append(), form.next());
    for (;;) {
        char s;
        if (!f.get(s)) break;
        if (s == '\n') str = &(form.append(), form.next());
        else           str->add(s);
    }
}

void vomitForm(Form &form, std::ostream &s)
{
    form.reset();
    bool first = true;
    do {
        String &str = form.next();
        s << str;
        if (!form.hasNext()) {
            s << std::endl;
            break;
        }
        s << "-," << std::endl;
        s << "," << std::setfill('-') << std::setw(str.length() + (first ? 1 : 4))
          << "`" << std::endl << "`->";
        first = false;
    } while (form.hasNext());
}

int main()
{
    Form form;
    std::ifstream input;
    input.open("input.txt");
    if (!input.is_open()) {
        std::cerr << "ОШИБКА: неудалость открыть файл" << std::endl;
        return 1;
    }

    readLines(form, input);
    form.reset();
    while (form.hasNext()) {
        if (!form.next().hasWords()) form.remove();
    }
    vomitForm(form, std::cout);

    return 0;
}
