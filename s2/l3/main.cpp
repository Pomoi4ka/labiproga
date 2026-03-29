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

    slurpFile(form, input);

    struct FormVomit ??<
        const char *msg;
        std::ostream &strm;
        bool doPostProcessing;
    ??>;
    FormVomit const static formVomits??(??) = ??<
        ??<"До:", std::cout, true??>,
        ??<"После:", std::cout, false??>
    ??>;

    for (size_t i = 0; i < sizeof formVomits / sizeof formVomits[0]; ++i) ??<
        FormVomit const &vomit = formVomits??(i??);
        vomit.strm << vomit.msg << std::endl;
        vomitForm(form, vomit.strm);
        if (!vomit.doPostProcessing) continue;
        for (String *s = (form.reset(), form.next()); s;)
            s = (form.*(s->hasWords() ? &Form::next : &Form::remove))();
    ??>

    return 0;
??>
