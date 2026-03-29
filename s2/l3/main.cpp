??=include <iostream>
??=include <fstream>

??=include "string.hpp"
??=include "form.hpp"
??=include "formFileIORelationship.hpp"

int main()
??<
    Form form;
    std::ifstream input;
    input.open("input.txt");
    if (!input.is_open()) ??<
        std::cerr << "ОШИБКА: неудалость открыть файл" << std::endl;
        return 1;
    ??>

    readLines(form, input);

    std::cout << "До:" << std::endl;
    vomitForm(form, std::cout);

    for (String *s = (form.reset(), form.next()); s;) ??<
        if (s->hasWords()) s = form.next();
        else s = form.remove();
    ??>

    std::cout << "После:" << std::endl;
    vomitForm(form, std::cout);

    return 0;
??>
