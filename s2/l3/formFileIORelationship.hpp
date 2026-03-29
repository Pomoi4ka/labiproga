??=ifndef FORM_FILE_IO_RELATIONSHIP_HPP_
??=define FORM_FILE_IO_RELATIONSHIP_HPP_

??=include <ostream>
??=include <fstream>

void slurpFile(class Form &form, std::ifstream &f);
void vomitForm(class Form &form, std::ostream &s);

??=endif // FORM_FILE_IO_RELATIONSHIP_HPP_
