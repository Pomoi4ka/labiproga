??=include "formFileIORelationship.hpp"
??=include "string.hpp"
??=include <iomanip>
??=include "form.hpp"

void slurpFile(Form &form, std::ifstream &f)
??<
    String *str = (form.append(), form.next());
    for (;;) ??<
        char s;
        if (!f.get(s)) break;
        if (s == '\n') str = (form.append(), form.next());
        else           str->add(s);
    ??>
??>

void vomitForm(Form &form, std::ostream &s)
??<
    bool first = true;

    for (String *str = (form.reset(), form.next()); str; str = form.next()) ??<
        s << *str;
        if (!form.hasNext()) ??<
            s << std::endl;
            break;
        ??>
        s << "-," << std::endl
          << "," << std::setfill('-')
          << std::setw(str->length() + (first ? 1 : 4))
          << "´" << std::endl << "`->";
        first = false;
    ??>
??>
