#include <iostream>
#include <iomanip>
#include <fstream>

#include "consts.hpp"
#include "chunk.hpp"
#include "chunkForm.hpp"
#include "linesForm.hpp"

void readFile(LinesForm &form, std::istream &strm)
{
    ChunkForm *line = NULL;
    Chunk *chunk = NULL;
    for (;;) {
        char s;
        if (!strm.get(s)) break;
        if (!line) line = (form.append(), form.next());
        if (s == '\n') {
            line = (form.append(), form.next());
            chunk = NULL;
        } else {
            if (!chunk) chunk = (line->append(), line->next());
            if (!chunk->add(s))
                (chunk = (line->append(), line->next()))->add(s);
        }
    }
}

void writeForm(LinesForm &form, std::ostream &strm)
{
    bool first = true;
    ChunkForm *line;
    Chunk *chunk;

    line = (form.reset(), form.next());
    for (; line; line = form.next()) {
        size_t len = 0;
        strm << "(";
        chunk = (line->reset(), line->next());
        for (; chunk; chunk = line->next()) {
            len += chunk->len() + 2;
            strm << '"' << *chunk << '"';
            if (!line->hasNext()) continue;
            strm << " -> ";
            len += 4;
        }
        strm << ")";
        len += 2;
        len += first ? 3 : 6;
        first = false;
        if (!form.hasNext()) continue;
        strm << " -," << std::endl
             << "," << std::setfill('-') << std::setw(len)
             << "´" << std::endl << "`-> ";
    }
    strm << std::endl;
}

int main()
{
    LinesForm form;
    std::ifstream f;

    const char *filename = "input.txt";
    f.open(filename);
    if (!f.is_open()) {
        std::cerr << "ERROR: could not open file: " << filename << std::endl;
        return 1;
    }

    readFile(form, f);

    std::ostream &s = std::cout;

    s << "До:" << std::endl;
    writeForm(form, s);

    form.bsort();
    s << "После:" << std::endl;
    writeForm(form, s);

    return 0;
}
